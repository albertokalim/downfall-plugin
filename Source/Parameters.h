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

    static juce::AudioParameterBool& createParameterBool(juce::AudioProcessor& processor, juce::String parameterID, juce::String parameterName,
        bool initialValue) {
        auto parameter = std::make_unique<juce::AudioParameterBool>(
            juce::ParameterID{ parameterID, versionHint },
            parameterName,
            initialValue);
        return addParameterToProcessor(processor, std::move(parameter));
    }

    static juce::AudioParameterChoice& createParameterChoice(juce::AudioProcessor& processor, juce::String parameterID, juce::String parameterName, 
        juce::StringArray values, int initialValueIndex) {
        auto parameter = std::make_unique<juce::AudioParameterChoice>(
            juce::ParameterID{ parameterID, versionHint },
            parameterName,
            values,
            initialValueIndex
        );
        return addParameterToProcessor(processor, std::move(parameter));
    }

    class GlobalParameters {
    public:
        explicit GlobalParameters(juce::AudioProcessor& audioProcessor)
            : outputGain(createParameterFloat(audioProcessor,
                "output.gain",
                "Output gain",
                juce::NormalisableRange{ -24.f, 24.f },
                0.f,
                juce::AudioParameterFloatAttributes{}.withLabel("dB"))),
            gateThreshold(createParameterFloat(audioProcessor,
                "gate.threshold",
                "Gate Threshold",
                juce::NormalisableRange{ -96.f, 0.f },
                -96.f,
                juce::AudioParameterFloatAttributes{}.withLabel("dB"))),
            inputGain(createParameterFloat(audioProcessor,
                "input.gain",
                "Input Gain",
                juce::NormalisableRange{ -24.f, 24.f },
                0.f,
                juce::AudioParameterFloatAttributes{}.withLabel("dB"))),
            bypassCabinet(createParameterBool(audioProcessor,
                "bypassCabinet",
                "Bypass Cabinet",
                false)),
            bypassPreamp(createParameterBool(audioProcessor,
                "bypassPreamp",
                "Bypass PreAmp",
                false))
        {
        }

        juce::AudioParameterFloat& getInputGain() { return inputGain; }
        juce::AudioParameterFloat& getOutputGain() { return outputGain; }
        juce::AudioParameterFloat& getGateThreshold() { return gateThreshold; }
        juce::AudioParameterBool& getBypassCabinet() { return bypassCabinet; }
        juce::AudioParameterBool& getBypassPreamp() { return bypassPreamp; }

    private:

        juce::AudioParameterFloat& inputGain;
        juce::AudioParameterFloat& outputGain;
        juce::AudioParameterFloat& gateThreshold;
        juce::AudioParameterBool& bypassCabinet;
        juce::AudioParameterBool& bypassPreamp;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GlobalParameters)
    };

    class PreAmpParameters {
    public:
        explicit PreAmpParameters(juce::AudioProcessor& audioProcessor) 
            : gain(createParameterFloat(audioProcessor, 
                "gain", 
                "Gain", 
                juce::NormalisableRange{ 0.f, 100.f },
                50.f,
                juce::AudioParameterFloatAttributes{}.withLabel("%"))),
            bass(createParameterFloat(audioProcessor,
                "bass",
                "Bass",
                juce::NormalisableRange{ 0.f, 100.f },
                50.f,
                juce::AudioParameterFloatAttributes{}.withLabel("%"))),
            middle(createParameterFloat(audioProcessor,
                "middle",
                "Middle",
                juce::NormalisableRange{ 0.f, 100.f },
                50.f,
                juce::AudioParameterFloatAttributes{}.withLabel("%"))),
            treble(createParameterFloat(audioProcessor,
                "treble",
                "Treble",
                juce::NormalisableRange{ 0.f, 100.f },
                50.f,
                juce::AudioParameterFloatAttributes{}.withLabel("%"))),
            presence(createParameterFloat(audioProcessor,
                "high-gain-presence",
                "High Gain Presence",
                juce::NormalisableRange{ 0.f, 100.f },
                50.f,
                juce::AudioParameterFloatAttributes{}.withLabel("%"))),
            master(createParameterFloat(audioProcessor,
                "master",
                "Master",
                juce::NormalisableRange{ 0.f, 100.f },
                50.f,
                juce::AudioParameterFloatAttributes{}.withLabel("%"))),
            ampType(createParameterChoice(audioProcessor,
                "ampType",
                "Amp Type",
                juce::StringArray {"Clean", "HighGain"},
                0))
        {}

        juce::AudioParameterFloat& getGain() { return gain; }
        juce::AudioParameterFloat& getBass() { return bass; }
        juce::AudioParameterFloat& getMiddle() { return middle; }
        juce::AudioParameterFloat& getTreble() { return treble; }
        juce::AudioParameterFloat& getPresence() { return presence; }
        juce::AudioParameterFloat& getMaster() { return master; }
        juce::AudioParameterChoice& getAmpType() { return ampType; }

    private:
        juce::AudioParameterFloat& gain;
        juce::AudioParameterFloat& bass;
        juce::AudioParameterFloat& middle;
        juce::AudioParameterFloat& treble;
        juce::AudioParameterFloat& presence;
        juce::AudioParameterFloat& master;
        juce::AudioParameterChoice& ampType;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PreAmpParameters)
    };

    class FXParameters {
    public:
        explicit FXParameters(juce::AudioProcessor& audioProcessor, juce::String effectName)
            : bypassEffect(createParameterBool(audioProcessor,
                effectName << "bypass",
                effectName << " Bypass",
                false))
        {}


        juce::AudioParameterBool& getBypassEffect() { return bypassEffect; }

    protected:
        juce::AudioParameterBool& bypassEffect;
    };

};