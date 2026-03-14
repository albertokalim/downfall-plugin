/*
  ==============================================================================

    HighGain.h
    Created: 12 Mar 2026 7:31:26pm
    Author:  Alberto

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PreAmp.h"

namespace preamp {
    class HighGainAmp : public PreAmpInterface {
    public:
        HighGainAmp() {
            minDrive = MIN_DRIVE;
            maxDrive = MAX_DRIVE;
        }

        HighGainAmp(float _minDrive, float _maxDrive) {
            minDrive = _minDrive;
            maxDrive = _maxDrive;
        }
        void prepare(juce::dsp::ProcessSpec& spec) override;
        void updateState(parameters::PreAmpParameters& parameters) override;
        void manageInput(juce::dsp::ProcessContextReplacing<float>& context) override;
        void prefilter(juce::dsp::ProcessContextReplacing<float>& context) override;
        void waveshaping(juce::dsp::ProcessContextReplacing<float>& context) override;
        void postfilter(juce::dsp::ProcessContextReplacing<float>& context) override;
        void eq(juce::dsp::ProcessContextReplacing<float>& context) override;
        void manageOutput(juce::dsp::ProcessContextReplacing<float>& context) override;
        void reset() override;

    private:
        float minDrive = MIN_DRIVE;
        float maxDrive = MAX_DRIVE;
        double sampleRate = 44100;

        juce::SmoothedValue<float> bassSmoother;
        juce::SmoothedValue<float> middleSmoother;
        juce::SmoothedValue<float> trebleSmoother;
        juce::SmoothedValue<float> presenceSmoother;

        std::unique_ptr<juce::dsp::Oversampling<float>> oversample;
        juce::dsp::Gain<float> gain;
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> lowMidBoost;
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> lowEndControl;
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> midBoost;
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> midBoost2;
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> pickAccentBoost;
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> highShelf;
        juce::dsp::WaveShaper<float> waveshaper{ { waveshapingFunctions::tanhHighGain } };
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> hpfPostWaveshaper;
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> highShelfPostWaveshaper;
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> pickAccentPostWaveshaper;
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> lowEndControlPostWaveshaper;
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> bassEQ;
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> middleEQ;
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> trebleEQ;
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> presence;
        juce::dsp::Gain<float> master;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HighGainAmp)
    };
}
