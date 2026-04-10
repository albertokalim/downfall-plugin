/*
  ==============================================================================

    BypassButton.cpp
    Created: 10 Apr 2026 12:28:29pm
    Author:  Alberto

  ==============================================================================
*/

#include "BypassButton.h"

BypassButton::BypassButton(const juce::String& text, juce::AudioParameterBool& p)
    : button(text), listener(p, button)
{
    addAndMakeVisible(button);
}

BypassButton::~BypassButton()
{
}

void BypassButton::resized()
{
    button.setBounds(0, 0, 100, 40);
}
