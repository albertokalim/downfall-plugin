/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DownfallPluginAudioProcessor::DownfallPluginAudioProcessor()
     : AudioProcessor (BusesProperties()
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true))
{
}

DownfallPluginAudioProcessor::~DownfallPluginAudioProcessor()
{
}

//==============================================================================
const juce::String DownfallPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DownfallPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DownfallPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DownfallPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DownfallPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DownfallPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DownfallPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DownfallPluginAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DownfallPluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void DownfallPluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DownfallPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    preamp::CleanAmp* cleanAmp = new preamp::CleanAmp(1.f, 7.5f);
    preamp::PreAmpDecorator* decoratorClean = new preamp::PreAmpDecorator(cleanAmp);
    preAmps[0] = std::unique_ptr<preamp::PreAmp>(new preamp::PreAmp(decoratorClean));

    preamp::HighGainAmp* highGainAmp = new preamp::HighGainAmp(25.f, 100.f);
    preamp::PreAmpDecorator* decoratorHighGain = new preamp::PreAmpDecorator(highGainAmp);
    preAmps[1] = std::unique_ptr<preamp::PreAmp>(new preamp::PreAmp(decoratorHighGain));

    inputGainSmoother.reset(sampleRate, 0.002f);
    inputGainSmoother.setCurrentAndTargetValue(0.f);

    outputGainSmoother.reset(sampleRate, 0.002f);
    outputGainSmoother.setCurrentAndTargetValue(0.f);

    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
    spec.numChannels = getTotalNumOutputChannels();
    spec.sampleRate = sampleRate;
    gate.reset();
    gate.prepare(spec);
    gate.setAttack(1.f);
    gate.setRelease(200.f);
    gate.setRatio(1.f);

    preAmps[0]->reset();
    preAmps[0]->prepare(spec);
    preAmps[1]->reset();
    preAmps[1]->prepare(spec);

    convolution.reset();
    convolution.prepare(spec);
    convolution.loadImpulseResponse(BinaryData::testir_wav, BinaryData::testir_wavSize,
        juce::dsp::Convolution::Stereo::yes,
        juce::dsp::Convolution::Trim::yes,
        0);
}

void DownfallPluginAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DownfallPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DownfallPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    inputGainSmoother.setTargetValue(globalParameters.getInputGain().get());
    outputGainSmoother.setTargetValue(globalParameters.getOutputGain().get());
    gate.setThreshold(globalParameters.getGateThreshold().get());

    buffer.applyGain(juce::Decibels::decibelsToGain(inputGainSmoother.getNextValue()));

    auto index = preAmpParameters.getAmpType().getIndex();
    preAmps[index]->update(preAmpParameters);

    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);

    if (!globalParameters.getBypassPreamp().get()) {
        preAmps[preAmpParameters.getAmpType().getIndex()]->process(context);
    }

    //gate.process(context);

    if (!globalParameters.getBypassCabinet().get()) {
        convolution.process(context);
    }

    buffer.applyGain(juce::Decibels::decibelsToGain(outputGainSmoother.getNextValue()));
}

//==============================================================================
bool DownfallPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DownfallPluginAudioProcessor::createEditor()
{
    return new DownfallPluginAudioProcessorEditor (*this);
}

//==============================================================================
void DownfallPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DownfallPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void DownfallPluginAudioProcessor::setIRToConvolution(juce::File newIRFile)
{
    convolution.loadImpulseResponse(newIRFile,
        juce::dsp::Convolution::Stereo::yes,
        juce::dsp::Convolution::Trim::yes,
        0);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DownfallPluginAudioProcessor();
}
