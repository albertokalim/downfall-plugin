/*
  ==============================================================================

    CleanAmp.h
    Created: 28 Apr 2026 11:35:17am
    Author:  Alberto

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PreAmp.h"
#include "Constants.h"

namespace preamp {
    class CleanAmp : public PreAmpInterface {
    public:
        CleanAmp() {
            minDrive = MIN_DRIVE;
            maxDrive = MAX_DRIVE;
        }

        CleanAmp(float _minDrive, float _maxDrive) {
            minDrive = _minDrive;
            maxDrive = _maxDrive;
        }
        void prepare(juce::dsp::ProcessSpec& spec) override;
        void updateState(parameters::Parameters& parameters) override;
        void manageInput(juce::dsp::ProcessContextReplacing<float>& context) override;
        void prefilter(juce::dsp::ProcessContextReplacing<float>& context) override;
        void waveshaping(juce::dsp::ProcessContextReplacing<float>& context) override;
        void postfilter(juce::dsp::ProcessContextReplacing<float>& context) override;
        void eq(juce::dsp::ProcessContextReplacing<float>& context) override;
        void manageOutput(juce::dsp::ProcessContextReplacing<float>& context) override;
        void reset() override;

        bool bypassPreFilters = false;
        bool bypassPostFilters = false;

    private:
        float minDrive = MIN_DRIVE;
        float maxDrive = MAX_DRIVE;
        double sampleRate = 44100;

        juce::SmoothedValue<float> bassSmoother;
        juce::SmoothedValue<float> middleSmoother;
        juce::SmoothedValue<float> trebleSmoother;

        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> lowMidBoost;
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> treble3kBoost;
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> pickStrokeAccentBoost;
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> highShelf;
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> lowEndBoost;
        std::unique_ptr<juce::dsp::Oversampling<float>> oversample;
        juce::dsp::Gain<float> gain;
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> postLowEndBoost;
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> postMidBoost;
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> lowPassFilter;
        juce::dsp::WaveShaper<float> waveshaper{ { waveshapingFunctions::softClipping } };
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> bassEQ;
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> middleEQ;
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> trebleEQ;
        juce::dsp::Gain<float> master;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CleanAmp)
    };
}