/*
  ==============================================================================

    PreAmp.h
    Created: 23 Feb 2026 11:16:52am
    Author:  Alberto

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace preamp {

    namespace waveshapingFunctions {
        static float asymptoticLimit(float x) {
            return (5*x) / (std::abs(5*x) + 1);
        }
    };

    class PreAmp {
    public:
        virtual void prepare(const juce::dsp::ProcessSpec&) = 0;
        virtual void process(const juce::dsp::ProcessContextReplacing<float>&) = 0;
        virtual void reset() = 0;
    };

    class CleanAmp : public PreAmp {
    public:
        CleanAmp() {

        }
        void prepare(const juce::dsp::ProcessSpec& spec) override {
            waveShaper.prepare(spec);
        }
        void process(const juce::dsp::ProcessContextReplacing<float>& context) override {
            waveShaper.process(context);
        }
        void reset() override {
            waveShaper.reset();
        }

    private:
        juce::dsp::WaveShaper<float> waveShaper{ { waveshapingFunctions::asymptoticLimit } };
        float gain = 0.0f;
        float bass = 0.0f;
        float middle = 0.0f;
        float treble = 0.0f;
        float master = 0.0f;
    };
};
