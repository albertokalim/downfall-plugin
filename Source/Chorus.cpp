/*
  ==============================================================================

    Chorus.cpp
    Created: 30 Apr 2026 6:02:37pm
    Author:  Alberto

  ==============================================================================
*/

#include "Chorus.h"

void effects::ChorusFX::prepare(juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;

    split.prepare(spec.maximumBlockSize);

    juce::dsp::ProcessSpec oneChannelSpec;
    oneChannelSpec.maximumBlockSize = spec.maximumBlockSize;
    oneChannelSpec.numChannels = 1;
    oneChannelSpec.sampleRate = spec.sampleRate;

    for (int i = 0; i < CHORUS_CHANNELS; i++) {
        delayLines[i].prepare(oneChannelSpec);
        double numSamples = MAX_CHORUS_DELAY / 1000.0 * spec.sampleRate;
        int maxDelayInSamples = int(std::ceil(numSamples));
        delayLines[i].setMaximumDelayInSamples(maxDelayInSamples);
        delayLines[i].reset();
        lfos[i].prepare(oneChannelSpec);
    }
    
    prepareSmoothedValueObject(sampleRate, rateSmoother, 1.f);
    prepareSmoothedValueObject(sampleRate, widthSmoother, 0.4f);
    prepareSmoothedValueObject(sampleRate, mixSmoother, 0.5f);
}

void effects::ChorusFX::update(parameters::Parameters& parameters)
{
    bypass = parameters.chorusBypass.get();

    rateSmoother.setTargetValue(parameters.rate.get());

    widthSmoother.setTargetValue(parameters.width.get() / 100.f);

    mixSmoother.setTargetValue(parameters.chorusMix.get() / 100.f);
}

void effects::ChorusFX::process(juce::dsp::ProcessContextReplacing<float>& context)
{
    if (bypass) {
        return;
    }

    
    auto outputBlock = context.getOutputBlock();

    split.clearAudioBuffers();
    split.split(context);

    for (int sample = 0; sample < outputBlock.getNumSamples(); ++sample) {
        auto& buffers = split.getAudioBuffers();
        for (int i = 0; i < CHORUS_CHANNELS; i++) {
            float* buffer = buffers[i]->getWritePointer(0);
            lfos[i].setFrequency(rateSmoother.getNextValue());

            float lfoNextValue = lfos[i].processSample(0.f);
            float lfoValue = lfoNextValue * widthSmoother.getNextValue();
            if (lfoValue < 0) lfoValue *= -1;
            float delayTime = mapValueInRange(lfoValue, MIN_CHORUS_DELAY, MAX_CHORUS_DELAY);
            float delayInSamples = delayTime / 1000.0f * sampleRate;
            delayLines[i].setDelay(delayInSamples);

            float dry = buffer[sample];

            delayLines[i].pushSample(0, dry);

            float wet = delayLines[i].popSample(0);

            float mixParam = mixSmoother.getNextValue();

            float mix = dry + wet * mixParam; //mix param

            buffer[sample] = mix;
        }

        float scaling = (float) outputBlock.getNumChannels() / (float)CHORUS_CHANNELS;
        if (outputBlock.getNumChannels() > 1) {
            float* outL = outputBlock.getChannelPointer(0);
            float* outR = outputBlock.getChannelPointer(1);
            outL[sample] += buffers[0]->getSample(0, sample) * scaling;
            outR[sample] += buffers[1]->getSample(0, sample) * scaling;
        }
        else {
            float* outL = outputBlock.getChannelPointer(0);
            outL[sample] += buffers[0]->getSample(0, sample) * scaling;
            outL[sample] += buffers[1]->getSample(0, sample) * scaling;
        }
    }
}

void effects::ChorusFX::reset()
{
    split.clearAudioBuffers();
    for (int i = 0; i < CHORUS_CHANNELS; ++i) {
        delayLines[i].reset();
    }
}