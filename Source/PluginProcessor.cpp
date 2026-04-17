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

    preAmps[0]->reset();
    preAmps[0]->prepare(spec);
    preAmps[1]->reset();
    preAmps[1]->prepare(spec);

    delay.reset();
    delay.prepare(spec);

    chorus.reset();
    chorus.prepare(spec);

    reverb.reset();
    reverb.prepare(spec);

    convolution.reset();
    convolution.prepare(spec);
    convolution.loadImpulseResponse(BinaryData::testir_wav, BinaryData::testir_wavSize,
        juce::dsp::Convolution::Stereo::yes,
        juce::dsp::Convolution::Trim::yes,
        0);

    eq.reset();
    eq.prepare(spec);

    outputLevelL.reset();
    outputLevelR.reset();
    inputLevelL.reset();
    inputLevelR.reset();
}

void DownfallPluginAudioProcessor::releaseResources()
{
    reverb.reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DownfallPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    //We are making sure that we are working with one and two channels only.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
        layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo()) {
        return false;
    }

    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet()) {
        return false;
    }

    return true;
}
#endif

void DownfallPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    if (parameters.bypass) return;

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    inputGainSmoother.setTargetValue(parameters.inputGain.get());
    outputGainSmoother.setTargetValue(parameters.outputGain.get());

    inputLevelL.updateIfGreater(buffer.getMagnitude(0, 0, buffer.getNumSamples()));
    inputLevelR.updateIfGreater(buffer.getMagnitude(1, 0, buffer.getNumSamples()));

    buffer.applyGain(juce::Decibels::decibelsToGain(inputGainSmoother.getNextValue()));

    auto index = parameters.ampType.getIndex();
    preAmps[index]->update(parameters);
    chorus.update(parameters);
    delay.updateTempoPlayHead(getPlayHead());
    delay.update(parameters);
    reverb.update(parameters);
    eq.update(parameters);
    
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);

    if (!parameters.bypassPreamp.get()) {
        preAmps[parameters.ampType.getIndex()]->process(context);
    }

    chorus.process(context);
    delay.process(context);
    reverb.process(context);

    if (!parameters.bypassCabinet.get()) {
        convolution.process(context);
    }

    eq.process(context);

    buffer.applyGain(juce::Decibels::decibelsToGain(outputGainSmoother.getNextValue()));

    outputLevelL.updateIfGreater(buffer.getMagnitude(0, 0, buffer.getNumSamples()));
    outputLevelR.updateIfGreater(buffer.getMagnitude(1, 0, buffer.getNumSamples()));
}

//==============================================================================
bool DownfallPluginAudioProcessor::hasEditor() const
{
    return true; 
}

juce::AudioProcessorEditor* DownfallPluginAudioProcessor::createEditor()
{
    return new DownfallPluginAudioProcessorEditor (*this);
}

//==============================================================================
void DownfallPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream outputStream{ destData, true };
    JsonSerializer::serializeParameters(parameters, outputStream);
}

void DownfallPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    juce::MemoryInputStream inputStream{ data, static_cast<size_t>(sizeInBytes), false };
    JsonSerializer::deserializeParameters(inputStream, parameters);
}

juce::AudioProcessorParameter* DownfallPluginAudioProcessor::getBypassParameter() const
{
    return &parameters.bypass;
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
