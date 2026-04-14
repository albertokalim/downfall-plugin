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
FXComponent::FXComponent(parameters::Parameters& p)
    : delayTimeKnob("Time", p.delayTime),
    feedbackKnob("Feedback", p.feedback),
    mixKnob("Mix", p.delayMix),
    delayNoteKnob("Note", p.delayNote),
    delayBypass("Bypass", p.delayBypass),
    syncAttachment(p.sync, syncButton),
    chorusRateKnob("Rate", p.rate),
    chorusWidthKnob("Width", p.width),
    chorusMixKnob("Mix", p.chorusMix),
    chorusBypass("Bypass", p.chorusBypass),
    reverbDecayKnob("Decay", p.decay),
    reverbMixKnob("Mix", p.reverbMix),
    reverbBypass("Bypass", p.reverbBypass)
{
    
    delayGroup.setText("Delay");
    chorusGroup.setText("Chorus");
    reverbGroup.setText("Reverb");

    delayGroup.addAndMakeVisible(delayTimeKnob);
    delayGroup.addAndMakeVisible(feedbackKnob);
    delayGroup.addAndMakeVisible(mixKnob);
    delayGroup.addAndMakeVisible(delayNoteKnob);
    delayGroup.addAndMakeVisible(delayBypass);
    delayGroup.addAndMakeVisible(syncButton);

    chorusGroup.addAndMakeVisible(chorusRateKnob);
    chorusGroup.addAndMakeVisible(chorusWidthKnob);
    chorusGroup.addAndMakeVisible(chorusMixKnob);
    chorusGroup.addAndMakeVisible(chorusBypass);

    reverbGroup.addAndMakeVisible(reverbDecayKnob);
    reverbGroup.addAndMakeVisible(reverbMixKnob);
    reverbGroup.addAndMakeVisible(reverbBypass);

    delayNoteKnob.setVisible(false);
    syncButton.onClick = [this]() {
            bool isSyncActive = syncButton.getToggleStateValue().getValue();
            delayNoteKnob.setVisible(isSyncActive);
            delayTimeKnob.setVisible(!isSyncActive);
        };

    addAndMakeVisible(delayGroup);
    addAndMakeVisible(chorusGroup);
    addAndMakeVisible(reverbGroup);
}

FXComponent::~FXComponent()
{
}

void FXComponent::resized()
{
    auto bounds = getLocalBounds();
    int offSetX = std::ceil((bounds.getWidth() * 0.05) / 2);
    int offSetY = std::ceil((bounds.getHeight() * 0.1) / 2);
    int fxZoneWidth = std::ceil(bounds.getWidth() * 0.95);
    int fxZoneHeight = std::ceil(bounds.getHeight() * 0.90);
    int fxGroupWidth = fxZoneWidth / 3;

    delayGroup.setBounds(offSetX, offSetY, fxGroupWidth, fxZoneHeight);
    chorusGroup.setBounds(offSetX + fxGroupWidth, offSetY, fxGroupWidth, fxZoneHeight);
    reverbGroup.setBounds(offSetX + 2*fxGroupWidth, offSetY, fxGroupWidth, fxZoneHeight);

    int column = fxGroupWidth / 2;
    int row = fxZoneHeight / 3;
    int padding = 75;

    delayTimeKnob.setTopLeftPosition(padding, padding);
    delayNoteKnob.setTopLeftPosition(padding, padding);
    feedbackKnob.setTopLeftPosition(column + padding, padding);
    mixKnob.setTopLeftPosition(padding, row+ padding);
    delayBypass.setBounds(0, 0, 100, 100);
    delayBypass.setTopLeftPosition(padding, 2 * row + padding);
    syncButton.setBounds(0, 0, 100, 100);
    syncButton.setTopLeftPosition(column + padding, row + padding);

    chorusRateKnob.setTopLeftPosition(padding, padding);
    chorusWidthKnob.setTopLeftPosition(column + padding, padding);
    chorusMixKnob.setTopLeftPosition(padding, row + padding);
    chorusBypass.setBounds(0, 0, 100, 100);
    chorusBypass.setTopLeftPosition(padding, 2 * row + padding);

    reverbDecayKnob.setTopLeftPosition(padding, padding);
    reverbMixKnob.setTopLeftPosition(column + padding, padding);
    reverbBypass.setBounds(0, 0, 100, 100);
    reverbBypass.setTopLeftPosition(padding, 2 * row + padding);
}
