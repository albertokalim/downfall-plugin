/*
  ==============================================================================

    Effect.h
    Created: 14 Mar 2026 12:17:08pm
    Author:  Alberto

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Parameters.h"
#include "Utils.h"

namespace effects {
    class FX {
    public:
        virtual ~FX() {}
        virtual void prepare(juce::dsp::ProcessSpec& spec) = 0;
        virtual void update(parameters::Parameters& parameters) = 0;
        virtual void process(juce::dsp::ProcessContextReplacing<float>& context) = 0;
        virtual void reset() = 0;
        bool isBypassed() const { return bypass; }
        void setBypassed(bool _bypass) { bypass = _bypass; }

    protected:
        bool bypass = true;
    };

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
        Tempo tempo; //TODO: Global Tempo, not a member of this class... It has no sense to have this here.
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
        //TODO: Implement a Delay Line with different delay time for right and left
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
};