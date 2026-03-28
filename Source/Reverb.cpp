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
    
    for (int i = 0; i < REVERB_CHANNELS; ++i) {
        highCutFilter[i].setType(juce::dsp::StateVariableTPTFilterType::lowpass);
        feedbacks[i] = 0.f;
    }
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

        highCutFilter[i].prepare(oneChannelSpec);
        highCutFilter[i].reset();
        highCutFilter[i].setCutoffFrequency(6000.f);
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

    const float* input[REVERB_CHANNELS];
    float* output[REVERB_CHANNELS];
    for (int i = 0; i < REVERB_CHANNELS; ++i) {
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

        delays[0].pushSample(0, feedbacks[0]);
        delays[1].pushSample(0, feedbacks[1]);
        delays[2].pushSample(0, feedbacks[2]);
        delays[3].pushSample(0, feedbacks[3]);

        float x0delay = delays[0].popSample(0);
        float x1delay = delays[1].popSample(0);
        float x2delay = delays[2].popSample(0);
        float x3delay = delays[3].popSample(0);

        //Householder mixing matrix
        float sum = x0delay + x1delay + x2delay + x3delay;
        float factor = 0.5f * sum;
        
        float x0wet = x0delay - factor;
        float x1wet = x1delay - factor;
        float x2wet = x2delay - factor;
        float x3wet = x0delay - factor;

        float mix0 = x0dry + x0wet * decay.getNextValue();
        float mix1 = x1dry + x1wet * decay.getNextValue();
        float mix2 = x2dry + x2wet * decay.getNextValue();
        float mix3 = x3dry + x3wet * decay.getNextValue();

        feedbacks[0] = highCutFilter[0].processSample(0, mix0);
        feedbacks[1] = highCutFilter[1].processSample(0, mix1);
        feedbacks[2] = highCutFilter[2].processSample(0, mix2);
        feedbacks[3] = highCutFilter[3].processSample(0, mix3);

        float resultL = (mix0 + mix2) * 0.5f;
        float resultR = (mix1 + mix3) * 0.5f;

        float mixValue = mix.getNextValue();
        outL[sample] = resultL * mixValue + inputL[sample] * (1 - mixValue);
        outR[sample] = resultR * mixValue + inputR[sample] * (1 - mixValue);
    }
}

void effects::ReverbFX::reset()
{
    for (int i = 0; i < REVERB_CHANNELS; ++i){
        feedbacks[i] = 0.f;
    }
}
