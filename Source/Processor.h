/*
  ==============================================================================

    Processor.h
    Created: 30 Apr 2026 5:23:50pm
    Author:  Alberto

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Parameters.h"

class ProcessorBase {
public:
    virtual ~ProcessorBase() {}

    virtual void prepare(juce::dsp::ProcessSpec& spec) = 0;

    virtual void update(parameters::Parameters& parameters) = 0;

    virtual void process(juce::dsp::ProcessContextReplacing<float>& context) = 0;

    virtual void reset() = 0;
};