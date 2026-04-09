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
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DownfallPluginAudioProcessor& audioProcessor;
    juce::GroupComponent top;
    RotaryKnob inputKnob{"Input", true };
    RotaryKnob outputKnob{ "Output", true };
    LevelMeter inputLevelMeter, outputLevelMeter;
    juce::GroupComponent middle;
    juce::GroupComponent menu;
    juce::TextButton ampButton{ "Amp", "Show amp" };
    juce::TextButton fxButton{ "FX", "Show effects" };
    juce::TextButton cabinetButton{ "Cabinet", "Show screen to load IR" };
    juce::TextButton eqButton{ "EQ", "Show eq" };
    juce::Label ampComponent{ "Amp" };
    juce::Label fxComponent{ "FX" };
    juce::Label cabinetComponent{ "Cabinet" };
    juce::Label eqComponent{ "EQ" };
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DownfallPluginAudioProcessorEditor)
};
