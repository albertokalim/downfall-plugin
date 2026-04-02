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
    static void serializeParameters(parameters::Parameters&, juce::OutputStream&);

    /** @return Error message on failure; empty string otherwise.
     *           In case of error, no parameters are updated. */
    static juce::Result deserializeParameters(juce::InputStream&, parameters::Parameters&);
};