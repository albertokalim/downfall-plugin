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

    bassSmoother.reset(spec.sampleRate, 0.002f);
    bassSmoother.setCurrentAndTargetValue(1.f);

    middleSmoother.reset(spec.sampleRate, 0.002f);
    middleSmoother.setCurrentAndTargetValue(1.f);

    trebleSmoother.reset(spec.sampleRate, 0.002f);
    trebleSmoother.setCurrentAndTargetValue(1.f);

    presenceSmoother.reset(spec.sampleRate, 0.002f);
    presenceSmoother.setCurrentAndTargetValue(1.f);

    gain.reset();
    gain.prepare(spec);
    gain.setGainLinear(mapValueInRange(0.5f, minDrive, maxDrive));

    master.reset();
    master.prepare(spec);
    master.setGainLinear(0.5f);

    lowMidBoost.reset();
    *lowMidBoost.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, 223.69f, 0.9f, 2.f);
    lowMidBoost.prepare(spec);

    lowEndControl.reset();
    *lowEndControl.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, 83.156f, 1.899f, 0.767f);
    lowEndControl.prepare(spec);

    midBoost2.reset();
    *midBoost2.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, 781.74f, 1.154f, 2.3f);
    midBoost2.prepare(spec);

    midBoost.reset();
    *midBoost.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, 3000.f, 0.9f, 2.f);
    midBoost.prepare(spec);

    pickAccentBoost.reset();
    *pickAccentBoost.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, 1385.7f, 6.f, 1.413f);
    pickAccentBoost.prepare(spec);

    highShelf.reset();
    *highShelf.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(spec.sampleRate, 5000.f, 0.3f, 2.f);
    highShelf.prepare(spec);

    hpfPostWaveshaper.reset();
    *hpfPostWaveshaper.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(spec.sampleRate, 81.23f, HPF_Q_FACTOR);
    hpfPostWaveshaper.prepare(spec);

    highShelfPostWaveshaper.reset();
    *highShelfPostWaveshaper.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(spec.sampleRate, 1860.1f, 0.3f, 2.1f);
    highShelfPostWaveshaper.prepare(spec);

    pickAccentPostWaveshaper.reset();
    *pickAccentPostWaveshaper.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, 1632.f, 3.087f, 1.9f);
    pickAccentPostWaveshaper.prepare(spec);

    lowEndControlPostWaveshaper.reset();
    *lowEndControlPostWaveshaper.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, 85.921f, 1.858f, 0.707f);
    lowEndControlPostWaveshaper.prepare(spec);

    bassEQ.reset();
    *bassEQ.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, BASS_CENTER_FQ, BASS_Q_FACTOR, 1.f);
    bassEQ.prepare(spec);

    middleEQ.reset();
    *middleEQ.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, MID_CENTER_FQ, MID_Q_FACTOR, 1.f);
    middleEQ.prepare(spec);

    trebleEQ.reset();
    *trebleEQ.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, TREBLE_CENTER_FQ, TREBLE_Q_FACTOR, 1.f);
    trebleEQ.prepare(spec);

    presence.reset();
    *presence.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(spec.sampleRate, 4500.f, 0.707, 1.f);
    presence.prepare(spec);

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

    waveshaper.reset();
    waveshaper.prepare(osSpec);
}

void preamp::HighGainAmp::updateState(parameters::Parameters& parameters)
{
    gain.setGainLinear(mapValueInRange(parameters.gain.get() / 100.f, minDrive, maxDrive));

    bassSmoother.setTargetValue(mapValueInRange(parameters.bass.get() / 100.f, MIN_BAND_GAIN, MAX_BAND_GAIN));
    *bassEQ.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate,
        BASS_CENTER_FQ,
        BASS_Q_FACTOR,
        juce::Decibels::decibelsToGain(bassSmoother.getNextValue()));

    middleSmoother.setTargetValue(mapValueInRange(parameters.middle.get() / 100.f, MIN_BAND_GAIN, MAX_BAND_GAIN));
    *middleEQ.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate,
        MID_CENTER_FQ,
        MID_Q_FACTOR,
        juce::Decibels::decibelsToGain(middleSmoother.getNextValue()));

    trebleSmoother.setTargetValue(mapValueInRange(parameters.treble.get() / 100.f, MIN_BAND_GAIN, MAX_BAND_GAIN));
    *trebleEQ.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate,
        TREBLE_CENTER_FQ,
        TREBLE_Q_FACTOR,
        juce::Decibels::decibelsToGain(trebleSmoother.getNextValue()));

    presenceSmoother.setTargetValue(mapValueInRange(parameters.presence.get() / 100.f, MIN_BAND_GAIN, MAX_BAND_GAIN));
    *presence.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate, 
        4500.f, 
        0.707,
        juce::Decibels::decibelsToGain(presenceSmoother.getNextValue()));

    master.setGainLinear(mapValueInRange(parameters.master.get() / 100.f, 0.f, 2.f));
}

void preamp::HighGainAmp::manageInput(juce::dsp::ProcessContextReplacing<float>& context)
{
    gain.process(context);
}

void preamp::HighGainAmp::prefilter(juce::dsp::ProcessContextReplacing<float>& context)
{
    highShelf.process(context);
    lowMidBoost.process(context);
    lowEndControl.process(context);
    midBoost2.process(context);
    midBoost.process(context);
    pickAccentBoost.process(context);
}

void preamp::HighGainAmp::waveshaping(juce::dsp::ProcessContextReplacing<float>& context)
{
    auto upSampleBlock = oversample->processSamplesUp(context.getOutputBlock());
    juce::dsp::ProcessContextReplacing<float> upSampleContext(upSampleBlock);

    waveshaper.process(upSampleContext);

    oversample->processSamplesDown(context.getOutputBlock());
}

void preamp::HighGainAmp::postfilter(juce::dsp::ProcessContextReplacing<float>& context)
{
    hpfPostWaveshaper.process(context);
    highShelfPostWaveshaper.process(context);
    pickAccentPostWaveshaper.process(context);
    lowEndControlPostWaveshaper.process(context);
}

void preamp::HighGainAmp::eq(juce::dsp::ProcessContextReplacing<float>& context)
{
    bassEQ.process(context);
    middleEQ.process(context);
    trebleEQ.process(context);
    presence.process(context);
}

void preamp::HighGainAmp::manageOutput(juce::dsp::ProcessContextReplacing<float>& context)
{
    master.process(context);
}

void preamp::HighGainAmp::reset()
{
}
