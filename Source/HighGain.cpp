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

    prepareSmoothedValueObject(spec.sampleRate, bassSmoother, 1.f);
    prepareSmoothedValueObject(spec.sampleRate, middleSmoother, 1.f);
    prepareSmoothedValueObject(spec.sampleRate, trebleSmoother, 1.f);
    prepareSmoothedValueObject(spec.sampleRate, presenceSmoother, 1.f);

    prepareGainObject(gain, spec, mapValueInRange(0.5f, minDrive, maxDrive));
    prepareGainObject(master, spec, 0.5f);

    prepareIIRFilter(lowMidBoost, spec, spec.sampleRate, 223.69f, 0.9f, 2.f);
    prepareIIRFilter(lowEndControl, spec, spec.sampleRate, 83.156f, 1.899f, 0.767f);
    prepareIIRFilter(mid1kBoost, spec, spec.sampleRate, 781.74f, 1.154f, 2.3f);
    prepareIIRFilter(treble3kBoost, spec, spec.sampleRate, 3000.f, 0.9f, 2.f);
    prepareIIRFilter(pickStrokeAccentBoost, spec, spec.sampleRate, 1385.7f, 6.f, 1.413f);
    prepareIIRShelf(highShelf, spec, ShelfType::HighShelf, spec.sampleRate, 5000.f, 0.3f, 2.f);

    prepareIIRCutOffFilter(hpfPostFilter, spec, FilterType::HighPass, spec.sampleRate, 81.23f, HPF_Q_FACTOR);
    prepareIIRShelf(highShelfPostFilter, spec, ShelfType::HighShelf, spec.sampleRate, 5000.f, 0.3f, 2.f);
    prepareIIRFilter(midBoostPostFilter, spec, spec.sampleRate, 1632.f, 3.087f, 1.9f);
    prepareIIRFilter(lowEndControlPostFilter, spec, spec.sampleRate, 85.921f, 1.858f, 0.707f);

    prepareIIRFilter(bassEQ, spec, spec.sampleRate, BASS_CENTER_FQ, BASS_Q_FACTOR, 1.f);
    prepareIIRFilter(middleEQ, spec, spec.sampleRate, MID_CENTER_FQ, MID_Q_FACTOR, 1.f);
    prepareIIRFilter(trebleEQ, spec, spec.sampleRate, TREBLE_CENTER_FQ, TREBLE_Q_FACTOR, 1.f);
    prepareIIRFilter(presenceEQ, spec, spec.sampleRate, 4500.f, 0.707, 1.f);

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
    bypassPostFilters = parameters.bypassPostFilters;
    bypassPreFilters = parameters.bypassPreFilters;

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
    *presenceEQ.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate, 
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
    if (bypassPreFilters) return;
    highShelf.process(context);
    lowMidBoost.process(context);
    lowEndControl.process(context);
    mid1kBoost.process(context);
    treble3kBoost.process(context);
    pickStrokeAccentBoost.process(context);
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
    if (bypassPostFilters) return;
    hpfPostFilter.process(context);
    highShelfPostFilter.process(context);
    midBoostPostFilter.process(context);
    lowEndControlPostFilter.process(context);
}

void preamp::HighGainAmp::eq(juce::dsp::ProcessContextReplacing<float>& context)
{
    bassEQ.process(context);
    middleEQ.process(context);
    trebleEQ.process(context);
    presenceEQ.process(context);
}

void preamp::HighGainAmp::manageOutput(juce::dsp::ProcessContextReplacing<float>& context)
{
    master.process(context);
}

void preamp::HighGainAmp::reset()
{
}
