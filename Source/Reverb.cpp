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
    highCutFilter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    for (int i = 0; i < 4u; ++i) {
        feedbacks[i] = 0.f;
    }
}

void effects::ReverbFX::prepare(juce::dsp::ProcessSpec& spec)
{
    split.prepare(spec.maximumBlockSize);
    for (int i = 0; i < 8u; ++i) {
        diff[i].prepare(spec);
    }

    juce::dsp::ProcessSpec oneChannelSpec;
    oneChannelSpec.maximumBlockSize = spec.maximumBlockSize;
    oneChannelSpec.numChannels = 1;
    oneChannelSpec.sampleRate = spec.sampleRate;

    for (int i = 0; i < 4u; ++i) {
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

    highCutFilter.prepare(spec);
    highCutFilter.reset();
    highCutFilter.setCutoffFrequency(6000.f);
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
    for (int i = 0; i < 8u; ++i) {
        diff[i].process(split);
    }

    const float* input[4u];
    float* output[4u];
    for (int i = 0; i < 4u; ++i) {
        input[i] = split.getAudioBuffer(i).getReadPointer(0);
        output[i] = split.getAudioBuffer(i).getWritePointer(0);
    }

    auto& stereoOutput = context.getOutputBlock();
    const auto& stereoInput = context.getInputBlock();
    const auto* inputL = stereoInput.getChannelPointer(0);
    const auto* inputR = stereoInput.getChannelPointer(1);
    auto* outL = stereoOutput.getChannelPointer(0);
    auto* outR = stereoOutput.getChannelPointer(1);

    for (int sample = 0; sample < split.getAudioBuffer(0).getNumSamples(); ++sample) {
        float x0dry = input[0][sample];
        float x1dry = input[1][sample];
        float x2dry = input[2][sample];
        float x3dry = input[3][sample];

        delays[0].pushSample(0, x0dry + feedbacks[0]);
        delays[1].pushSample(0, x1dry + feedbacks[1]);
        delays[2].pushSample(0, x2dry + feedbacks[2]);
        delays[3].pushSample(0, x3dry + feedbacks[3]);

        float x0wet = delays[0].popSample(0);
        float x1wet = delays[1].popSample(0);
        float x2wet = delays[2].popSample(0);
        float x3wet = delays[3].popSample(0);

        //Householder mixing matrix
        float sum = x0wet + x1wet + x2wet + x3wet;
        float factor = 0.5f * sum;

        feedbacks[0] = highCutFilter.processSample(0, (x0wet - factor) * decay.getNextValue());
        feedbacks[1] = highCutFilter.processSample(0, (x1wet - factor) * decay.getNextValue());
        feedbacks[2] = highCutFilter.processSample(0, (x2wet - factor) * decay.getNextValue());
        feedbacks[3] = highCutFilter.processSample(0, (x3wet - factor) * decay.getNextValue());

        float mix0 = (x0wet + x0dry) * 0.5f;
        float mix1 = (x1wet + x1dry) * 0.5f;
        float mix2 = (x2wet + x2dry) * 0.5f;
        float mix3 = (x3wet + x3dry) * 0.5f;

        output[0][sample] = mix0;
        output[1][sample] = mix1;
        output[2][sample] = mix2;
        output[3][sample] = mix3;

        float resultL = (output[0][sample] + output[2][sample]) * 0.5f;
        float resultR = (output[1][sample] + output[3][sample]) * 0.5f;

        float mixValue = mix.getNextValue();
        outL[sample] = (resultL + inputL[sample])  * mixValue;
        outR[sample] = (resultR + inputR[sample])  * mixValue;
    }
}

void effects::ReverbFX::reset()
{
    for (int i = 0; i < 4u; ++i){
        feedbacks[i] = 0.f;
    }
}
