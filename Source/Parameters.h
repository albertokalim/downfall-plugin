/*
  ==============================================================================

    Parameters.h
    Created: 19 Feb 2026 11:40:12am
    Author:  Alberto

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

static constexpr float MAX_DELAY_TIME = 5000.0f;
static constexpr float MIN_DELAY_TIME = 5.0f;
static const int versionHint = 1;

namespace parameters {
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
        GlobalParameters(juce::AudioProcessor& audioProcessor)
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
        PreAmpParameters(juce::AudioProcessor& audioProcessor)
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
                juce::StringArray{ "Clean", "HighGain" },
                0))
        {
        }

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
        FXParameters(juce::AudioProcessor& audioProcessor, juce::String effectName)
            : bypassEffect(createParameterBool(audioProcessor,
                effectName << "Bypass",
                effectName << " Bypass",
                true))
        {
        }
        virtual ~FXParameters() {}


        juce::AudioParameterBool& getBypassEffect() { return bypassEffect; }

    protected:
        juce::AudioParameterBool& bypassEffect;
    };

    class DelayParameters : public FXParameters {
    public:
        DelayParameters(juce::AudioProcessor& audioProcessor)
            : FXParameters(audioProcessor, "delay"),
            delayTime(createParameterFloat(audioProcessor,
                "delayTime",
                "Delay Time",
                juce::NormalisableRange<float> { MIN_DELAY_TIME, MAX_DELAY_TIME, 0.001f, 0.25f },
                100.0f,
                juce::AudioParameterFloatAttributes{}.withLabel("ms"))),
            mix(createParameterFloat(audioProcessor,
                "delayMix",
                "Delay Mix",
                juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
                100.0f,
                juce::AudioParameterFloatAttributes{}.withLabel("%"))),
            feedback(createParameterFloat(audioProcessor,
                "delayFeedback",
                "Delay Feedback",
                juce::NormalisableRange<float>(0.f, 100.0f, 1.0f),
                0.0f,
                juce::AudioParameterFloatAttributes{}.withLabel("%"))),
            sync(createParameterBool(audioProcessor,
                "delaySync",
                "Delay Sync",
                false)),
            delayNote(createParameterChoice(audioProcessor,
                "delayNote",
                "Delay Note",
                juce::StringArray{
                    "1/32",
                    "1/16 trip",
                    "1/32 dot",
                    "1/16",
                    "1/8 trip",
                    "1/16 dot",
                    "1/8",
                    "1/4 trip",
                    "1/8 dot",
                    "1/4",
                    "1/2 trip",
                    "1/4 dot",
                    "1/2",
                    "1/1 trip",
                    "1/2 dot",
                    "1/1",
                },
                9))
        {
        }
        ~DelayParameters() {}

        juce::AudioParameterFloat& getDelayTime() { return delayTime; }
        juce::AudioParameterFloat& getMix() { return mix; }
        juce::AudioParameterFloat& getFeedback() { return feedback; }
        juce::AudioParameterBool& getSync() { return sync; }
        juce::AudioParameterChoice& getDelayNote() { return delayNote; }

    private:
        juce::AudioParameterFloat& delayTime;
        juce::AudioParameterFloat& mix;
        juce::AudioParameterFloat& feedback;
        juce::AudioParameterBool& sync;
        juce::AudioParameterChoice& delayNote;
    };

    class ChorusParameters : public FXParameters {
    public:
        ChorusParameters(juce::AudioProcessor& audioProcessor) : 
            FXParameters(audioProcessor, "chorus"),
            rate(createParameterFloat(audioProcessor,
                "chorusRate",
                "Chorus Rate",
                juce::NormalisableRange<float>(0.1f, 20.0f, 0.1f),
                5.f,
                juce::AudioParameterFloatAttributes{}.withLabel("Hz"))),
            width(createParameterFloat(audioProcessor,
                "chorusWidth",
                "Chorus Width",
                juce::NormalisableRange<float>(0.f, 100.f, 0.1f),
                40.f,
                juce::AudioParameterFloatAttributes{}.withLabel("%"))),
            mix(createParameterFloat(audioProcessor,
                "chorusMix",
                "Chorus Mix",
                juce::NormalisableRange<float>(0.f, 100.0f, 1.0f),
                50.f,
                juce::AudioParameterFloatAttributes{}.withLabel("%")))
        {}

        juce::AudioParameterFloat& getRate() { return rate; }
        juce::AudioParameterFloat& getWidth() { return width; }
        juce::AudioParameterFloat& getMix() { return mix; }

    private:
        juce::AudioParameterFloat& rate;
        juce::AudioParameterFloat& width;
        juce::AudioParameterFloat& mix;
    };
};