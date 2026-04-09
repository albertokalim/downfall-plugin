/*
  ==============================================================================

    AmpComponent.h
    Created: 9 Apr 2026 1:38:34pm
    Author:  Alberto

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class AmpComponent  : public juce::Component
{
public:
    AmpComponent();
    ~AmpComponent() override;

    static AmpComponent* get()
    {
        static AmpComponent instance;
        return &instance;
    }

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Label text{"Amp"};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmpComponent)
};
