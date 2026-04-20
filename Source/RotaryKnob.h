/*
  ==============================================================================

    RotaryKnob.h
    Created: 7 Apr 2026 7:46:35pm
    Author:  Alberto

  ==============================================================================
*/

#pragma once
#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
namespace Colors {
    namespace Knob {
        const juce::Colour trackBackground{ 96, 96, 96 };
        const juce::Colour trackActive{ 127, 255, 212 };
        const juce::Colour outline{ 51, 255, 255 };
        const juce::Colour gradientTop{ 0, 0, 0 };
        const juce::Colour gradientBottom{ 96, 96, 96 };
        const juce::Colour dial{ 51, 255, 255 };
        const juce::Colour dropShadow{ 195, 190, 185 };
        const juce::Colour label{ 255, 255, 255 };
        const juce::Colour textBoxBackground{ 80, 80, 80 };
        const juce::Colour value{ 127, 255, 212 };
        const juce::Colour caret{ 255, 255, 255 };
    }
}

class RotaryKnobLookAndFeel : public juce::LookAndFeel_V4
{
public:
    static RotaryKnobLookAndFeel* get()
    {
        static RotaryKnobLookAndFeel instance;
        return &instance;
    }

    juce::Label* createSliderTextBox(juce::Slider&) override;
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
        float sliderPos, float rotaryStartAngle,
        float rotaryEndAngle, juce::Slider& slider) override;
    void drawTextEditorOutline(juce::Graphics&, int, int,
        juce::TextEditor&) override {
    }
    void fillTextEditorBackground(juce::Graphics&, int width, int height,
        juce::TextEditor&) override;

private:
    RotaryKnobLookAndFeel();


    juce::DropShadow dropShadow{ { 195, 190, 185 }, 6, { 0, 3 } };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RotaryKnobLookAndFeel)

};

class RotaryKnob : public juce::Component
{
public:
    RotaryKnob(const juce::String& text, juce::RangedAudioParameter& p, bool drawFromMiddle = false);
    ~RotaryKnob() override;

    void resized() override;

    juce::Slider slider;
    juce::Label label;

private:
    juce::SliderParameterAttachment listener;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RotaryKnob)
};