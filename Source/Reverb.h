/*
  ==============================================================================

    Reverb.h
    Created: 21 Mar 2026 1:16:20pm
    Author:  Alberto

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Splitter.h"
#include "Diffuser.h"
#include "Effect.h"
#include "Constants.h"

namespace effects {
    class ReverbFX : public FX {
    public:
        ReverbFX(float _delayTime);
        void prepare(juce::dsp::ProcessSpec& spec) override;
        void update(parameters::FXParameters& parameters) override;
        void process(juce::dsp::ProcessContextReplacing<float>& context) override;
        void reset() override;

    private:
        static constexpr float multiplier = -2.f / REVERB_CHANNELS;

        float delayTime = 0.f;
        float sampleRate = 44100.f;
        Splitter split;
        std::array<Diffuser, DIFF_STEPS> diff{ Diffuser{20.0f},Diffuser{40.0f},Diffuser{80.0f},Diffuser{160.0f} };
        std::array<juce::dsp::DelayLine<float>, REVERB_CHANNELS> delays;
        IIRFilter highShelfCut{ juce::dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate, 8000.f, 0.3f, 0.9f) };
        std::array<float, REVERB_CHANNELS> delayedSamples;
        float* output[REVERB_CHANNELS];

        juce::SmoothedValue<float> decay;
        juce::SmoothedValue<float> mix;

        void Householder(std::array<float, REVERB_CHANNELS>& inputs);
    };
}