/*
  ==============================================================================

    CabinetComponent.h
    Created: 9 Apr 2026 1:39:50pm
    Author:  Alberto

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class CabinetComponent  : public juce::Component
{
public:
    CabinetComponent();
    ~CabinetComponent() override;

    static CabinetComponent* get()
    {
        static CabinetComponent instance;
        return &instance;
    }

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Label text{ "Cabinet" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabinetComponent)
};
