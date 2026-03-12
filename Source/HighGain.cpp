/*
  ==============================================================================

    HighGain.cpp
    Created: 12 Mar 2026 7:31:26pm
    Author:  Alberto

  ==============================================================================
*/

#include "HighGain.h"

void preamp::HighGainAmp::prepare(juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;

    gain.reset();
    gain.prepare(spec);
    gain.setGainLinear(mapValueInRange(0.5f, minDrive, maxDrive));

    master.reset();
    master.prepare(spec);
    master.setGainLinear(0.5f);

    oversample = std::unique_ptr<juce::dsp::Oversampling<float>>(new juce::dsp::Oversampling<float>(spec.numChannels,
        2,
        juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR)
    );

    oversample->reset();
    oversample->initProcessing(spec.maximumBlockSize);

    juce::dsp::ProcessSpec osSpec;
    osSpec.maximumBlockSize = spec.maximumBlockSize;
    osSpec.numChannels = spec.numChannels;
    osSpec.sampleRate = spec.sampleRate * oversample->getOversamplingFactor();

    waveshaperStage1.reset();
    waveshaperStage1.prepare(osSpec);
    waveshaperStage2.reset();
    waveshaperStage2.prepare(osSpec);
    waveshaperStage3.reset();
    waveshaperStage3.prepare(osSpec);
    waveshaperStage4.reset();
    waveshaperStage4.prepare(osSpec);
}

void preamp::HighGainAmp::updateState(parameters::PreAmpParameters& parameters)
{
    gain.setGainLinear(mapValueInRange(parameters.getGain().get() / 100.f, minDrive, maxDrive));

    master.setGainLinear(mapValueInRange(parameters.getMaster().get() / 100.f, 0.f, 2.f));
}

void preamp::HighGainAmp::manageInput(juce::dsp::ProcessContextReplacing<float>& context)
{
    gain.process(context);
}

void preamp::HighGainAmp::prefilter(juce::dsp::ProcessContextReplacing<float>& context)
{
}

void preamp::HighGainAmp::waveshaping(juce::dsp::ProcessContextReplacing<float>& context)
{
    auto upSampleBlock = oversample->processSamplesUp(context.getOutputBlock());
    juce::dsp::ProcessContextReplacing<float> upSampleContext(upSampleBlock);

    waveshaperStage1.process(upSampleContext);

    oversample->processSamplesDown(context.getOutputBlock());
}

void preamp::HighGainAmp::postfilter(juce::dsp::ProcessContextReplacing<float>& context)
{
}

void preamp::HighGainAmp::eq(juce::dsp::ProcessContextReplacing<float>& context)
{
}

void preamp::HighGainAmp::manageOutput(juce::dsp::ProcessContextReplacing<float>& context)
{
    master.process(context);
}

void preamp::HighGainAmp::reset()
{
}
