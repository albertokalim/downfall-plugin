/*
  ==============================================================================

    Delay.h
    Created: 30 Apr 2026 6:02:27pm
    Author:  Alberto

  ==============================================================================
*/

#pragma once
#include "Effect.h"

namespace effects {
    class Tempo {
    public:

        void reset() noexcept;

        void update(const juce::AudioPlayHead* playhead) noexcept;

        double getMillisecondsForNoteLength(int index) const noexcept;

        double getTempo() const noexcept
        {
            return bpm;
        }

    private:
        double bpm = 120.0;
    };

    class DelayFX : public FX {
    public:
        ~DelayFX() {}
        void prepare(juce::dsp::ProcessSpec& spec) override;
        void update(parameters::Parameters& parameters) override;
        void process(juce::dsp::ProcessContextReplacing<float>& context) override;
        void reset() override;
        void updateTempoPlayHead(juce::AudioPlayHead* playhead) { tempo.update(playhead); }

    private:
        Tempo tempo;
        juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> delayLine;
        juce::SmoothedValue<float> delayTimeSmoother;
        juce::SmoothedValue<float> mixSmoother;
        juce::SmoothedValue<float> feedbackSmoother;

        bool sync = false;
        int delayNoteIndex = 0;
        float sampleRate = 44100.f;
        float feedbackL = 0.0f;
        float feedbackR = 0.0f;
    };
}