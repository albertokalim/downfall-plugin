/*
  ==============================================================================

    Reverb.h
    Created: 21 Mar 2026 1:16:20pm
    Author:  Alberto

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Splitter.h"
#include "Diffuser.h"
#include "Effect.h"

namespace effects {
    class ReverbFX : public FX {
    public:
        ReverbFX();
        void prepare(juce::dsp::ProcessSpec& spec) override;
        void update(parameters::FXParameters& parameters) override;
        void process(juce::dsp::ProcessContextReplacing<float>& context) override;
        void reset() override;

    private:
        std::array<float, 4u> delayTimes{ 31.f, 37.f, 41.f, 43.f };

        Splitter split;
        std::array<Diffuser, 8u> diff;
        std::array<juce::dsp::DelayLine<float>, 4u> delays;
        std::array<float, 4u> feedbacks;
        juce::dsp::StateVariableTPTFilter<float> highCutFilter; //low-pass filter

        juce::SmoothedValue<float> decay;
        juce::SmoothedValue<float> mix;
    };
}