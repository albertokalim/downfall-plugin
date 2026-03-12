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
#include "Parameters.h"

namespace preamp {

    static constexpr float MIN_DRIVE = 1.f;
    static constexpr float MAX_DRIVE = 20.f;
    static constexpr float MAX_BAND_GAIN = 6.f;
    static constexpr float MIN_BAND_GAIN = -6.f;
    static constexpr float HPF_CENTER_FQ = 20.0f;
    static constexpr float HPF_Q_FACTOR = 0.9f;
    static constexpr float BASS_CENTER_FQ = 100.f;
    static constexpr float BASS_Q_FACTOR = 0.6f;
    static constexpr float MID_CENTER_FQ = 500.f;
    static constexpr float MID_Q_FACTOR = 0.9f;
    static constexpr float TREBLE_CENTER_FQ = 5000.f;
    static constexpr float TREBLE_Q_FACTOR = 0.6f; 

    using IIRFilter = juce::dsp::IIR::Filter<float>;
    using IIRCoefs = juce::dsp::IIR::Coefficients<float>;

    namespace waveshapingFunctions {
        static float asymptoticLimit(float x) {
            return x / (std::abs(x) + 1.f);
        }

        static float tanh(float x) {
            return std::tanh(x);
        }
    };

    class PreAmpInterface {
    public:
        virtual ~PreAmpInterface() {}
        virtual void prepare(juce::dsp::ProcessSpec& spec) = 0;
        virtual void updateState(parameters::PreAmpParameters& parameters) = 0;
        virtual void manageInput(juce::dsp::ProcessContextReplacing<float>& context) = 0;
        virtual void prefilter(juce::dsp::ProcessContextReplacing<float>& context) = 0;
        virtual void waveshaping(juce::dsp::ProcessContextReplacing<float>& context) = 0;
        virtual void postfilter(juce::dsp::ProcessContextReplacing<float>& context) = 0;
        virtual void eq(juce::dsp::ProcessContextReplacing<float>& context) = 0;
        virtual void manageOutput(juce::dsp::ProcessContextReplacing<float>& context) = 0;
        virtual void reset() = 0;
    };

    class PreAmpDecorator : public PreAmpInterface {
    public:
        PreAmpDecorator(PreAmpInterface* _preAmp) : preAmp(_preAmp) {}
        ~PreAmpDecorator() { }

        void prepare(juce::dsp::ProcessSpec& spec) override { preAmp->prepare(spec); }
        void updateState(parameters::PreAmpParameters& parameters) override { preAmp->updateState(parameters); }
        void manageInput(juce::dsp::ProcessContextReplacing<float>& context) override { preAmp->manageInput(context); }
        void prefilter(juce::dsp::ProcessContextReplacing<float>& context) override { preAmp->prefilter(context); }
        void waveshaping(juce::dsp::ProcessContextReplacing<float>& context) override { preAmp->waveshaping(context); }
        void postfilter(juce::dsp::ProcessContextReplacing<float>& context) override { preAmp->postfilter(context); }
        void eq(juce::dsp::ProcessContextReplacing<float>& context) override { preAmp->eq(context); }
        void manageOutput(juce::dsp::ProcessContextReplacing<float>& context) override { preAmp->manageOutput(context); }
        void reset() override { preAmp->reset(); }

    protected:
        std::unique_ptr<PreAmpInterface> preAmp;
    };

    class PreAmp {
    public:
        PreAmp(PreAmpDecorator* _decorator) : decorator(_decorator) {}
        ~PreAmp() { }

        void prepare(juce::dsp::ProcessSpec& spec) { decorator->prepare(spec); }
        void update(parameters::PreAmpParameters& parameters) { decorator->updateState(parameters); }
        void process(juce::dsp::ProcessContextReplacing<float>& context) {
            decorator->manageInput(context);
            decorator->prefilter(context);
            decorator->waveshaping(context);
            decorator->postfilter(context);
            decorator->eq(context);
            decorator->manageOutput(context);
        }
        void reset() { 
            jassert(decorator != nullptr);
            decorator->reset(); 
        }

    private:
        std::unique_ptr<PreAmpDecorator> decorator;
    };

    class CleanAmp : public PreAmpInterface {
    public:
        CleanAmp() {
            minDrive = MIN_DRIVE;
            maxDrive = MAX_DRIVE;
        }

        CleanAmp(float _minDrive, float _maxDrive){
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

        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> lowMidBoost;
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> midBoost;
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> pickAccentBoost;
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> highShelf;
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> lowEndBoost;
        std::unique_ptr<juce::dsp::Oversampling<float>> oversample;
        juce::dsp::Gain<float> gain;
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> highPassFilter;
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> lowPassFilter;
        juce::dsp::WaveShaper<float> waveshaper{ { waveshapingFunctions::asymptoticLimit } };
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> bassEQ;
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> middleEQ;
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> trebleEQ;
        juce::dsp::Gain<float> master;
    };
};
