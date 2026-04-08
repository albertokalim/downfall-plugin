/*
  ==============================================================================

    LevelMeter.h
    Created: 7 Apr 2026 7:46:57pm
    Author:  Alberto

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Measurement.h"

class LevelMeter : public juce::Component, private juce::Timer
{
public:
    LevelMeter(Measurement& measurementL_, Measurement& measurementR_);
    ~LevelMeter() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;

    Measurement& measurementL;
    Measurement& measurementR;
    float dbLevelL;
    float dbLevelR;

    float decay = 0.0f;
    float levelL = clampLevel;
    float levelR = clampLevel;
    float maxPos = 0.0f;
    float minPos = 0.0f;

    static constexpr float maxdB = 6.0f;
    static constexpr float mindB = -60.0f;
    static constexpr float stepdB = 6.0f;
    static constexpr float clampdB = -120.0f;
    static constexpr float clampLevel = 0.000001f;
    static constexpr int refreshRate = 60;

    void drawLevel(juce::Graphics& g, float level, int x, int width);
    void updateLevel(float newLevel, float& smoothedLevel, float& leveldB) const;

    int positionForLevel(float dbLevel) const noexcept
    {
        return int(std::round(juce::jmap(dbLevel, mindB, maxdB, minPos, maxPos)));
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelMeter)
};