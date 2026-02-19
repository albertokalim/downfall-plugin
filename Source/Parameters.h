/*
  ==============================================================================

    Parameters.h
    Created: 19 Feb 2026 11:40:12am
    Author:  Alberto

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

constexpr auto versionHint = 1;

auto& addParameterToProcessor(juce::AudioProcessor& processor, auto parameter) {
    auto& parameterReference = *parameter;
    processor.addParameter(parameter.release());
    return parameterReference;
}

juce::AudioParameterFloat& createOutputGainParameter(juce::AudioProcessor& processor) {
    auto parameter = std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ "output.gain", versionHint },
        "Output gain",
        juce::NormalisableRange{ -12.f, 12.f, 0.1f, 0.4f },
        0.f,
        juce::AudioParameterFloatAttributes{}.withLabel("dB"));
    return addParameterToProcessor(processor, std::move(parameter));
}

struct Parameters {
    explicit Parameters(juce::AudioProcessor& audioProcessor) 
        : outputGain(createOutputGainParameter(audioProcessor))
    {
    }

    juce::AudioParameterFloat& outputGain;

    JUCE_DECLARE_NON_COPYABLE(Parameters);
    JUCE_DECLARE_NON_MOVEABLE(Parameters);
};