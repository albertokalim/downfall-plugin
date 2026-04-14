/*
  ==============================================================================

    EQModule.h
    Created: 14 Apr 2026 1:53:41pm
    Author:  Alberto

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Parameters.h"

using IIRFilter = juce::dsp::IIR::Filter<float>;

class EQModule {
public:
    EQModule();

    void prepare(juce::dsp::ProcessSpec& spec);
    void update(parameters::Parameters& parameters);
    void process(juce::dsp::ProcessContextReplacing<float>& context);
    void reset();

private:
    float sampleRate = 44100.f;

    IIRFilter highPassFilter;
    IIRFilter lowPassFilter;
    IIRFilter bell65;
    IIRFilter bell125;
    IIRFilter bell250;
    IIRFilter bell500;
    IIRFilter bell1k;
    IIRFilter bell2k;
    IIRFilter bell4k;
    IIRFilter bell8k;
    IIRFilter bell16k;

    juce::SmoothedValue<float> highPassCenterFqSmoother;
    juce::SmoothedValue<float> lowPassCenterFqSmoother;
    juce::SmoothedValue<float> gain65Smoother;
    juce::SmoothedValue<float> gain125Smoother;
    juce::SmoothedValue<float> gain250Smoother;
    juce::SmoothedValue<float> gain500Smoother;
    juce::SmoothedValue<float> gain1kSmoother;
    juce::SmoothedValue<float> gain2kSmoother;
    juce::SmoothedValue<float> gain4kSmoother;
    juce::SmoothedValue<float> gain8kSmoother;
    juce::SmoothedValue<float> gain16kSmoother;
};