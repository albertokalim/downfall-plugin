/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DownfallPluginAudioProcessorEditor::DownfallPluginAudioProcessorEditor (DownfallPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), 
    audioProcessor (p), 
    inputLevelMeter(p.inputLevelL, p.inputLevelR), 
    outputLevelMeter(p.outputLevelL, p.outputLevelR)
{
    top.addAndMakeVisible(inputLevelMeter);
    top.addAndMakeVisible(inputKnob);
    top.addAndMakeVisible(outputKnob);
    top.addAndMakeVisible(outputLevelMeter);
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

    top.setBounds(0, 0, bounds.getWidth(), heightTop);
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
    outputKnob.setTopLeftPosition(bounds.getWidth() - knobPaddingFromBorder - outputKnob.getWidth(), knobYCoord);
    outputLevelMeter.setBounds(0, 0, widthLevelMeter, heightLevelMeter);
    outputLevelMeter.setTopLeftPosition(outputLevelX, outputLevelY);
}
