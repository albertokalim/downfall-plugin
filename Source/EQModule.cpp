/*
  ==============================================================================

    EQModule.cpp
    Created: 14 Apr 2026 1:53:41pm
    Author:  Alberto

  ==============================================================================
*/

#include "EQModule.h"

EQModule::EQModule()
{

}

void EQModule::prepare(juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;

    highPassCenterFqSmoother.reset(spec.sampleRate, 0.002f);
    highPassCenterFqSmoother.setCurrentAndTargetValue(0.f);

    lowPassCenterFqSmoother.reset(spec.sampleRate, 0.002f);
    lowPassCenterFqSmoother.setCurrentAndTargetValue(0.f);

    gain65Smoother.reset(spec.sampleRate, 0.002f);
    gain65Smoother.setCurrentAndTargetValue(0.f);

    gain125Smoother.reset(spec.sampleRate, 0.002f);
    gain125Smoother.setCurrentAndTargetValue(0.f);

    gain250Smoother.reset(spec.sampleRate, 0.002f);
    gain250Smoother.setCurrentAndTargetValue(0.f);

    gain500Smoother.reset(spec.sampleRate, 0.002f);
    gain500Smoother.setCurrentAndTargetValue(0.f);

    gain1kSmoother.reset(spec.sampleRate, 0.002f);
    gain1kSmoother.setCurrentAndTargetValue(0.f);

    gain2kSmoother.reset(spec.sampleRate, 0.002f);
    gain2kSmoother.setCurrentAndTargetValue(0.f);

    gain4kSmoother.reset(spec.sampleRate, 0.002f);
    gain4kSmoother.setCurrentAndTargetValue(0.f);

    gain8kSmoother.reset(spec.sampleRate, 0.002f);
    gain8kSmoother.setCurrentAndTargetValue(0.f);

    gain16kSmoother.reset(spec.sampleRate, 0.002f);
    gain16kSmoother.setCurrentAndTargetValue(0.f);

    highPassFilter.reset();
    highPassFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(spec.sampleRate, 20.f);
    highPassFilter.prepare(spec);

    lowPassFilter.reset();
    lowPassFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(spec.sampleRate, 20000.f);
    lowPassFilter.prepare(spec);

    bell65.reset();
    bell65.coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, 65.0f, 0.9f, 1.f);
    bell65.prepare(spec);

    bell125.reset();
    bell125.coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, 125.0f, 0.9f, 1.f);
    bell125.prepare(spec);

    bell250.reset();
    bell250.coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, 250.0f, 0.9f, 1.f);
    bell250.prepare(spec);

    bell500.reset();
    bell500.coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, 500.0f, 0.9f, 1.f);
    bell500.prepare(spec);

    bell1k.reset();
    bell1k.coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, 1000.0f, 0.9f, 1.f);
    bell1k.prepare(spec);

    bell2k.reset();
    bell2k.coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, 2000.0f, 0.9f, 1.f);
    bell2k.prepare(spec);

    bell4k.reset();
    bell4k.coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, 4000.0f, 0.9f, 1.f);
    bell4k.prepare(spec);

    bell8k.reset();
    bell8k.coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, 8000.0f, 0.9f, 1.f);
    bell8k.prepare(spec);

    bell16k.reset();
    bell16k.coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, 16000.0f, 0.9f, 1.f);
    bell16k.prepare(spec);
}

void EQModule::update(parameters::Parameters& parameters)
{
    highPassCenterFqSmoother.setTargetValue(parameters.hpfFq.get());
    highPassFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, highPassCenterFqSmoother.getNextValue());

    lowPassCenterFqSmoother.setTargetValue(parameters.lpfFq.get());
    highPassFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, lowPassCenterFqSmoother.getNextValue());

    gain65Smoother.setTargetValue(parameters.eq65hzGain.get());
    bell65.coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 65.0f, 0.9f, juce::Decibels::decibelsToGain(gain65Smoother.getNextValue()));

    gain125Smoother.setTargetValue(parameters.eq65hzGain.get());
    bell125.coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 65.0f, 0.9f, juce::Decibels::decibelsToGain(gain125Smoother.getNextValue()));

    gain250Smoother.setTargetValue(parameters.eq65hzGain.get());
    bell250.coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 65.0f, 0.9f, juce::Decibels::decibelsToGain(gain250Smoother.getNextValue()));

    gain500Smoother.setTargetValue(parameters.eq65hzGain.get());
    bell500.coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 65.0f, 0.9f, juce::Decibels::decibelsToGain(gain500Smoother.getNextValue()));

    gain1kSmoother.setTargetValue(parameters.eq65hzGain.get());
    bell1k.coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 65.0f, 0.9f, juce::Decibels::decibelsToGain(gain1kSmoother.getNextValue()));

    gain2kSmoother.setTargetValue(parameters.eq65hzGain.get());
    bell2k.coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 65.0f, 0.9f, juce::Decibels::decibelsToGain(gain2kSmoother.getNextValue()));

    gain4kSmoother.setTargetValue(parameters.eq65hzGain.get());
    bell4k.coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 65.0f, 0.9f, juce::Decibels::decibelsToGain(gain4kSmoother.getNextValue()));

    gain8kSmoother.setTargetValue(parameters.eq65hzGain.get());
    bell8k.coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 65.0f, 0.9f, juce::Decibels::decibelsToGain(gain8kSmoother.getNextValue()));

    gain16kSmoother.setTargetValue(parameters.eq65hzGain.get());
    bell16k.coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 65.0f, 0.9f, juce::Decibels::decibelsToGain(gain16kSmoother.getNextValue()));
}

void EQModule::process(juce::dsp::ProcessContextReplacing<float>& context)
{
    if (highPassCenterFqSmoother.getNextValue() > 0.f) highPassFilter.process(context);
    if (lowPassCenterFqSmoother.getNextValue() > 0.f) lowPassFilter.process(context);
    bell65.process(context);
    bell125.process(context);
    bell250.process(context);
    bell500.process(context);
    bell1k.process(context);
    bell2k.process(context);
    bell4k.process(context);
    bell8k.process(context);
    bell16k.process(context);
}

void EQModule::reset()
{

}
