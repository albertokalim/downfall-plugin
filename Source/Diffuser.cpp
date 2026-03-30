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
    float* output[REVERB_CHANNELS];
    int numSamples = split.getAudioBuffer(0).getNumSamples();
    for (int c = 0; c < REVERB_CHANNELS; ++c) {
        output[c] = split.getAudioBuffer(c).getWritePointer(0);
        Hadamard(output[c], numSamples);
    }
}

void effects::Diffuser::Hadamard(float* buffer, int numSamples)
{
    for (int sample = 0; sample < numSamples; ++sample) {
        float a = buffer[sample];
        if (sample + 1 < numSamples) {
            float b = buffer[sample + 1];
            buffer[sample] = a + b;
            buffer[sample + 1] = a - b;
        }
        buffer[sample] *= scalingFactor;
        //flip polarity
        if (rand() % 2) {
            buffer[sample] *= -1;
        }
    }
}
