/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "RotaryKnob.h"
#include "LevelMeter.h"
#include "AmpComponent.h"
#include "FXComponent.h"
#include "CabinetComponent.h"
#include "EQComponent.h"
#include "BypassButton.h"

//==============================================================================
/**
*/
class DownfallPluginAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    DownfallPluginAudioProcessorEditor (DownfallPluginAudioProcessor&);
    ~DownfallPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    DownfallPluginAudioProcessor& audioProcessor;
    juce::GroupComponent top;

    RotaryKnob inputKnob{"Input", audioProcessor.parameters.inputGain, true };
    RotaryKnob outputKnob{ "Output", audioProcessor.parameters.outputGain, true };
    LevelMeter inputLevelMeter, outputLevelMeter;
    BypassButton bypassButton{"On/Off", audioProcessor.parameters.bypass };
    juce::GroupComponent middle;
    juce::GroupComponent menu;
    juce::TextButton ampButton{ "Amp", "Show amp" };
    juce::TextButton fxButton{ "FX", "Show effects" };
    juce::TextButton cabinetButton{ "Cabinet", "Show screen to load IR" };
    juce::TextButton eqButton{ "EQ", "Show eq" };
    AmpComponent ampComponent;
    FXComponent fxComponent;
    CabinetComponent cabinetComponent;
    EQComponent eqComponent;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DownfallPluginAudioProcessorEditor)
};
