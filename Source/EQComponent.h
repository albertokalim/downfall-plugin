/*
  ==============================================================================

    EQComponent.h
    Created: 9 Apr 2026 1:40:10pm
    Author:  Alberto

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "RotaryKnob.h"
#include "Parameters.h"
#include "BypassButton.h"

//==============================================================================
/*
*/
class EQComponent  : public juce::Component
{
public:
    EQComponent(parameters::Parameters& p);
    ~EQComponent() override;

    void resized() override;

private:
    juce::GroupComponent eq;

    RotaryKnob hpfKnob;
    RotaryKnob lpfKnob;
    BypassButton bypassButton;
    juce::Slider fq65Slider;
    juce::SliderParameterAttachment fq65listener;
    juce::Slider fq125Slider;
    juce::SliderParameterAttachment fq125listener;
    juce::Slider fq250Slider;
    juce::SliderParameterAttachment fq250listener;
    juce::Slider fq500Slider;
    juce::SliderParameterAttachment fq500listener;
    juce::Slider fq1kSlider;
    juce::SliderParameterAttachment fq1klistener;
    juce::Slider fq2kSlider;
    juce::SliderParameterAttachment fq2klistener;
    juce::Slider fq4kSlider;
    juce::SliderParameterAttachment fq4klistener;
    juce::Slider fq8kSlider;
    juce::SliderParameterAttachment fq8klistener;
    juce::Slider fq16kSlider;
    juce::SliderParameterAttachment fq16klistener;
    juce::Label fq65Label;
    juce::Label fq125Label;
    juce::Label fq250Label;
    juce::Label fq500Label;
    juce::Label fq1kLabel;
    juce::Label fq2kLabel;
    juce::Label fq4kLabel;
    juce::Label fq8kLabel;
    juce::Label fq16kLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EQComponent)
};
