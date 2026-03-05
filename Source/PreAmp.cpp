/*
  ==============================================================================

    PreAmp.cpp
    Created: 5 Mar 2026 7:58:07pm
    Author:  Alberto

  ==============================================================================
*/

#include "PreAmp.h"

void preamp::PreAmp::updateCommonParameters(parameters::PreAmpParameters& parameters) {

    gain.setGainLinear(mapValueInRange(parameters.getGain().get() / 100.f, MIN_DRIVE, MAX_DRIVE));

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

    master.setGainLinear(parameters.getMaster().get() / 100.f);
}

void preamp::PreAmp::prepareCommonParameters(juce::dsp::ProcessSpec& spec) { //Esto alomejor va fuera.

    sampleRate = spec.sampleRate;

    gain.reset();
    gain.prepare(spec);
    gain.setGainLinear(mapValueInRange(0.5f, minDrive, maxDrive));

    distortion->prepare(spec);

    bassSmoother.reset(spec.sampleRate, 0.002f);
    bassSmoother.setCurrentAndTargetValue(mapValueInRange(0.5f, MIN_BAND_GAIN, MAX_BAND_GAIN));

    middleSmoother.reset(spec.sampleRate, 0.002f);
    middleSmoother.setCurrentAndTargetValue(mapValueInRange(0.5f, MIN_BAND_GAIN, MAX_BAND_GAIN));

    trebleSmoother.reset(spec.sampleRate, 0.002f);
    trebleSmoother.setCurrentAndTargetValue(mapValueInRange(0.5f, MIN_BAND_GAIN, MAX_BAND_GAIN));

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
}

void preamp::PreAmp::process(juce::dsp::ProcessContextReplacing<float>& context) {
    gain.process(context);

    /*auto upSampleBlock = oversampler->processSamplesUp(context.getOutputBlock());
    juce::dsp::ProcessContextReplacing<float> upSampleContext(upSampleBlock);

    waveShaper.process(upSampleContext);
    highPassFilter.process(upSampleContext);
    lowPassFilter.process(upSampleContext);

    oversampler->processSamplesDown(context.getOutputBlock());*/
    distortion->applyDistortion(context);

    bassEQ.process(context);
    middleEQ.process(context);
    trebleEQ.process(context);

    master.process(context);
}