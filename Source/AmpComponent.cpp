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
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    addAndMakeVisible(text);
}

AmpComponent::~AmpComponent()
{
}

void AmpComponent::paint (juce::Graphics& g)
{
    
}

void AmpComponent::resized()
{
    text.setTopLeftPosition(this->getWidth() / 2 - text.getWidth() / 2, this->getHeight() / 2 - text.getHeight() / 2);

}
