/*
  ==============================================================================

    Difusser.cpp
    Created: 25 Mar 2026 10:07:38am
    Author:  Alberto

  ==============================================================================
*/

#include "Diffuser.h"

void effects::Diffuser::prepare(juce::dsp::ProcessSpec& spec)
{
    juce::dsp::ProcessSpec oneChannelSpec;
    oneChannelSpec.maximumBlockSize = spec.maximumBlockSize;
    oneChannelSpec.numChannels = 1;
    oneChannelSpec.sampleRate = spec.sampleRate;

    for (int i = 0; i < REVERB_CHANNELS; ++i) {
        auto& delayLine = delays[i];
        delayLine.prepare(oneChannelSpec);
        
        double maxNumSamples =  5000.f / 1000.0 * spec.sampleRate;
        int maxDelayInSamples = int(std::ceil(maxNumSamples));
        delayLine.setMaximumDelayInSamples(maxDelayInSamples);

        double numSamples = delayTimes[i] / 1000.0 * spec.sampleRate;
        delayLine.setDelay(numSamples);
    }
}

void effects::Diffuser::process(Splitter& split)
{
    //Delay
    for (int i = 0; i < REVERB_CHANNELS; ++i) {
        auto& delayLine = delays[i];
        auto& buffer = split.getAudioBuffer(i);
        juce::dsp::AudioBlock<float> block(buffer);
        juce::dsp::ProcessContextReplacing<float> context(block);
        delayLine.process(context);
    }

    //Hadamard mixing matrix.
    const float* input[REVERB_CHANNELS];
    float* output[REVERB_CHANNELS];
    for (int i = 0; i < REVERB_CHANNELS; ++i) {
        input[i] = split.getAudioBuffer(i).getReadPointer(0);
        output[i] = split.getAudioBuffer(i).getWritePointer(0);
    }
    
    for (int sample = 0; sample < split.getAudioBuffer(0).getNumSamples(); ++sample) {
        float x0 = input[0][sample];
        float x1 = input[1][sample];
        float x2 = input[2][sample];
        float x3 = input[3][sample];

        output[0][sample] = (x0 + x1 + x2 + x3) * 0.5f; // 1/sqrt(n), n = 4 

        output[1][sample] = (x0 - x1 + x2 - x3) * 0.5f;

        output[2][sample] = (x0 + x1 - x2 - x3) * 0.5f;

        output[3][sample] = (x0 - x1 - x2 + x3) * 0.5f;
    }

    //allpass filter
    for (int i = 0; i < REVERB_CHANNELS; ++i) {
        auto& buffer = split.getAudioBuffer(i);
        juce::dsp::AudioBlock<float> block(buffer);
        juce::dsp::ProcessContextReplacing<float> context(block);
        if (flipPolarity[i]) {
            block.multiplyBy(-1);
        }
    }
}