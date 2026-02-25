/*
  ==============================================================================

    PreAmp.h
    Created: 23 Feb 2026 11:16:52am
    Author:  Alberto

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Utils.h"
#include "ProcessorBase.h"

namespace preamp {

    namespace waveshapingFunctions {
        static float asymptoticLimit(float x) {
            return (5*x) / (std::abs(5*x) + 1);
        }
    };

    class PreAmp {
    public:
        virtual void prepare(juce::dsp::ProcessSpec& spec) = 0;
        virtual void process(juce::dsp::ProcessContextReplacing<float>& context) = 0;
        virtual void reset() = 0;

    protected:
        juce::SmoothedValue<float> gainSmoother;
    };

    class CleanAmp : public PreAmp {
    public:
        CleanAmp() {

        }

        void prepare(juce::dsp::ProcessSpec& spec) override {

            gainSmoother.reset(spec.sampleRate, 0.002f);
            gainSmoother.setCurrentAndTargetValue(mapValueInRange(0.5f, MIN_DRIVE, MAX_DRIVE));

            waveShaper.reset();
            waveShaper.prepare(spec);
        }

        void process(juce::dsp::ProcessContextReplacing<float>& context) override {
            waveShaper.process(context);
        }

        void reset() override {
            
        }

        void updateParameters(parameters::PreAmpParameters& parameters) {
            gainSmoother.setTargetValue(mapValueInRange(parameters.getGain().get() / 100.f, MIN_DRIVE, MAX_DRIVE));
        }

    private:
        
        static constexpr float MIN_DRIVE = 1.f;
        static constexpr float MAX_DRIVE = 20.f;

        juce::dsp::WaveShaper<float> waveShaper{ { waveshapingFunctions::asymptoticLimit } };
        float bass = 0.0f;
        float middle = 0.0f;
        float treble = 0.0f;
        float master = 0.0f;


    };
};
