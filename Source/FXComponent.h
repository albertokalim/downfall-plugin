/*
  ==============================================================================

    FXComponent.h
    Created: 9 Apr 2026 1:39:13pm
    Author:  Alberto

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class FXComponent  : public juce::Component
{
public:
    FXComponent();
    ~FXComponent() override;

    static FXComponent* get()
    {
        static FXComponent instance;
        return &instance;
    }

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Label text{ "FX" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FXComponent)
};
