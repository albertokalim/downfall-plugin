/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Parameters.h"
#include "PreAmp.h"
#include "HighGain.h"
#include "Effect.h"
#include "Reverb.h"

//==============================================================================
/**
*/
class DownfallPluginAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    DownfallPluginAudioProcessor();
    ~DownfallPluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void setIRToConvolution(juce::File newIRFile);

private:
    parameters::GlobalParameters globalParameters{ *this };
    parameters::PreAmpParameters preAmpParameters{ *this };
    parameters::DelayParameters delayParameters{ *this };
    parameters::ChorusParameters chorusParameters{ *this };
    parameters::ReverbParameters reverbParameters{ *this };

    juce::SmoothedValue<float> inputGainSmoother;
    juce::SmoothedValue<float> outputGainSmoother;

    juce::dsp::NoiseGate<float> gate;
    std::vector<std::unique_ptr<preamp::PreAmp>> preAmps{ 2 };

    effects::DelayFX delay;
    effects::ChorusFX chorus;
    effects::ReverbFX reverb{ 100.0 };

    juce::dsp::Convolution convolution;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DownfallPluginAudioProcessor)
};
