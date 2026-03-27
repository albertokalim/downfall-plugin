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

    for (int i = 0; i < 4u; ++i) {
        auto& delayLine = delays[i];
        delayLine.prepare(oneChannelSpec);
        
        double maxNumSamples =  5000.f / 1000.0 * spec.sampleRate;
        int maxDelayInSamples = int(std::ceil(maxNumSamples));
        delayLine.setMaximumDelayInSamples(maxDelayInSamples);

        double numSamples = delayTimes[i] / 1000.0 * spec.sampleRate;
        delayLine.setDelay(numSamples);

        allPassFilters[i].reset();
        *allPassFilters[i].state = *juce::dsp::IIR::Coefficients<float>::makeAllPass(spec.sampleRate, allPassFrequencies[i]);
        allPassFilters[i].prepare(spec);
    }
}

void effects::Diffuser::process(Splitter& split)
{
    for (int i = 0; i < 4u; ++i) {
        //1. Delay
        auto& delayLine = delays[i];
        auto& buffer = split.getAudioBuffer(i);
        juce::dsp::AudioBlock<float> block(buffer);
        juce::dsp::ProcessContextReplacing<float> context(block);
        delayLine.process(context);

        //2. allpass filter
        allPassFilters[i].process(context);
    }

    //3. Hadamard mixing matrix.
    const float* input[4u];
    float* output[4u];
    for (int i = 0; i < 4u; ++i) {
        input[i] = split.getAudioBuffer(i).getReadPointer(0);
        output[i] = split.getAudioBuffer(i).getWritePointer(0);
    }

    //number of samples is the same for all the audio buffers
    for (int sample = 0; sample < split.getAudioBuffer(0).getNumSamples(); ++sample) {
        float x00 = input[0][sample] * hadamard[0][0];
        float x01 = input[1][sample] * hadamard[0][1];
        float x02 = input[2][sample] * hadamard[0][2];
        float x03 = input[3][sample] * hadamard[0][3];

        output[0][sample] = (x00 + x01 + x02 + x03) * 0.5f; // 1/sqrt(n), n = 4 

        float x10 = input[0][sample] * hadamard[1][0];
        float x11 = input[1][sample] * hadamard[1][1];
        float x12 = input[2][sample] * hadamard[1][2];
        float x13 = input[3][sample] * hadamard[1][3];

        output[1][sample] = (x10 + x11 + x12 + x13) * 0.5f;

        float x20 = input[0][sample] * hadamard[2][0];
        float x21 = input[1][sample] * hadamard[2][1];
        float x22 = input[2][sample] * hadamard[2][2];
        float x23 = input[3][sample] * hadamard[2][3];

        output[2][sample] = (x20 + x21 + x22 + x23) * 0.5f;

        float x30 = input[0][sample] * hadamard[3][0];
        float x31 = input[1][sample] * hadamard[3][1];
        float x32 = input[2][sample] * hadamard[3][2];
        float x33 = input[3][sample] * hadamard[3][3];

        output[3][sample] = (x30 + x31 + x32 + x33) * 0.5f;
    }
}