/*
  ==============================================================================

    CleanAmp.cpp
    Created: 28 Apr 2026 11:35:17am
    Author:  Alberto

  ==============================================================================
*/

#include "CleanAmp.h"
#include "Utils.h"
void preamp::CleanAmp::prepare(juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;

    prepareGainObject(gain, spec, mapValueInRange(0.5f, minDrive, maxDrive));

    prepareSmoothedValueObject(spec.sampleRate, bassSmoother, 1.f);
    prepareSmoothedValueObject(spec.sampleRate, middleSmoother, 1.f);
    prepareSmoothedValueObject(spec.sampleRate, trebleSmoother, 1.f);

    prepareIIRFilter(lowMidBoost, spec, spec.sampleRate, 223.69f, 0.9f, 2.f);
    prepareIIRFilter(treble3kBoost, spec, spec.sampleRate, 3000.f, 0.9f, 2.f);
    prepareIIRFilter(pickStrokeAccentBoost, spec, spec.sampleRate, 1385.7f, 6.f, 1.413f);
    
    prepareIIRShelf(highShelf, spec, ShelfType::HighShelf, spec.sampleRate, 5000.f, 0.3f, 1.413f);

    prepareIIRFilter(bassEQ, spec, spec.sampleRate, BASS_CENTER_FQ, BASS_Q_FACTOR, 1.f);
    prepareIIRFilter(middleEQ, spec, spec.sampleRate, MID_CENTER_FQ, MID_Q_FACTOR, 1.f);
    prepareIIRFilter(trebleEQ, spec, spec.sampleRate, TREBLE_CENTER_FQ, TREBLE_Q_FACTOR, 1.f);

    prepareGainObject(gain, spec, 0.5f);

    prepareIIRFilter(postLowEndBoost, spec, spec.sampleRate, 165.29f, 1.113f, 2.2f);
    prepareIIRFilter(postMidBoost, spec, spec.sampleRate, 1298.f, 2.716f, 2.2f);
    prepareIIRCutOffFilter(lowPassFilter, spec, FilterType::LowPass, spec.sampleRate, 15000.f, 0.7f);

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

void preamp::CleanAmp::updateState(parameters::Parameters& parameters)
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

    master.setGainLinear(mapValueInRange(parameters.master.get() / 100.f, 0.f, 2.f));
}

void preamp::CleanAmp::manageInput(juce::dsp::ProcessContextReplacing<float>& context)
{
    gain.process(context);
}

void preamp::CleanAmp::prefilter(juce::dsp::ProcessContextReplacing<float>& context)
{
    if (bypassPreFilters) return;
    highShelf.process(context);
    lowMidBoost.process(context);
    treble3kBoost.process(context);
    pickStrokeAccentBoost.process(context);
}

void preamp::CleanAmp::waveshaping(juce::dsp::ProcessContextReplacing<float>& context)
{
    auto upSampleBlock = oversample->processSamplesUp(context.getOutputBlock());
    juce::dsp::ProcessContextReplacing<float> upSampleContext(upSampleBlock);

    waveshaper.process(upSampleContext);

    oversample->processSamplesDown(context.getOutputBlock());
}

void preamp::CleanAmp::postfilter(juce::dsp::ProcessContextReplacing<float>& context)
{
    if (bypassPostFilters) return;
    postLowEndBoost.process(context);
    postMidBoost.process(context);
    lowPassFilter.process(context);
}

void preamp::CleanAmp::eq(juce::dsp::ProcessContextReplacing<float>& context)
{
    bassEQ.process(context);
    middleEQ.process(context);
    trebleEQ.process(context);
}

void preamp::CleanAmp::manageOutput(juce::dsp::ProcessContextReplacing<float>& context)
{
    master.process(context);
}

void preamp::CleanAmp::reset()
{
}