/*
  ==============================================================================

    Difusser.h
    Created: 25 Mar 2026 10:07:38am
    Author:  Alberto

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Splitter.h"

using IIRFilter = juce::dsp::IIR::Filter<float>;
using IIRCoefs = juce::dsp::IIR::Coefficients<float>;
namespace effects {
    class Diffuser {
    public:
        void prepare(juce::dsp::ProcessSpec& spec);
        void process(Splitter& split);

    private:
        std::array<float, REVERB_CHANNELS> delayTimes = { 13.3f, 33.3f, 66.6f, 100.f };
        std::array<juce::dsp::DelayLine<float>, REVERB_CHANNELS> delays;
        std::array<bool, REVERB_CHANNELS> flipPolarity = { true, false, false, true };
    };
}