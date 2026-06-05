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
        void update(parameters::Parameters& parameters) override;
        void process(juce::dsp::ProcessContextReplacing<float>& context) override;
        void reset() override;

    private:
        static constexpr float multiplier = -2.f / REVERB_CHANNELS;
        static constexpr float base_diff_delay = 20.f;
        static constexpr float cutOff_fq = 8000.f;
        static constexpr float q_high_cut = 0.3f;
        static constexpr float gain_reduction_linear = 0.9f;


        float delayTime = 0.f;
        float sampleRate = DEFAULT_SAMPLE_RATE;
        
        std::array<Diffuser, DIFF_STEPS> diff{ Diffuser{base_diff_delay},Diffuser{base_diff_delay * 2},Diffuser{base_diff_delay * 4},Diffuser{base_diff_delay * 8}};
        std::array<juce::dsp::DelayLine<float>, REVERB_CHANNELS> delays;
        IIRFilter highShelfCut{ juce::dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate, cutOff_fq, q_high_cut, gain_reduction_linear) };
        std::array<float, REVERB_CHANNELS> delayedSamples;
        float* output[REVERB_CHANNELS];

        juce::SmoothedValue<float> decay;
        juce::SmoothedValue<float> mix;
        Splitter split{ REVERB_CHANNELS };

        void Householder(std::array<float, REVERB_CHANNELS>& inputs);
    };
}