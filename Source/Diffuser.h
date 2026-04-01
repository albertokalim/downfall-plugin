/*
  ==============================================================================

    Difusser.h
    Created: 25 Mar 2026 10:07:38am
    Author:  Alberto

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Splitter.h"

using IIRFilter = juce::dsp::IIR::Filter<float>;
using IIRCoefs = juce::dsp::IIR::Coefficients<float>;
namespace effects {
    class Diffuser {
    public:
        Diffuser(float);
        ~Diffuser();
        void prepare(juce::dsp::ProcessSpec& spec);
        void process(Splitter& split);
        void reset();

    private:
        float delayTime = 0.0f;
        std::array<juce::dsp::DelayLine<float>, REVERB_CHANNELS> delays;
        std::array<bool, REVERB_CHANNELS> flipPolarity;
        float* frame;
        float* output[REVERB_CHANNELS];
        const float scalingFactor = std::sqrt(1.f / (float)REVERB_CHANNELS);

        void Hadamard(float* input);
        void HadamardRecursive(float* input, int size);
    };
}