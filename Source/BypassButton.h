/*
  ==============================================================================

    BypassButton.h
    Created: 10 Apr 2026 12:28:01pm
    Author:  Alberto

  ==============================================================================
*/
#pragma once
#include <JuceHeader.h>

class BypassButtonLookAndFeel : public juce::LookAndFeel_V4
{
public:
    static BypassButtonLookAndFeel* get()
    {
        static BypassButtonLookAndFeel instance;
        return &instance;
    }

    void drawToggleButton(juce::Graphics&, juce::ToggleButton&, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

private:
    BypassButtonLookAndFeel();
    juce::DropShadow dropShadow;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BypassButtonLookAndFeel)
};

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