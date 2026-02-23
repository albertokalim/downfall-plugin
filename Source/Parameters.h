/*
  ==============================================================================

    Parameters.h
    Created: 19 Feb 2026 11:40:12am
    Author:  Alberto

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
namespace parameters {
    static const int versionHint = 1;

    static auto& addParameterToProcessor(juce::AudioProcessor& processor, auto parameter) {
        auto& parameterReference = *parameter;
        processor.addParameter(parameter.release());
        return parameterReference;
    }

    static juce::AudioParameterFloat& createParameterFloat(juce::AudioProcessor& processor, juce::String parameterID, juce::String parameterName,
        juce::NormalisableRange<float> normalisableRange, float initialValue, juce::AudioParameterFloatAttributes attributes) {
        auto parameter = std::make_unique<juce::AudioParameterFloat>(
            juce::ParameterID{ parameterID, versionHint },
            parameterName,
            normalisableRange,
            initialValue,
            attributes);
        return addParameterToProcessor(processor, std::move(parameter));
    }

    class GlobalParameters {
    public:
        explicit GlobalParameters(juce::AudioProcessor& audioProcessor)
            : outputGain(createParameterFloat(audioProcessor,
                "output.gain",
                "Output gain",
                juce::NormalisableRange{ -12.f, 12.f, 0.1f, 0.4f },
                0.f,
                juce::AudioParameterFloatAttributes{}.withLabel("dB"))),
            gateThreshold(createParameterFloat(audioProcessor,
                "gate.threshold",
                "Gate Threshold",
                juce::NormalisableRange{ -96.f, 0.f, 0.1f, 0.4f },
                -96.f,
                juce::AudioParameterFloatAttributes{}.withLabel("dB"))),
            inputGain(createParameterFloat(audioProcessor,
                "input.gain",
                "Input Gain",
                juce::NormalisableRange{ -12.f, 12.f, 0.1f, 0.4f },
                0.f,
                juce::AudioParameterFloatAttributes{}.withLabel("dB")))
        {
        }

        juce::AudioParameterFloat& getInputGain() { return inputGain; }
        juce::AudioParameterFloat& getOutputGain() { return outputGain; }
        juce::AudioParameterFloat& getGateThreshold() { return gateThreshold; }

    private:

        juce::AudioParameterFloat& inputGain;
        juce::AudioParameterFloat& outputGain;
        juce::AudioParameterFloat& gateThreshold;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GlobalParameters)
    };

    class PreAmpParameters {
    public:
        explicit PreAmpParameters(juce::AudioProcessor& audioProcessor) 
            : gain(createParameterFloat(audioProcessor, 
                "gain", 
                "Gain", 
                juce::NormalisableRange{ 0.f, 100.f, 0.1f, 0.4f },
                50.f,
                juce::AudioParameterFloatAttributes{}.withLabel("%")))
        {}

        juce::AudioParameterFloat& getGain() { return gain; };

    private:
        juce::AudioParameterFloat& gain;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PreAmpParameters)
    };

};