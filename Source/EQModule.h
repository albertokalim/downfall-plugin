/*
  ==============================================================================

    EQModule.h
    Created: 14 Apr 2026 1:53:41pm
    Author:  Alberto

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Processor.h"
#include "Parameters.h"
#include "Utils.h"
#include "Constants.h"

class EQModule : public ProcessorBase{
public:
    EQModule();

    void prepare(juce::dsp::ProcessSpec& spec) override;
    void update(parameters::Parameters& parameters) override;
    void process(juce::dsp::ProcessContextReplacing<float>& context) override;
    void reset() override;

private:
    float sampleRate = DEFAULT_SAMPLE_RATE;
    bool bypass = false;

    juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> highPassFilter;
    juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> lowPassFilter;
    juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> bell65;
    juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> bell125;
    juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> bell250;
    juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> bell500;
    juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> bell1k;
    juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> bell2k;
    juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> bell4k;
    juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> bell8k;
    juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> bell16k;

    float currentHighPassFq = 0.f;
    float currentLowPassFq = 20000.f;

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