/*
  ==============================================================================

    Reverb.h
    Created: 21 Mar 2026 1:16:20pm
    Author:  Alberto

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Effect.h"

namespace effects {
    class ReverbFX : public FX {
    public:
        void prepare(juce::dsp::ProcessSpec& spec) override;
        void update(parameters::FXParameters& parameters) override;
        void process(juce::dsp::ProcessContextReplacing<float>& context) override;
        void reset() override;

    private:

    };
}