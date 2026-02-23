/*
  ==============================================================================

    Utils.h
    Created: 23 Feb 2026 2:14:16pm
    Author:  Alberto

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

// value must be a float in range [0, 1]
inline float mapValueInRange(float value, float min, float max) {
    jassert(value >= 0.f && value <= 1.f);
    return juce::jmap(value, min, max);
}