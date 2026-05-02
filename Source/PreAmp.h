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
#include "Processor.h"

namespace preamp {

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

    class PreAmpDecorator : public ProcessorBase{
    public:
        PreAmpDecorator(PreAmpInterface* _decorator) : decorator(_decorator) {}
        ~PreAmpDecorator() {}

        void prepare(juce::dsp::ProcessSpec& spec) override { decorator->prepare(spec); }
        void update(parameters::Parameters& parameters) override { decorator->updateState(parameters); }
        void process(juce::dsp::ProcessContextReplacing<float>& context) override {
            decorator->manageInput(context);
            decorator->prefilter(context);
            decorator->waveshaping(context);
            decorator->postfilter(context);
            decorator->eq(context);
            decorator->manageOutput(context);
        }
        void reset() override { 
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
