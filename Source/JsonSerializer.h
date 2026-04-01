/*
  ==============================================================================

    JsonSerializer.h
    Created: 1 Apr 2026 9:32:54am
    Author:  Alberto

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Parameters.h"

class JsonSerializer {
public:
    static void serializeGlobalParameters(parameters::GlobalParameters&, juce::OutputStream&);
    static void serializePreAmpParameters(parameters::PreAmpParameters&, juce::OutputStream&);
    static void serializeDelayParameters(parameters::DelayParameters&, juce::OutputStream&);
    static void serializeChorusParameters(parameters::ChorusParameters&, juce::OutputStream&);
    static void serializeReverbParameters(parameters::ReverbParameters&, juce::OutputStream&);

    /** @return Error message on failure; empty string otherwise.
     *           In case of error, no parameters are updated. */
    static juce::Result deserializeGlobalParameters(juce::InputStream&, parameters::GlobalParameters&);
    static juce::Result deserializePreAmpParameters(juce::InputStream&, parameters::PreAmpParameters&);
    static juce::Result deserializeDelayParameters(juce::InputStream&, parameters::DelayParameters&);
    static juce::Result deserializeChorusParameters(juce::InputStream&, parameters::ChorusParameters&);
    static juce::Result deserializeReverbParameters(juce::InputStream&, parameters::ReverbParameters&);
};