/*
  ==============================================================================

    CabinetComponent.h
    Created: 9 Apr 2026 1:39:50pm
    Author:  Alberto

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <functional>

//==============================================================================
/*
*/
class CabinetComponent  : public juce::Component
{
public:
    CabinetComponent();
    ~CabinetComponent() override;

    void resized() override;
    void addListener(juce::ChangeListener&);

private:
    juce::Label text{ "Default IR" };
    juce::TextButton buttonIRLoader{ "Load custom IR" };
    juce::FileChooser fileChooser{ "Select an IR file to load", juce::File::getSpecialLocation(juce::File::userHomeDirectory), "*.wav" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabinetComponent)
};
