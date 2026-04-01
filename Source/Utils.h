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