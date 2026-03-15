/*
  ==============================================================================

    Effect.h
    Created: 14 Mar 2026 12:17:08pm
    Author:  Alberto

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Parameters.h"
namespace effects {
    class FX {
    public:
        virtual void prepare(juce::dsp::ProcessSpec& spec) = 0;
        virtual void update(parameters::FXParameters& parameters) = 0;
        virtual void process(juce::dsp::ProcessContextReplacing<float>& context) = 0;
        bool isBypassed() const { return bypass; }
        void setBypassed(bool _bypass) { bypass = _bypass; }

    private:
        bool bypass = true;
    };

    class DelayFX : public FX {

    };
}
