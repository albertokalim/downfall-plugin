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
#include "Utils.h"
#include "Processor.h"

namespace effects {
    class FX : public ProcessorBase {
    public:
        virtual ~FX() {}
        virtual void prepare(juce::dsp::ProcessSpec& spec) override = 0;
        virtual void update(parameters::Parameters& parameters) override = 0;
        virtual void process(juce::dsp::ProcessContextReplacing<float>& context) override = 0;
        virtual void reset() override = 0;
        bool isBypassed() const noexcept { return bypass; }
        void setBypassed(bool _bypass) noexcept { bypass = _bypass; }

    protected:
        bool bypass = true;
    };
};