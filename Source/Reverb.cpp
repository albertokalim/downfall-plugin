/*
  ==============================================================================

    Reverb.cpp
    Created: 21 Mar 2026 1:16:20pm
    Author:  Alberto

  ==============================================================================
*/

#include "Reverb.h"

effects::ReverbFX::ReverbFX(float _delayTime)
{
    delayTime = _delayTime;
}

void effects::ReverbFX::prepare(juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;
    split.prepare(spec.maximumBlockSize);
    for (int i = 0; i < DIFF_STEPS; ++i) {
        diff[i].prepare(spec);
    }

    juce::dsp::ProcessSpec oneChannelSpec;
    oneChannelSpec.maximumBlockSize = spec.maximumBlockSize;
    oneChannelSpec.numChannels = 1;
    oneChannelSpec.sampleRate = spec.sampleRate;

    highShelfCut.reset();
    highShelfCut.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate, 8000.f, 0.3f, 0.9f);
    highShelfCut.prepare(oneChannelSpec);

    float delaySamplesBase = delayTime / 1000.0f * spec.sampleRate;
    for (int i = 0; i < REVERB_CHANNELS; ++i) {
        auto& delayLine = delays[i];
        delayLine.prepare(oneChannelSpec);
        delayLine.reset();
        float r = i / delaySamplesBase;
        float delayInSamples = (float) std::pow(2, r) * delaySamplesBase;
        float ceilDelaySamples = std::ceilf(delayInSamples);
        delayLine.setMaximumDelayInSamples(ceilDelaySamples);
        delayLine.setDelay(ceilDelaySamples);
        delayedSamples[i] = 0.f;
    }

    decay.reset(0.002f);
    decay.setCurrentAndTargetValue(0.f);

    mix.reset(0.002f);
    mix.setCurrentAndTargetValue(0.5f);

    
}

void effects::ReverbFX::update(parameters::Parameters& parameters)
{
    bypass = parameters.reverbBypass.get();
    decay.setTargetValue(parameters.decay.get() / 100.f);
    highShelfCut.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate,
        8000.f, 
        0.3f,
        juce::jlimit<float>(0.f, 0.9f, decay.getNextValue()));
    mix.setTargetValue(parameters.reverbMix.get() / 100.f);
}

void effects::ReverbFX::process(juce::dsp::ProcessContextReplacing<float>& context)
{
    if (bypass)
        return;

    split.clearAudioBuffers();
    split.split(context);
    for (int i = 0; i < DIFF_STEPS; ++i) {
        diff[i].process(split);
    }

    for (int c = 0; c < REVERB_CHANNELS; ++c) {
        output[c] = split.getAudioBuffer(c).getWritePointer(0);
        jassert(output[c] != nullptr);
        delayedSamples[c] = 0.f;
    }
    
    for (int sample = 0; sample < split.getAudioBuffer(0).getNumSamples(); ++sample) {
        for (int c = 0; c < REVERB_CHANNELS; ++c) {
            delayedSamples[c] = delays[c].popSample(0);
        }
        Householder(delayedSamples);
        for (int c = 0; c < REVERB_CHANNELS; ++c) {
            float filteredValue = highShelfCut.processSample(delayedSamples[c]);
            highShelfCut.reset();
            float mixedValue = delayedSamples[c] * decay.getNextValue();
            float sum = output[c][sample] + mixedValue;
            delays[c].pushSample(0, sum);
            output[c][sample] = sum;
        }

        float left = 0.f;
        float right = 0.f;
        for (int c = 0; c < REVERB_CHANNELS; c += 2) {
            left += output[c][sample];
            right += output[c + 1][sample];
        }

        auto& outputBlock = context.getOutputBlock();
        float scaling = (float) outputBlock.getNumChannels() / (float) REVERB_CHANNELS;
        float mixParam = mix.getNextValue();
        if (outputBlock.getNumChannels() > 1) {
            float* outL = outputBlock.getChannelPointer(0);
            float* outR = outputBlock.getChannelPointer(1);
            outL[sample] += left * scaling * mixParam;
            outR[sample] += right * scaling * mixParam;
        }
        else {
            float* outL = outputBlock.getChannelPointer(0);
            outL[sample] = left * scaling;
        }
    }
}

void effects::ReverbFX::reset()
{
    split.clearAudioBuffers();
    for (int i = 0; i < REVERB_CHANNELS; ++i) {
        delayedSamples[i] = 0.f;
        delays[i].reset();
    }
    for (int i = 0; i < DIFF_STEPS; ++i) {
        diff[i].reset();
    }
}

void effects::ReverbFX::Householder(std::array<float, REVERB_CHANNELS>& inputs)
{
    float sum = 0.f;
    for (int c = 0; c < REVERB_CHANNELS; ++c) {
        sum += inputs[c];
    }
    sum *= multiplier;
    for (int c = 0; c < REVERB_CHANNELS; ++c) {
        inputs[c] += sum;
    }
}
