/*
  ==============================================================================

    CabinetComponent.cpp
    Created: 9 Apr 2026 1:39:50pm
    Author:  Alberto

  ==============================================================================
*/

#include <JuceHeader.h>
#include "CabinetComponent.h"

//==============================================================================
CabinetComponent::CabinetComponent()
{
    text.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    text.setText("Cabinet", juce::NotificationType::dontSendNotification);
    text.setJustificationType(juce::Justification::horizontallyCentred);
    text.setBorderSize(juce::BorderSize<int>{ 0, 0, 0, 0 });
    addAndMakeVisible(text);
}

CabinetComponent::~CabinetComponent()
{
}

void CabinetComponent::resized()
{
    text.setBounds(0, 0, 100, 100);
    text.setTopLeftPosition(0, 0);
}
