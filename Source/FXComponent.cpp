/*
  ==============================================================================

    FXComponent.cpp
    Created: 9 Apr 2026 1:39:13pm
    Author:  Alberto

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FXComponent.h"

//==============================================================================
FXComponent::FXComponent()
{
    text.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    text.setText("FX", juce::NotificationType::dontSendNotification);
    text.setJustificationType(juce::Justification::horizontallyCentred);
    text.setBorderSize(juce::BorderSize<int>{ 0, 0, 0, 0 });
    addAndMakeVisible(text);
}

FXComponent::~FXComponent()
{
}

void FXComponent::resized()
{
    text.setBounds(0, 0, 100, 100);
    text.setTopLeftPosition(0, 0);
}
