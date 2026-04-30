/*
  ==============================================================================

    Chorus.h
    Created: 30 Apr 2026 6:02:37pm
    Author:  Alberto

  ==============================================================================
*/

#pragma once
#include "Effect.h"

namespace effects {
    class ChorusFX : public FX {
    public:
        ~ChorusFX() {}
        void prepare(juce::dsp::ProcessSpec& spec) override;
        void update(parameters::Parameters& parameters) override;
        void process(juce::dsp::ProcessContextReplacing<float>& context) override;
        void reset() override;

    private:
        static constexpr float MAX_CHORUS_DELAY = 35.f;
        static constexpr float MIN_CHORUS_DELAY = 15.f;

        juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> delayLine;
        juce::dsp::Oscillator<float> lfo{
            [](auto phase) {
                return std::sin(phase);
            }
        };

        float delayTime = MIN_CHORUS_DELAY;
        float sampleRate = 44100.f;
        juce::SmoothedValue<float> mixSmoother;
        juce::SmoothedValue<float> rateSmoother; //fq in Hz
        juce::SmoothedValue<float> widthSmoother;
    };
}