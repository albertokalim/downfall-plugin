/*
  ==============================================================================

    Utils.h
    Created: 23 Feb 2026 2:14:16pm
    Author:  Alberto

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <random>

using IIRFilter = juce::dsp::IIR::Filter<float>;
using IIRCoefs = juce::dsp::IIR::Coefficients<float>;

enum ShelfType{
    HighShelf,
    LowShelf
};

enum FilterType {
    HighPass,
    LowPass
};

// value must be a float in range [0, 1]
inline float mapValueInRange(float value, float min, float max) {
    jassert(value >= 0.f && value <= 1.f);
    return juce::jmap(value, min, max);
}

inline float randomInRange(float low, float high) {
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_real_distribution<> distr(low, high); // define the range

    return (float)distr(gen);
}

inline void prepareSmoothedValueObject(double sampleRate, juce::SmoothedValue<float>& s, float initialValue) {
    s.reset(sampleRate, 0.002f);
    s.setCurrentAndTargetValue(initialValue);
}

inline void prepareIIRFilter(juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs>& filter, juce::dsp::ProcessSpec& spec, 
    double sampleRate, float fq, float q, float gain) {
    filter.reset();
    *filter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, fq, q, gain);
    filter.prepare(spec);
}

inline void prepareIIRShelf(juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs>& filter, juce::dsp::ProcessSpec& spec, ShelfType type, 
    double sampleRate, float fq, float q, float gain) {
    filter.reset();

    switch (type) {
    case HighShelf:
        *filter.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate, fq, q, gain);
        break;
    case LowShelf:
        *filter.state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(sampleRate, fq, q, gain);
        break;
    }
    
    filter.prepare(spec);
}

inline void prepareIIRCutOffFilter(juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs>& filter, juce::dsp::ProcessSpec& spec, FilterType type,
    double sampleRate, float fq, float q) {
    filter.reset();

    switch (type) {
    case HighPass:
        *filter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, fq, q);
        break;
    case LowPass:
        *filter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, fq, q);
        break;
    }

    filter.prepare(spec);
}

inline void prepareGainObject(juce::dsp::Gain<float>& gain, juce::dsp::ProcessSpec spec, float initialValue) {
    gain.reset();
    gain.prepare(spec);
    gain.setGainLinear(initialValue);
}