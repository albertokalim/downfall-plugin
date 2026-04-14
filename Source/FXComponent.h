/*
  ==============================================================================

    FXComponent.h
    Created: 9 Apr 2026 1:39:13pm
    Author:  Alberto

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "RotaryKnob.h"
#include "BypassButton.h"
#include "Parameters.h"

//==============================================================================
/*
*/
class FXComponent  : public juce::Component
{
public:
    FXComponent(parameters::Parameters& p);
    ~FXComponent() override;

    void resized() override;

private:
    juce::GroupComponent delayGroup;
    RotaryKnob delayTimeKnob;
    RotaryKnob feedbackKnob;
    RotaryKnob mixKnob;
    RotaryKnob delayNoteKnob;
    BypassButton delayBypass;
    juce::ToggleButton syncButton{ "Sync" };
    juce::ButtonParameterAttachment syncAttachment;

    juce::GroupComponent chorusGroup;
    RotaryKnob chorusRateKnob;
    RotaryKnob chorusWidthKnob;
    RotaryKnob chorusMixKnob;
    BypassButton chorusBypass;

    juce::GroupComponent reverbGroup;
    RotaryKnob reverbDecayKnob;
    RotaryKnob reverbMixKnob;
    BypassButton reverbBypass;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FXComponent)
};
