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
#include "Constants.h"

namespace effects {
    class ReverbFX : public FX {
    public:
        ReverbFX();
        void prepare(juce::dsp::ProcessSpec& spec) override;
        void update(parameters::FXParameters& parameters) override;
        void process(juce::dsp::ProcessContextReplacing<float>& context) override;
        void reset() override;

    private:
        static constexpr float multiplier = -2.f / REVERB_CHANNELS;

        std::array<float, REVERB_CHANNELS> delayTimes{ 100.f, 133.3f, 166.6f, 200.f };

        Splitter split;
        std::array<Diffuser, DIFF_STEPS> diff;
        std::array<juce::dsp::DelayLine<float>, REVERB_CHANNELS> delays;

        juce::SmoothedValue<float> decay;
        juce::SmoothedValue<float> mix;

        void Householder(std::vector<float>& buffer);
    };
}