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
    juce::GroupComponent middle;
    juce::GroupComponent menu;

    RotaryKnob inputKnob{"Input", audioProcessor.parameters.inputGain, true };
    RotaryKnob outputKnob{ "Output", audioProcessor.parameters.outputGain, true };
    RotaryKnob gateKnob{ "Gate", audioProcessor.parameters.gateThreshold };
    LevelMeter inputLevelMeter, outputLevelMeter;
    BypassButton bypassButton{"Bypass Plugin", audioProcessor.parameters.bypass };
    juce::TextButton ampButton{ "Amp", "Show amp" };
    juce::TextButton fxButton{ "FX", "Show effects" };
    juce::TextButton cabinetButton{ "Cabinet", "Show screen to load IR" };
    juce::TextButton eqButton{ "EQ", "Show eq" };
    AmpComponent ampComponent;
    FXComponent fxComponent;
    juce::Label textIR{ "Default IR" };
    juce::TextButton buttonIRLoader{ "Load IR" };
    juce::FileChooser fileChooser{ "Select an IR file to load", juce::File::getSpecialLocation(juce::File::userHomeDirectory), "*.wav" };
    juce::AlertWindow alertWindow{ "Success!!", "IR Succesfully loaded", juce::MessageBoxIconType::InfoIcon };
    BypassButton bypassCabinet;
    EQComponent eqComponent;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DownfallPluginAudioProcessorEditor)
};
