/*
  ==============================================================================

    Distortion.h
    Created: 5 Mar 2026 7:57:28pm
    Author:  Alberto

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace preamp {
    namespace waveshapingFunctions {
        static float asymptoticLimit(float x) {
            return x / (std::abs(x) + 1.f);
        }

        static float tanh(float x) {
            return std::tanh(5.f * x);
        }

        static float tanh2(float x) {
            return std::tanh(3.96f * x);
        }

        static float polynomial(float x) {
            float limit = juce::jlimit(-1.f, 1.f, x);
            return (-0.5f * limit) + (3 / 2 * std::pow(limit, 3));
        }
    };

    class Distortion {
    public:
        virtual ~Distortion() = default;
        virtual void applyDistortion(juce::dsp::ProcessContextReplacing<float>& context) = 0;
        virtual void prepare(juce::dsp::ProcessSpec& spec) = 0;
        virtual void reset() = 0;
    };

    class SoftClipDistortion : public Distortion{
    public:
        void applyDistortion(juce::dsp::ProcessContextReplacing<float>& context) override;

        void prepare(juce::dsp::ProcessSpec& spec) override;

        void reset() override;

    };

    class HighGainDistortion : public Distortion {
    public:
        void applyDistortion(juce::dsp::ProcessContextReplacing<float>& context) override;

        void prepare(juce::dsp::ProcessSpec& spec) override;

        void reset() override;

    };
}