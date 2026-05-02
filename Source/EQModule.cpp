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

    prepareSmoothedValueObject(sampleRate, highPassCenterFqSmoother, 0.f);
    prepareSmoothedValueObject(sampleRate, lowPassCenterFqSmoother, 0.f);
    prepareSmoothedValueObject(sampleRate, gain65Smoother, 0.f);
    prepareSmoothedValueObject(sampleRate, gain125Smoother, 0.f);
    prepareSmoothedValueObject(sampleRate, gain250Smoother, 0.f);
    prepareSmoothedValueObject(sampleRate, gain500Smoother, 0.f);
    prepareSmoothedValueObject(sampleRate, gain1kSmoother, 0.f);
    prepareSmoothedValueObject(sampleRate, gain2kSmoother, 0.f);
    prepareSmoothedValueObject(sampleRate, gain4kSmoother, 0.f);
    prepareSmoothedValueObject(sampleRate, gain8kSmoother, 0.f);
    prepareSmoothedValueObject(sampleRate, gain16kSmoother, 0.f);

    prepareIIRCutOffFilter(highPassFilter, spec, FilterType::HighPass, sampleRate, 20.f, 0.6f);
    prepareIIRCutOffFilter(lowPassFilter, spec, FilterType::LowPass, sampleRate, 20000.f, 0.6f);
    prepareIIRFilter(bell65, spec, sampleRate, 65.0f, 0.9f, 1.f);
    prepareIIRFilter(bell125, spec, sampleRate, 125.0f, 0.9f, 1.f);
    prepareIIRFilter(bell250, spec, sampleRate, 250.0f, 0.9f, 1.f);
    prepareIIRFilter(bell500, spec, sampleRate, 500.0f, 0.9f, 1.f);
    prepareIIRFilter(bell1k, spec, sampleRate, 1000.0f, 0.9f, 1.f);
    prepareIIRFilter(bell2k, spec, sampleRate, 2000.0f, 0.9f, 1.f);
    prepareIIRFilter(bell4k, spec, sampleRate, 4000.0f, 0.9f, 1.f);
    prepareIIRFilter(bell8k, spec, sampleRate, 8000.0f, 0.9f, 1.f);
    prepareIIRFilter(bell16k, spec, sampleRate, 16000.0f, 0.9f, 1.f);
}

void EQModule::update(parameters::Parameters& parameters)
{
    bypass = parameters.bypassEq;

    highPassCenterFqSmoother.setTargetValue(parameters.hpfFq.get());
    currentHighPassFq = highPassCenterFqSmoother.getNextValue();
    if(currentHighPassFq > 0.f)
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
