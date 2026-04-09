/*
  ==============================================================================

    EQComponent.h
    Created: 9 Apr 2026 1:40:10pm
    Author:  Alberto

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class EQComponent  : public juce::Component
{
public:
    EQComponent();
    ~EQComponent() override;

    static EQComponent* get()
    {
        static EQComponent instance;
        return &instance;
    }

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Label text{ "EQ" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EQComponent)
};
