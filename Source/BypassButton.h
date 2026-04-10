/*
  ==============================================================================

    BypassButton.h
    Created: 10 Apr 2026 12:28:01pm
    Author:  Alberto

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
class BypassButton : public juce::Component 
{
public:
    BypassButton(const juce::String& text, juce::AudioParameterBool& p);
    ~BypassButton() override;

    void resized() override;

    juce::ToggleButton button;

private:
    juce::ButtonParameterAttachment listener;
};