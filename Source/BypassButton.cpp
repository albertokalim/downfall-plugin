/*
  ==============================================================================

    BypassButton.cpp
    Created: 10 Apr 2026 12:28:29pm
    Author:  Alberto

  ==============================================================================
*/

#include "BypassButton.h"
#include <numbers>

BypassButton::BypassButton(const juce::String& text, juce::AudioParameterBool& p)
    : button(text), listener(p, button)
{
    button.setLookAndFeel(BypassButtonLookAndFeel::get());
    addAndMakeVisible(button);
}

BypassButton::~BypassButton()
{
}

void BypassButton::resized()
{
    button.setBounds(0, 0, 40, 40);
}

BypassButtonLookAndFeel::BypassButtonLookAndFeel()
{

}

void BypassButtonLookAndFeel::drawToggleButton(juce::Graphics& g, juce::ToggleButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    auto bounds = g.getClipBounds().toFloat();
    auto shadowRect = bounds.reduced(2.0f, 2.0f);
    auto buttonRect = bounds.reduced(4.f, 4.f);
    auto path = juce::Path();
    path.addEllipse(shadowRect);
    dropShadow.colour = juce::Colours::darkgrey;
    dropShadow.drawForPath(g, path);
    g.setColour(button.getToggleState() ? juce::Colours::grey : juce::Colours::red);
    g.fillEllipse(buttonRect);
    
    auto center = buttonRect.getCentre();
    auto radius = buttonRect.getWidth() / 2.f;
    auto arcRadius = radius - 15.f / 2.f;
    float angleFromVerticalDegrees = 30.f;
    float angleFromVerticalRadians = angleFromVerticalDegrees * std::numbers::pi / 180.f;
    float startAngle = angleFromVerticalRadians;
    float endAngle = 2.f * std::numbers::pi - angleFromVerticalRadians;
    juce::Path innerCirclePath;
    innerCirclePath.addCentredArc(center.x, center.y,
        arcRadius, arcRadius,
        0.f,
        startAngle, endAngle,
        true);
    auto strokeType = juce::PathStrokeType(
        2.f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded);
    juce::Colour colour = juce::Colours::black.withAlpha(0.5f);
    g.setColour(colour);
    g.strokePath(innerCirclePath, strokeType);

    juce::Path linePath;
    float distanceFromCenter = 5.f;
    float lineSize = 5.f;
    juce::Line<float> buttonLine(center.x, center.y - distanceFromCenter, center.x, center.y - (distanceFromCenter + lineSize));
    linePath.addLineSegment(buttonLine, 1.f);
    g.setColour(colour);
    g.strokePath(linePath, strokeType);
}
