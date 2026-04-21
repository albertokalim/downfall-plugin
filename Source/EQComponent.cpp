/*
  ==============================================================================

    EQComponent.cpp
    Created: 9 Apr 2026 1:40:10pm
    Author:  Alberto

  ==============================================================================
*/

#include <JuceHeader.h>
#include "EQComponent.h"

//==============================================================================
EQComponent::EQComponent(parameters::Parameters& p)
    : hpfKnob("HPF", p.hpfFq),
    lpfKnob("LPF", p.lpfFq),
    bypassButton("Bypass EQ", p.bypassEq),
    fq65listener(p.eq65hzGain, fq65Slider),
    fq125listener(p.eq125hzGain, fq125Slider),
    fq250listener(p.eq250hzGain, fq250Slider),
    fq500listener(p.eq500hzGain, fq500Slider),
    fq1klistener(p.eq1khzGain, fq1kSlider),
    fq2klistener(p.eq2khzGain, fq2kSlider),
    fq4klistener(p.eq4khzGain, fq4kSlider),
    fq8klistener(p.eq8khzGain, fq8kSlider),
    fq16klistener(p.eq16khzGain, fq16kSlider)
{
    int labelWidth = 100;
    int labelHeight = 50;

    fq65Slider.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::black);
    fq65Slider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    fq65Slider.setTextBoxStyle(juce::Slider::TextBoxAbove, true, labelWidth, labelHeight);

    fq125Slider.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::black);
    fq125Slider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    fq125Slider.setTextBoxStyle(juce::Slider::TextBoxAbove, true, labelWidth, labelHeight);

    fq250Slider.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::black);
    fq250Slider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    fq250Slider.setTextBoxStyle(juce::Slider::TextBoxAbove, true, labelWidth, labelHeight);

    fq500Slider.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::black);
    fq500Slider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    fq500Slider.setTextBoxStyle(juce::Slider::TextBoxAbove, true, labelWidth, labelHeight);

    fq1kSlider.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::black);
    fq1kSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    fq1kSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, true, labelWidth, labelHeight);

    fq2kSlider.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::black);
    fq2kSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    fq2kSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, true, labelWidth, labelHeight);

    fq4kSlider.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::black);
    fq4kSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    fq4kSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, true, labelWidth, labelHeight);

    fq8kSlider.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::black);
    fq8kSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    fq8kSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, true, labelWidth, labelHeight);

    fq16kSlider.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::black);
    fq16kSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    fq16kSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, true, labelWidth, labelHeight);

    fq65Label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    fq65Label.setText("65 Hz", juce::NotificationType::dontSendNotification);
    fq65Label.setBorderSize(juce::BorderSize<int>{ 0, 0, 0, 0 });

    fq125Label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    fq125Label.setText("125 Hz", juce::NotificationType::dontSendNotification);
    fq125Label.setBorderSize(juce::BorderSize<int>{ 0, 0, 0, 0 });

    fq250Label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    fq250Label.setText("250 Hz", juce::NotificationType::dontSendNotification);
    fq250Label.setBorderSize(juce::BorderSize<int>{ 0, 0, 0, 0 });

    fq500Label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    fq500Label.setText("500 Hz", juce::NotificationType::dontSendNotification);
    fq500Label.setBorderSize(juce::BorderSize<int>{ 0, 0, 0, 0 });

    fq1kLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    fq1kLabel.setText("1k Hz", juce::NotificationType::dontSendNotification);
    fq1kLabel.setBorderSize(juce::BorderSize<int>{ 0, 0, 0, 0 });

    fq2kLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    fq2kLabel.setText("2k Hz", juce::NotificationType::dontSendNotification);
    fq2kLabel.setBorderSize(juce::BorderSize<int>{ 0, 0, 0, 0 });

    fq4kLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    fq4kLabel.setText("4k Hz", juce::NotificationType::dontSendNotification);
    fq4kLabel.setBorderSize(juce::BorderSize<int>{ 0, 0, 0, 0 });

    fq8kLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    fq8kLabel.setText("8k Hz", juce::NotificationType::dontSendNotification);
    fq8kLabel.setBorderSize(juce::BorderSize<int>{ 0, 0, 0, 0 });

    fq16kLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    fq16kLabel.setText("16k Hz", juce::NotificationType::dontSendNotification);
    fq16kLabel.setBorderSize(juce::BorderSize<int>{ 0, 0, 0, 0 });

    eq.addAndMakeVisible(bypassButton);
    eq.addAndMakeVisible(hpfKnob);
    eq.addAndMakeVisible(lpfKnob);
    eq.addAndMakeVisible(fq65Slider);
    eq.addAndMakeVisible(fq125Slider);
    eq.addAndMakeVisible(fq250Slider);
    eq.addAndMakeVisible(fq500Slider);
    eq.addAndMakeVisible(fq1kSlider);
    eq.addAndMakeVisible(fq2kSlider);
    eq.addAndMakeVisible(fq4kSlider);
    eq.addAndMakeVisible(fq8kSlider);
    eq.addAndMakeVisible(fq16kSlider);
    eq.addAndMakeVisible(fq65Label);
    eq.addAndMakeVisible(fq125Label);
    eq.addAndMakeVisible(fq250Label);
    eq.addAndMakeVisible(fq500Label);
    eq.addAndMakeVisible(fq1kLabel);
    eq.addAndMakeVisible(fq2kLabel);
    eq.addAndMakeVisible(fq4kLabel);
    eq.addAndMakeVisible(fq8kLabel);
    eq.addAndMakeVisible(fq16kLabel);
    eq.setColour(juce::GroupComponent::ColourIds::outlineColourId, juce::Colours::white.withAlpha(0.3f));
    addAndMakeVisible(eq);
}

EQComponent::~EQComponent()
{
}

void EQComponent::resized()
{
    auto bounds = getLocalBounds();
    int heightTenPerc = std::ceil(bounds.getHeight() * 0.2);
    int width = bounds.getWidth();
    int height = bounds.getHeight() - heightTenPerc;
    eq.setBounds(0, heightTenPerc / 2, width, height);

    bypassButton.setBounds(0, 0, 100, 100);
    bypassButton.setTopLeftPosition(60, 50);

    int sliderHeight = 300;
    int sliderWidth = 75;
    int labelHeight = 50;
    int partition = width / 11;
    int paddingLeft = 30;
    int yCoordSliders = 60;
    int yCoordLabels = 350;
    int labelHorizontalOffset = 18;
    
    hpfKnob.setTopLeftPosition(paddingLeft, 150);
    fq65Slider.setBounds(paddingLeft + partition, yCoordSliders, sliderWidth, sliderHeight);
    fq125Slider.setBounds(paddingLeft + 2 * partition, yCoordSliders, sliderWidth, sliderHeight);
    fq250Slider.setBounds(paddingLeft + 3 * partition, yCoordSliders, sliderWidth, sliderHeight);
    fq500Slider.setBounds(paddingLeft + 4 * partition, yCoordSliders, sliderWidth, sliderHeight);
    fq1kSlider.setBounds(paddingLeft + 5 * partition, yCoordSliders, sliderWidth, sliderHeight);
    fq2kSlider.setBounds(paddingLeft + 6 * partition, yCoordSliders, sliderWidth, sliderHeight);
    fq4kSlider.setBounds(paddingLeft + 7 * partition, yCoordSliders, sliderWidth, sliderHeight);
    fq8kSlider.setBounds(paddingLeft + 8 * partition, yCoordSliders, sliderWidth, sliderHeight);
    fq16kSlider.setBounds(paddingLeft + 9 * partition, yCoordSliders, sliderWidth, sliderHeight);

    fq65Label.setBounds(labelHorizontalOffset + paddingLeft + partition, yCoordLabels, sliderWidth, labelHeight);
    fq125Label.setBounds(labelHorizontalOffset + paddingLeft + 2 * partition, yCoordLabels, sliderWidth, labelHeight);
    fq250Label.setBounds(labelHorizontalOffset + paddingLeft + 3 * partition, yCoordLabels, sliderWidth, labelHeight);
    fq500Label.setBounds(labelHorizontalOffset + paddingLeft + 4 * partition, yCoordLabels, sliderWidth, labelHeight);
    fq1kLabel.setBounds(labelHorizontalOffset + paddingLeft + 5 * partition, yCoordLabels, sliderWidth, labelHeight);
    fq2kLabel.setBounds(labelHorizontalOffset + paddingLeft + 6 * partition, yCoordLabels, sliderWidth, labelHeight);
    fq4kLabel.setBounds(labelHorizontalOffset + paddingLeft + 7 * partition, yCoordLabels, sliderWidth, labelHeight);
    fq8kLabel.setBounds(labelHorizontalOffset + paddingLeft + 8 * partition, yCoordLabels, sliderWidth, labelHeight);
    fq16kLabel.setBounds(labelHorizontalOffset + paddingLeft + 9 * partition, yCoordLabels, sliderWidth, labelHeight);
    lpfKnob.setTopLeftPosition(paddingLeft + 10 * partition, 150);
}
