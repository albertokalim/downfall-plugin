/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DownfallPluginAudioProcessorEditor::DownfallPluginAudioProcessorEditor (DownfallPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{

    top.addAndMakeVisible(inputKnob);
    top.addAndMakeVisible(outputKnob);
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
    int knobPaddingFromBorder = 80;

    top.setBounds(0, 0, bounds.getWidth(), heightTop);
    middle.setBounds(0, heightTop, bounds.getWidth(), bounds.getHeight() - heightTop);

    int knobYCoord = (heightTop / 2) - (inputKnob.getHeight() / 2);

    inputKnob.setTopLeftPosition(knobPaddingFromBorder, knobYCoord);
    outputKnob.setTopLeftPosition(bounds.getWidth() - knobPaddingFromBorder - outputKnob.getWidth(), knobYCoord);
}
