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
AmpComponent::AmpComponent(parameters::Parameters& p)
    : gainKnob("Gain", p.gain),
    bassKnob("Bass", p.bass),
    middleKnob("Middle", p.middle),
    trebleKnob("Treble", p.treble),
    presenceKnob("Presence", p.presence),
    masterKnob("Master", p.master),
    bypassAmp("Active", p.bypassPreamp),
    ampTypeComboBox("Amp Type"),
    ampTypeParameterAttachment(p.ampType, ampTypeComboBox)
{
    head.addAndMakeVisible(gainKnob);
    head.addAndMakeVisible(bassKnob);
    head.addAndMakeVisible(middleKnob);
    head.addAndMakeVisible(trebleKnob);
    head.addAndMakeVisible(masterKnob);
    head.addAndMakeVisible(presenceKnob);
    head.addAndMakeVisible(bypassAmp);

    ampTypeComboBox.addItemList(p.ampType.getAllValueStrings(), 1);
    ampTypeComboBox.onChange = [this]() {
        if (this->ampTypeComboBox.getSelectedId() == 2) {
            presenceKnob.setVisible(true);
        }
        else {
            presenceKnob.setVisible(false);
        }
        };
    ampTypeComboBox.setSelectedId(p.ampType.getIndex() + 1);
    

    head.addAndMakeVisible(ampTypeComboBox);
    addAndMakeVisible(head);
}

AmpComponent::~AmpComponent()
{
}

void AmpComponent::resized()
{
    auto bounds = getLocalBounds();
    int widthTenPerc = std::ceil(bounds.getWidth() * 0.1);
    int heightTenPerc = std::ceil(bounds.getHeight() * 0.3);
    int width = bounds.getWidth() - widthTenPerc;
    int height = bounds.getHeight() - heightTenPerc;
    head.setBounds(widthTenPerc/2, heightTenPerc/2, width, height);

    int knobAreaWidth = std::ceil(head.getWidth() * 0.9);
    int knobWidth = knobAreaWidth / 6;
    int bottomHeight = std::ceil(head.getHeight() * 0.2);
    int knobHeight = std::ceil(head.getHeight() * 0.8);
    int knobY = knobHeight / 2 - bottomHeight;
    int offset = std::ceil(head.getWidth() * 0.1);
    
    gainKnob.setTopLeftPosition(offset, knobY);
    bassKnob.setTopLeftPosition(offset + knobWidth, knobY);
    middleKnob.setTopLeftPosition(offset + 2*knobWidth, knobY);
    trebleKnob.setTopLeftPosition(offset + 3*knobWidth, knobY);
    masterKnob.setTopLeftPosition(offset + 4*knobWidth, knobY);
    presenceKnob.setTopLeftPosition(offset + 5*knobWidth, knobY);
    bypassAmp.setBounds(head.getWidth() - offset - offset/2, knobHeight + bottomHeight / 2 - 50, 100, 100);
    ampTypeComboBox.setBounds(offset, knobHeight + bottomHeight / 2 - 50, 200, 30);
}
