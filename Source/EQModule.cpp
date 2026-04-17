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
    *highPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(spec.sampleRate, 20.f);
    highPassFilter.prepare(spec);

    lowPassFilter.reset();
    *lowPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(spec.sampleRate, 20000.f);
    lowPassFilter.prepare(spec);

    bell65.reset();
    *bell65.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, 65.0f, 0.9f, 1.f);
    bell65.prepare(spec);

    bell125.reset();
    *bell125.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, 125.0f, 0.9f, 1.f);
    bell125.prepare(spec);

    bell250.reset();
    *bell250.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, 250.0f, 0.9f, 1.f);
    bell250.prepare(spec);

    bell500.reset();
    *bell500.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, 500.0f, 0.9f, 1.f);
    bell500.prepare(spec);

    bell1k.reset();
    *bell1k.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, 1000.0f, 0.9f, 1.f);
    bell1k.prepare(spec);

    bell2k.reset();
    *bell2k.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, 2000.0f, 0.9f, 1.f);
    bell2k.prepare(spec);

    bell4k.reset();
    *bell4k.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, 4000.0f, 0.9f, 1.f);
    bell4k.prepare(spec);

    bell8k.reset();
    *bell8k.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, 8000.0f, 0.9f, 1.f);
    bell8k.prepare(spec);

    bell16k.reset();
    *bell16k.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, 16000.0f, 0.9f, 1.f);
    bell16k.prepare(spec);
}

void EQModule::update(parameters::Parameters& parameters)
{
    bypass = parameters.bypassEq;

    highPassCenterFqSmoother.setTargetValue(parameters.hpfFq.get());
    currentHighPassFq = highPassCenterFqSmoother.getNextValue();
    *highPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, currentHighPassFq);

    lowPassCenterFqSmoother.setTargetValue(parameters.lpfFq.get());
    currentLowPassFq = lowPassCenterFqSmoother.getNextValue();
    *lowPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, currentLowPassFq);

    gain65Smoother.setTargetValue(parameters.eq65hzGain.get());
    *bell65.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 65.0f, 0.9f, juce::Decibels::decibelsToGain(gain65Smoother.getNextValue()));

    gain125Smoother.setTargetValue(parameters.eq125hzGain.get());
    *bell125.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 125.0f, 0.9f, juce::Decibels::decibelsToGain(gain125Smoother.getNextValue()));

    gain250Smoother.setTargetValue(parameters.eq250hzGain.get());
    *bell250.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 250.0f, 0.9f, juce::Decibels::decibelsToGain(gain250Smoother.getNextValue()));

    gain500Smoother.setTargetValue(parameters.eq500hzGain.get());
    *bell500.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 500.0f, 0.9f, juce::Decibels::decibelsToGain(gain500Smoother.getNextValue()));

    gain1kSmoother.setTargetValue(parameters.eq1khzGain.get());
    *bell1k.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 1000.0f, 0.9f, juce::Decibels::decibelsToGain(gain1kSmoother.getNextValue()));

    gain2kSmoother.setTargetValue(parameters.eq2khzGain.get());
    *bell2k.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 2000.0f, 0.9f, juce::Decibels::decibelsToGain(gain2kSmoother.getNextValue()));

    gain4kSmoother.setTargetValue(parameters.eq4khzGain.get());
    *bell4k.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 4000.0f, 0.9f, juce::Decibels::decibelsToGain(gain4kSmoother.getNextValue()));

    gain8kSmoother.setTargetValue(parameters.eq8khzGain.get());
    *bell8k.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 8000.0f, 0.9f, juce::Decibels::decibelsToGain(gain8kSmoother.getNextValue()));

    gain16kSmoother.setTargetValue(parameters.eq16khzGain.get());
    *bell16k.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 16000.0f, 0.9f, juce::Decibels::decibelsToGain(gain16kSmoother.getNextValue()));
}

void EQModule::process(juce::dsp::ProcessContextReplacing<float>& context)
{
    if (bypass) return;

    if (currentHighPassFq > 0.f) highPassFilter.process(context);
    if (currentLowPassFq < 20000.f) lowPassFilter.process(context);
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
