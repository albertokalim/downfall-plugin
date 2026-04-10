/*
  ==============================================================================

    AmpComponent.cpp
    Created: 9 Apr 2026 1:38:34pm
    Author:  Alberto

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AmpComponent.h"

//==============================================================================
AmpComponent::AmpComponent()
{
    text.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    text.setText("Amp", juce::NotificationType::dontSendNotification);
    text.setJustificationType(juce::Justification::horizontallyCentred);
    text.setBorderSize(juce::BorderSize<int>{ 0, 0, 0, 0 });
    addAndMakeVisible(text);
}

AmpComponent::~AmpComponent()
{
    
}

void AmpComponent::resized()
{
    text.setBounds(0, 0, 100, 100);
    text.setTopLeftPosition(0, 0);
}
