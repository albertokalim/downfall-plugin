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
#include "Parameters.h"

namespace preamp {

    static constexpr float MIN_DRIVE = 1.f;
    static constexpr float MAX_DRIVE = 20.f;
    static constexpr float MAX_BAND_GAIN = 4.f;
    static constexpr float MIN_BAND_GAIN = 0.001f;
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
        static constexpr float W_SOFT = 3.5f;
        static constexpr float W_HARD = 50.f;

        static float sigmoidFunction(float x, float w) {
            return (x * (abs(x) + w) / (x * x + (w - 1) * abs(x) + 1));
        }

        static float softClipping(float x) {
            return juce::jlimit(-1.f, 1.f, sigmoidFunction(x, W_SOFT));
        }

        static float hardClipping(float x) {
            return juce::jlimit(-1.f, 1.f, sigmoidFunction(x, W_HARD));
        }
    };

    class PreAmpInterface {
    public:
        virtual ~PreAmpInterface() {}
        virtual void prepare(juce::dsp::ProcessSpec& spec) = 0;
        virtual void updateState(parameters::Parameters& parameters) = 0;
        virtual void manageInput(juce::dsp::ProcessContextReplacing<float>& context) = 0;
        virtual void prefilter(juce::dsp::ProcessContextReplacing<float>& context) = 0;
        virtual void waveshaping(juce::dsp::ProcessContextReplacing<float>& context) = 0;
        virtual void postfilter(juce::dsp::ProcessContextReplacing<float>& context) = 0;
        virtual void eq(juce::dsp::ProcessContextReplacing<float>& context) = 0;
        virtual void manageOutput(juce::dsp::ProcessContextReplacing<float>& context) = 0;
        virtual void reset() = 0;
    };

    class PreAmpDecorator {
    public:
        PreAmpDecorator(PreAmpInterface* _decorator) : decorator(_decorator) {}
        ~PreAmpDecorator() {}

        void prepare(juce::dsp::ProcessSpec& spec) { decorator->prepare(spec); }
        void update(parameters::Parameters& parameters) { decorator->updateState(parameters); }
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

        void setDecorator(PreAmpInterface* _decorator) {
            jassert(_decorator != nullptr);
            decorator = _decorator; 
        }

    private:
        PreAmpInterface* decorator;
    };
};
