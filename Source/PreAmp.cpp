/*
  ==============================================================================

    PreAmp.cpp
    Created: 11 Mar 2026 11:52:16am
    Author:  Alberto

  ==============================================================================
*/

#include "PreAmp.h"

void preamp::CleanAmp::prepare(juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;

    gain.reset();
    gain.prepare(spec);
    gain.setGainLinear(mapValueInRange(0.5f, minDrive, maxDrive));

    bassSmoother.reset(spec.sampleRate, 0.002f);
    bassSmoother.setCurrentAndTargetValue(mapValueInRange(0.5f, MIN_BAND_GAIN, MAX_BAND_GAIN));

    middleSmoother.reset(spec.sampleRate, 0.002f);
    middleSmoother.setCurrentAndTargetValue(mapValueInRange(0.5f, MIN_BAND_GAIN, MAX_BAND_GAIN));

    trebleSmoother.reset(spec.sampleRate, 0.002f);
    trebleSmoother.setCurrentAndTargetValue(mapValueInRange(0.5f, MIN_BAND_GAIN, MAX_BAND_GAIN));

    lowMidBoost.reset();
    *lowMidBoost.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, 223.69f, 0.9f, 8.f);
    lowMidBoost.prepare(spec);

    midBoost.reset();
    *midBoost.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, 3000.f, 0.9f, 8.f);
    midBoost.prepare(spec);

    pickAccentBoost.reset();
    *pickAccentBoost.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, 1385.7f, 6.f, 4.f);
    pickAccentBoost.prepare(spec);

    highShelf.reset();
    *highShelf.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(spec.sampleRate, 5000.f, 0.3f, 4.f);
    highShelf.prepare(spec);

    bassEQ.reset();
    *bassEQ.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, BASS_CENTER_FQ, BASS_Q_FACTOR, 0.f);
    bassEQ.prepare(spec);

    middleEQ.reset();
    *middleEQ.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, MID_CENTER_FQ, MID_Q_FACTOR, 0.f);
    middleEQ.prepare(spec);

    trebleEQ.reset();
    *trebleEQ.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, TREBLE_CENTER_FQ, TREBLE_Q_FACTOR, 0.f);
    trebleEQ.prepare(spec);

    master.reset();
    master.prepare(spec);
    master.setGainLinear(0.5f);

    highPassFilter.reset();
    *highPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(spec.sampleRate, HPF_CENTER_FQ, HPF_Q_FACTOR);
    highPassFilter.prepare(spec);

    lowPassFilter.reset();
    *lowPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(spec.sampleRate, 15000.f, 0.7f);
    lowPassFilter.prepare(spec);

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

void preamp::CleanAmp::updateState(parameters::PreAmpParameters& parameters)
{
    gain.setGainLinear(mapValueInRange(parameters.getGain().get() / 100.f, minDrive, maxDrive));

    bassSmoother.setTargetValue(mapValueInRange(parameters.getBass().get() / 100.f, MIN_BAND_GAIN, MAX_BAND_GAIN));
    *bassEQ.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate,
        BASS_CENTER_FQ,
        BASS_Q_FACTOR,
        juce::Decibels::decibelsToGain(bassSmoother.getNextValue()));

    middleSmoother.setTargetValue(mapValueInRange(parameters.getMiddle().get() / 100.f, MIN_BAND_GAIN, MAX_BAND_GAIN));
    *middleEQ.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate,
        MID_CENTER_FQ,
        MID_Q_FACTOR,
        juce::Decibels::decibelsToGain(middleSmoother.getNextValue()));

    trebleSmoother.setTargetValue(mapValueInRange(parameters.getTreble().get() / 100.f, MIN_BAND_GAIN, MAX_BAND_GAIN));
    *trebleEQ.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate,
        TREBLE_CENTER_FQ,
        TREBLE_Q_FACTOR,
        juce::Decibels::decibelsToGain(trebleSmoother.getNextValue()));

    master.setGainLinear(mapValueInRange(parameters.getMaster().get() / 100.f, 0.f, 2.f));
}

void preamp::CleanAmp::manageInput(juce::dsp::ProcessContextReplacing<float>& context)
{
    gain.process(context);
}

void preamp::CleanAmp::prefilter(juce::dsp::ProcessContextReplacing<float>& context)
{
    highShelf.process(context);
    lowMidBoost.process(context);
    midBoost.process(context);
    pickAccentBoost.process(context);
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
    //highPassFilter.process(context);
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
