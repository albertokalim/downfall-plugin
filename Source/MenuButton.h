/*
  ==============================================================================

    MenuButton.h
    Created: 9 Apr 2026 2:29:13am
    Author:  Alberto

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class MenuButtonLookAndFeel : public juce::LookAndFeel_V4
{
public:
    MenuButtonLookAndFeel();

    static MenuButtonLookAndFeel* get()
    {
        static MenuButtonLookAndFeel instance;
        return &instance;
    }

    void drawButtonBackground(juce::Graphics&, juce::Button&, const juce::Colour& backgroundColour,
        bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
    void drawButtonText(juce::Graphics& g, juce::TextButton& button, bool isMouseOverButton, bool isButtonDown) override;

private:
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MenuButtonLookAndFeel)
};