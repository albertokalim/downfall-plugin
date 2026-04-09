/*
  ==============================================================================

    MenuButton.cpp
    Created: 9 Apr 2026 2:29:31am
    Author:  Alberto

  ==============================================================================
*/

#include "MenuButton.h"

MenuButtonLookAndFeel::MenuButtonLookAndFeel()
{
    setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    setColour(juce::TextButton::textColourOnId, juce::Colours::black);
    setColour(juce::TextButton::textColourOffId, juce::Colours::white);
}

void MenuButtonLookAndFeel::drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
    bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    auto buttonArea = button.getLocalBounds();
    auto edge = 4;
    if (shouldDrawButtonAsDown || button.getToggleState()) {
        g.setColour(juce::Colours::white);
    } 
    else if (shouldDrawButtonAsHighlighted){
        g.setColour(juce::Colours::darkgrey);
    }
    else {
        g.setColour(backgroundColour);
    }
    
    g.fillRect(buttonArea);
}

void MenuButtonLookAndFeel::drawButtonText(juce::Graphics& g, juce::TextButton& button, bool isMouseOverButton, bool isButtonDown)
{
    auto font = getTextButtonFont(button, button.getHeight());
    g.setFont(font);
    g.setColour(button.findColour(button.getToggleState() ? juce::TextButton::textColourOnId
        : juce::TextButton::textColourOffId));
    auto yIndent = juce::jmin(4, button.proportionOfHeight(0.3f));
    auto cornerSize = juce::jmin(button.getHeight(), button.getWidth()) / 2;
    auto fontHeight = juce::roundToInt(font.getHeight() * 0.6f);
    auto leftIndent = juce::jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2));
    auto rightIndent = juce::jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));
    auto textWidth = button.getWidth() - leftIndent - rightIndent;
    if (textWidth > 0)
        g.drawFittedText(button.getButtonText(),
            leftIndent,
            yIndent,
            textWidth,
            button.getHeight() - yIndent * 2,
            juce::Justification::centred,
            2);
}
