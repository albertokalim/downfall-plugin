/*
  ==============================================================================

    EQComponent.cpp
    Created: 9 Apr 2026 1:40:10pm
    Author:  Alberto

  ==============================================================================
*/

#include <JuceHeader.h>
#include "EQComponent.h"

//==============================================================================
EQComponent::EQComponent()
{
    text.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    text.setText("EQ", juce::NotificationType::dontSendNotification);
    text.setJustificationType(juce::Justification::horizontallyCentred);
    text.setBorderSize(juce::BorderSize<int>{ 0, 0, 0, 0 });
    addAndMakeVisible(text);
}

EQComponent::~EQComponent()
{
}

void EQComponent::resized()
{
    text.setBounds(0, 0, 100, 100);
    text.setTopLeftPosition(0, 0);
}
