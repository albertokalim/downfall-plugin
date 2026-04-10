/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "MenuButton.h"


//==============================================================================
DownfallPluginAudioProcessorEditor::DownfallPluginAudioProcessorEditor (DownfallPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), 
    audioProcessor (p), 
    inputLevelMeter(p.inputLevelL, p.inputLevelR), 
    outputLevelMeter(p.outputLevelL, p.outputLevelR)
{
    ampButton.setToggleable(true);
    fxButton.setToggleable(true);
    cabinetButton.setToggleable(true);
    eqButton.setToggleable(true);

    ampButton.setClickingTogglesState(true);
    fxButton.setClickingTogglesState(true);
    cabinetButton.setClickingTogglesState(true);
    eqButton.setClickingTogglesState(true);

    ampButton.setLookAndFeel(MenuButtonLookAndFeel::get());
    fxButton.setLookAndFeel(MenuButtonLookAndFeel::get());
    cabinetButton.setLookAndFeel(MenuButtonLookAndFeel::get());
    eqButton.setLookAndFeel(MenuButtonLookAndFeel::get());

    ampButton.onClick = [this]() {
        if (this->fxButton.getToggleState()) {
            this->fxButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        }
        if (this->cabinetButton.getToggleState()) {
            this->cabinetButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        }
        if (this->eqButton.getToggleState()) {
            this->eqButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        }
        middle.removeAllChildren();
        middle.addAndMakeVisible(ampComponent);
    };

    fxButton.onClick = [this]() {
        if (this->ampButton.getToggleState()) {
            this->ampButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        }
        if (this->cabinetButton.getToggleState()) {
            this->cabinetButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        }
        if (this->eqButton.getToggleState()) {
            this->eqButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        }

        middle.removeAllChildren();
        middle.addAndMakeVisible(fxComponent);
    };

    cabinetButton.onClick = [this]() {
        if (this->ampButton.getToggleState()) {
            this->ampButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        }
        if (this->fxButton.getToggleState()) {
            this->fxButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        }
        if (this->eqButton.getToggleState()) {
            this->eqButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        }
        middle.removeAllChildren();
        middle.addAndMakeVisible(cabinetComponent);
    };

    eqButton.onClick = [this]() {
        if (this->ampButton.getToggleState()) {
            this->ampButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        }
        if (this->fxButton.getToggleState()) {
            this->fxButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        }
        if (this->cabinetButton.getToggleState()) {
            this->cabinetButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        }
        middle.removeAllChildren();
        middle.addAndMakeVisible(eqComponent);
    };

    menu.setColour(juce::GroupComponent::outlineColourId, juce::Colours::black);
    menu.addAndMakeVisible(ampButton);
    menu.addAndMakeVisible(fxButton);
    menu.addAndMakeVisible(cabinetButton);
    menu.addAndMakeVisible(eqButton);

    top.setColour(juce::GroupComponent::outlineColourId, juce::Colours::black);
    top.addAndMakeVisible(inputLevelMeter);
    top.addAndMakeVisible(inputKnob);
    top.addAndMakeVisible(menu);
    top.addAndMakeVisible(bypassButton);
    top.addAndMakeVisible(outputKnob);
    top.addAndMakeVisible(outputLevelMeter);

    ampButton.setToggleState(true, juce::NotificationType::dontSendNotification);
    middle.addAndMakeVisible(ampComponent);
    addAndMakeVisible(top);
    addAndMakeVisible(middle);
    setSize (1356, 706);
}

DownfallPluginAudioProcessorEditor::~DownfallPluginAudioProcessorEditor()
{
}

//==============================================================================
void DownfallPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::black);
}

void DownfallPluginAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    int heightTop = 2 * bounds.getHeight() / 10;
    int menuWidth = bounds.getWidth() / 3;
    int menuHeight = 50;

    top.setBounds(0, 0, bounds.getWidth(), heightTop);
    menu.setBounds(bounds.getWidth() / 2 - menuWidth / 2, heightTop - menuHeight - 4, menuWidth, menuHeight);
    middle.setBounds(0, heightTop, bounds.getWidth(), bounds.getHeight() - heightTop);

    int knobPaddingFromBorder = 80;
    int knobYCoord = (heightTop / 2) - (inputKnob.getHeight() / 2);
    int heightLevelMeter = heightTop - 30;
    int widthLevelMeter = 15;
    int inputLevelX = knobPaddingFromBorder / 2 - widthLevelMeter / 2;
    int inputLevelY = heightTop / 2 - heightLevelMeter / 2;
    int outputLevelX = bounds.getWidth() - inputLevelX - widthLevelMeter;
    int outputLevelY = inputLevelY;

    inputLevelMeter.setBounds(0, 0, widthLevelMeter, heightLevelMeter);
    inputLevelMeter.setTopLeftPosition(inputLevelX, inputLevelY);
    inputKnob.setTopLeftPosition(knobPaddingFromBorder, knobYCoord);
    bypassButton.setBounds(bounds.getWidth() / 2 - menuWidth / 2 - 200, heightTop / 2 - 20, 100, 40);
    outputKnob.setTopLeftPosition(bounds.getWidth() - knobPaddingFromBorder - outputKnob.getWidth(), knobYCoord);
    outputLevelMeter.setBounds(0, 0, widthLevelMeter, heightLevelMeter);
    outputLevelMeter.setTopLeftPosition(outputLevelX, outputLevelY);

    int buttonWidth = menu.getWidth() / 4;
    int buttonHeight = menu.getHeight();

    ampButton.setBounds(0, 0, buttonWidth, buttonHeight);
    ampButton.setTopLeftPosition(0, 0);
    fxButton.setBounds(0, 0, buttonWidth, buttonHeight);
    fxButton.setTopLeftPosition(menu.getWidth() / 4, 0);
    cabinetButton.setBounds(0, 0, buttonWidth, buttonHeight);
    cabinetButton.setTopLeftPosition(2 * menu.getWidth() / 4, 0);
    eqButton.setBounds(0, 0, buttonWidth, buttonHeight);
    eqButton.setTopLeftPosition(3* menu.getWidth() / 4, 0);

    ampComponent.setBounds(0, 0, middle.getWidth(), middle.getHeight());
    ampComponent.setTopLeftPosition(0, 0);
    fxComponent.setBounds(0, 0, middle.getWidth(), middle.getHeight());
    fxComponent.setTopLeftPosition(0, 0);
    cabinetComponent.setBounds(0, 0, middle.getWidth(), middle.getHeight());
    cabinetComponent.setTopLeftPosition(0, 0);
    eqComponent.setBounds(0, 0, middle.getWidth(), middle.getHeight());
    eqComponent.setTopLeftPosition(0, 0);
}
