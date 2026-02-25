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

    static constexpr float MIN_DRIVE = 1.f;
    static constexpr float MAX_DRIVE = 20.f;
    static constexpr float MAX_BAND_GAIN = 6.f;
    static constexpr float MIN_BAND_GAIN = -6.f;
    static constexpr float BASS_CENTER_FQ = 100.f;
    static constexpr float BASS_Q_FACTOR = 0.6f;

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

        void updateParameters(parameters::PreAmpParameters& parameters) {

            gain.setGainLinear(mapValueInRange(parameters.getGain().get() / 100.f, MIN_DRIVE, MAX_DRIVE));

            bassSmoother.setTargetValue(mapValueInRange(parameters.getBass().get() / 100.f, MIN_BAND_GAIN, MAX_BAND_GAIN));
            float gainFactor = juce::Decibels::decibelsToGain(bassSmoother.getNextValue());
            *bassEQ.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, BASS_CENTER_FQ, BASS_Q_FACTOR, gainFactor);
        }

    protected:
        using IIRFilter = juce::dsp::IIR::Filter<float>;
        using IIRCoefs = juce::dsp::IIR::Coefficients<float>;

        double sampleRate = 44100;

        juce::dsp::Gain<float> gain;
        juce::SmoothedValue<float> bassSmoother;

        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> bassEQ;
    };

    class CleanAmp : public PreAmp {
    public:
        CleanAmp() {

        }

        void prepare(juce::dsp::ProcessSpec& spec) override {

            sampleRate = spec.sampleRate;

            gain.reset();
            gain.prepare(spec);
            gain.setGainLinear(mapValueInRange(0.5f, MIN_DRIVE, MAX_DRIVE));

            bassSmoother.reset(spec.sampleRate, 0.002f);
            bassSmoother.setCurrentAndTargetValue(mapValueInRange(0.5f, MIN_BAND_GAIN, MAX_BAND_GAIN));

            waveShaper.reset();
            waveShaper.prepare(spec);

            bassEQ.reset();
            *bassEQ.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, BASS_CENTER_FQ, BASS_Q_FACTOR, 0.f);
            bassEQ.prepare(spec);
        }

        void process(juce::dsp::ProcessContextReplacing<float>& context) override {
            gain.process(context);
            waveShaper.process(context);
            bassEQ.process(context);
        }

        void reset() override {
            
        }

    private:
        juce::dsp::WaveShaper<float> waveShaper{ { waveshapingFunctions::asymptoticLimit } };
    };
};
