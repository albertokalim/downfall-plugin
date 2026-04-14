/*
  ==============================================================================

    AmpComponent.h
    Created: 9 Apr 2026 1:38:34pm
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
class AmpComponent  : public juce::Component
{
public:
    AmpComponent(parameters::Parameters& p);
    ~AmpComponent() override;

    void resized() override;

private:
    juce::GroupComponent head;
    RotaryKnob gainKnob;
    RotaryKnob bassKnob;
    RotaryKnob middleKnob;
    RotaryKnob trebleKnob;
    RotaryKnob presenceKnob;
    RotaryKnob masterKnob;
    BypassButton bypassAmp;
    juce::ComboBox ampTypeComboBox;
    juce::ComboBoxParameterAttachment ampTypeParameterAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmpComponent)
};
