/*
  ==============================================================================

    Difusser.cpp
    Created: 25 Mar 2026 10:07:38am
    Author:  Alberto

  ==============================================================================
*/

#include "Diffuser.h"
#include "Utils.h"

effects::Diffuser::Diffuser(float _delayTime) : frame(new float[REVERB_CHANNELS])
{
    delayTime = _delayTime;
    for (int i = 0; i < REVERB_CHANNELS; ++i) {
        flipPolarity[i] = false;
        frame[i] = 0.f;
    }
}

effects::Diffuser::~Diffuser()
{
    delete frame;
}

void effects::Diffuser::prepare(juce::dsp::ProcessSpec& spec)
{
    juce::dsp::ProcessSpec oneChannelSpec;
    oneChannelSpec.maximumBlockSize = spec.maximumBlockSize;
    oneChannelSpec.numChannels = 1;
    oneChannelSpec.sampleRate = spec.sampleRate;

    float delaySamplesBase = delayTime * 0.001f * oneChannelSpec.sampleRate;
    for (int i = 0; i < REVERB_CHANNELS; ++i) {
        auto& delayLine = delays[i];
        delayLine.prepare(oneChannelSpec);
        delayLine.reset();
        float rangeLow = delaySamplesBase * i / REVERB_CHANNELS;
        float rangeHigh = delaySamplesBase * (i + 1) / REVERB_CHANNELS;
        float delayInSamples = randomInRange(rangeLow, rangeHigh);
        float ceilDelaySamples = std::ceilf(delayInSamples);
        delayLine.setMaximumDelayInSamples(ceilDelaySamples);
        delayLine.setDelay(ceilDelaySamples);
        flipPolarity[i] = rand() % 2;
        frame[i] = 0.f;
    }
}

void effects::Diffuser::process(Splitter& split)
{
    for (int c = 0; c < REVERB_CHANNELS; ++c) {
        output[c] = split.getAudioBuffer(c).getWritePointer(0);
        jassert(output[c] != nullptr);
    }
    int numSamples = split.getAudioBuffer(0).getNumSamples();

    for (int sample = 0; sample < numSamples; ++sample) {
        for (int c = 0; c < REVERB_CHANNELS; ++c) {
            delays[c].pushSample(0, output[c][sample]);
            frame[c] = delays[c].popSample(0);
        }
        Hadamard(frame);
        for (int c = 0; c < REVERB_CHANNELS; ++c) {
            if (flipPolarity[c]) {
                frame[c] *= -1.f;
            }
            output[c][sample] = frame[c];
        }
    }
}

void effects::Diffuser::Hadamard(float* input)
{
    HadamardRecursive(input, REVERB_CHANNELS);
    for (int c = 0; c < REVERB_CHANNELS; ++c) {
        input[c] *= scalingFactor;
    }
}

void effects::Diffuser::HadamardRecursive(float* input, int size)
{
    if (size <= 1) return;
    const int hSize = size / 2;

    HadamardRecursive(input, hSize);
    HadamardRecursive(input + hSize, hSize);

    for (int i = 0; i < hSize; ++i) {
        float a = input[i];
        float b = input[i + hSize];
        input[i] = (a + b);
        input[i + hSize] = (a - b);
    }
}

void effects::Diffuser::reset()
{
    for (int i = 0; i < REVERB_CHANNELS; ++i) {
        delays[i].reset();
        frame[i] = 0.f;
    }
}
