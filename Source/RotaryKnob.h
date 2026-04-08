/*
  ==============================================================================

    RotaryKnob.h
    Created: 7 Apr 2026 7:46:35pm
    Author:  Alberto

  ==============================================================================
*/

#pragma once
#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class RotaryKnob : public juce::Component
{
public:
    RotaryKnob(const juce::String& text, bool drawFromMiddle = false);
    ~RotaryKnob() override;

    void resized() override;

    juce::Slider slider;
    juce::Label label;

private:


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RotaryKnob)
};