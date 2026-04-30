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

    delayLine.prepare(spec);
    double numSamples = MAX_DELAY_TIME / 1000.0 * spec.sampleRate;
    int maxDelayInSamples = int(std::ceil(numSamples));
    delayLine.setMaximumDelayInSamples(maxDelayInSamples);
    delayLine.reset();

    lfo.prepare(spec);

    rateSmoother.reset(spec.sampleRate, 0.002f);
    rateSmoother.setCurrentAndTargetValue(1.f);

    widthSmoother.reset(spec.sampleRate, 0.002f);
    widthSmoother.setCurrentAndTargetValue(0.4f);

    mixSmoother.reset(spec.sampleRate, 0.002f);
    mixSmoother.setCurrentAndTargetValue(0.5f);
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

    auto inputBlock = context.getInputBlock();
    auto outputBlock = context.getOutputBlock();

    for (int sample = 0; sample < inputBlock.getNumSamples(); ++sample) {
        lfo.setFrequency(rateSmoother.getNextValue());

        float lfoNextValue = lfo.processSample(0.f);
        float lfoValue = lfoNextValue * widthSmoother.getNextValue();
        float delayTime = mapValueInRange(lfoValue, MIN_CHORUS_DELAY, MAX_CHORUS_DELAY);
        float delayInSamples = delayTime / 1000.0f * sampleRate;
        delayLine.setDelay(delayInSamples);

        float dryL = inputBlock.getSample(0, sample);
        float dryR = inputBlock.getSample(1, sample);

        delayLine.pushSample(0, dryL);
        delayLine.pushSample(1, dryR);

        float wetL = delayLine.popSample(0);
        float wetR = delayLine.popSample(1);

        float mix = mixSmoother.getNextValue();

        float mixL = dryL + wetL * mix; //mix param
        float mixR = dryR + wetR * mix;

        outputBlock.setSample(0, sample, mixL);
        outputBlock.setSample(1, sample, mixR);
    }
}

void effects::ChorusFX::reset()
{
}