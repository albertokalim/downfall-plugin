/*
  ==============================================================================

    LevelMeter.cpp
    Created: 7 Apr 2026 7:46:57pm
    Author:  Alberto

  ==============================================================================
*/

#include "LevelMeter.h"
#include <JuceHeader.h>
#include "LevelMeter.h"

//==============================================================================
LevelMeter::LevelMeter(Measurement& measurementL_, Measurement& measurementR_)
    : measurementL(measurementL_), measurementR(measurementR_), dbLevelL(clampdB), dbLevelR(clampdB)
{
    setOpaque(true);
    startTimerHz(refreshRate);
    decay = 1.0f - std::exp(-1.0f / (float(refreshRate) * 0.2f));
}

LevelMeter::~LevelMeter()
{
}

void LevelMeter::paint(juce::Graphics& g)
{
    const auto bounds = getLocalBounds();
    g.fillAll(juce::Colours::black);
    drawLevel(g, dbLevelL, 0, 7);
    drawLevel(g, dbLevelR, 9, 7);

    for (float db = maxdB; db >= mindB; db -= stepdB) {
        int y = positionForLevel(db);
        g.setColour({ 200, 200, 200 });
        g.fillRect(0, y, 16, 1);
        g.setColour({ 80, 80, 80 });
        g.drawSingleLineText(juce::String(int(db)), bounds.getWidth(), y + 3,
            juce::Justification::right);
    }
}

void LevelMeter::resized()
{
    maxPos = 4.0f;
    minPos = float(getHeight()) - 4.0f;
}

void LevelMeter::timerCallback()
{
    updateLevel(measurementL.readAndReset(), levelL, dbLevelL);
    updateLevel(measurementR.readAndReset(), levelR, dbLevelR);

    repaint();
}

void LevelMeter::drawLevel(juce::Graphics& g, float level, int x, int width)
{
    int y = positionForLevel(level);

    if (level > 0.0f)
    {
        int y0 = positionForLevel(0.0f);
        g.setColour({ 226, 74, 81 });
        g.fillRect(x, y, width, y0 - y);
        g.setColour({ 65, 206, 88 });
        g.fillRect(x, y0, width, getHeight() - y0);
    }
    else if (y < getHeight())
    {
        g.setColour({ 65, 206, 88 });
        g.fillRect(x, y, width, getHeight() - y);
    }
}

void LevelMeter::updateLevel(float newLevel, float& smoothedLevel, float& leveldB) const
{
    if (newLevel > smoothedLevel) {
        smoothedLevel = newLevel; // instantaneous attack
    }
    else {
        smoothedLevel += (newLevel - smoothedLevel) * decay;
    }
    if (smoothedLevel > clampLevel) {
        leveldB = juce::Decibels::gainToDecibels(smoothedLevel);
    }
    else {
        leveldB = clampdB;
    }
}
