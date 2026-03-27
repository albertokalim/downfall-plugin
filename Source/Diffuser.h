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
        std::array<float, 4u> delayTimes = { 31.f, 37.f, 41.f, 43.f };

        std::array<juce::dsp::DelayLine<float>, 4u> delays;
        std::array<juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs>, 4u> allPassFilters;
        std::array<float, 4u> allPassFrequencies = { 450.f, 1100.f, 1900.f, 2750.f };
        float hadamard[4][4] = {
            {1,1,1,1},
            {1,-1,1,-1},
            {1,1,-1,-1},
            {1,-1,-1,1}
        };
    };
}