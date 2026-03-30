/*
  ==============================================================================

    Reverb.cpp
    Created: 21 Mar 2026 1:16:20pm
    Author:  Alberto

  ==============================================================================
*/

#include "Reverb.h"

effects::ReverbFX::ReverbFX()
{

}

void effects::ReverbFX::prepare(juce::dsp::ProcessSpec& spec)
{
    split.prepare(spec.maximumBlockSize);
    for (int i = 0; i < DIFF_STEPS; ++i) {
        diff[i].prepare(spec);
    }

    juce::dsp::ProcessSpec oneChannelSpec;
    oneChannelSpec.maximumBlockSize = spec.maximumBlockSize;
    oneChannelSpec.numChannels = 1;
    oneChannelSpec.sampleRate = spec.sampleRate;

    for (int i = 0; i < REVERB_CHANNELS; ++i) {
        auto& delayLine = delays[i];
        delayLine.prepare(oneChannelSpec);

        double maxNumSamples = 5000.f / 1000.0 * spec.sampleRate;
        int maxDelayInSamples = int(std::ceil(maxNumSamples));
        delayLine.setMaximumDelayInSamples(maxDelayInSamples);

        double numSamples = delayTimes[i] / 1000.0 * spec.sampleRate;
        delayLine.setDelay(numSamples);
    }

    decay.reset(0.002f);
    decay.setCurrentAndTargetValue(0.f);

    mix.reset(0.002f);
    mix.setCurrentAndTargetValue(0.5f);

    
}

void effects::ReverbFX::update(parameters::FXParameters& parameters)
{
    bypass = parameters.getBypassEffect().get();

    parameters::ReverbParameters& reverbParams = dynamic_cast<parameters::ReverbParameters&>(parameters);
    decay.setTargetValue(reverbParams.getDecay().get() / 100.f);
    mix.setTargetValue(reverbParams.getMix().get() / 100.f);
}

void effects::ReverbFX::process(juce::dsp::ProcessContextReplacing<float>& context)
{
    if (bypass)
        return;

    split.split(context);
    for (int i = 0; i < DIFF_STEPS; ++i) {
        diff[i].process(split);
    }

    float* output[REVERB_CHANNELS];
    for (int c = 0; c < REVERB_CHANNELS; ++c) {
        output[c] = split.getAudioBuffer(c).getWritePointer(0);
    }

    std::vector<float> delayedSamples;
    for (int c = 0; c < REVERB_CHANNELS; ++c) {
        delayedSamples.insert(delayedSamples.begin() + c, 0.f);
    }
    
    //Creo que el problema está en el feedback.
    for (int sample = 0; sample < split.getAudioBuffer(0).getNumSamples(); ++sample) {
        for (int c = 0; c < REVERB_CHANNELS; ++c) {
            delayedSamples[c] = delays[c].popSample(0);
        }
        Householder(delayedSamples);
        for (int c = 0; c < REVERB_CHANNELS; ++c) {
            float mixedValue = delayedSamples[c] * mix.getTargetValue() * decay.getNextValue();
            float sum = output[c][sample] + mixedValue;
            delays[c].pushSample(0, sum);
            output[c][sample] = sum;
        }
    }
}

void effects::ReverbFX::reset()
{
}

void effects::ReverbFX::Householder(std::vector<float>& buffer)
{
    float sum = 0.f;
    for (int c = 0; c < REVERB_CHANNELS; ++c) {
        sum += buffer[c];
    }
    sum *= multiplier;
    for (int c = 0; c < REVERB_CHANNELS; ++c) {
        buffer[c] += sum;
    }
}
