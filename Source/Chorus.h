/*
  ==============================================================================

    Chorus.h
    Created: 30 Apr 2026 6:02:37pm
    Author:  Alberto

  ==============================================================================
*/

#pragma once
#include "Effect.h"
#include "Constants.h"
#include "Splitter.h"

namespace effects {
    class ChorusFX : public FX {
    public:
        ~ChorusFX() {}
        void prepare(juce::dsp::ProcessSpec& spec) override;
        void update(parameters::Parameters& parameters) override;
        void process(juce::dsp::ProcessContextReplacing<float>& context) override;
        void reset() override;

    private:
        static const int CHORUS_CHANNELS = 2;
        static constexpr float MAX_CHORUS_DELAY = 20.f;
        static constexpr float MIN_CHORUS_DELAY = 0.f;

        std::array<juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear>, CHORUS_CHANNELS> delayLines;
        std::array<juce::dsp::Oscillator<float>, CHORUS_CHANNELS> lfos{
                juce::dsp::Oscillator<float>{[](auto phase) {
                    return std::sin(phase);
                }
            },
                juce::dsp::Oscillator<float>{[](auto phase) {
                    return std::cos(phase);
                }
            }
        };

        float delayTime = MIN_CHORUS_DELAY;
        float sampleRate = DEFAULT_SAMPLE_RATE;
        juce::SmoothedValue<float> mixSmoother;
        juce::SmoothedValue<float> rateSmoother; //fq in Hz
        juce::SmoothedValue<float> widthSmoother;
        Splitter split{ CHORUS_CHANNELS };
    };
}