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
static constexpr auto MARSHALLING_VERSION = 1;

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

    class Parameters {
    public:
        Parameters(juce::AudioProcessor& audioProcessor)
            : outputGain(createParameterFloat(audioProcessor,
                "output.gain",
                "Output gain",
                juce::NormalisableRange{ -24.f, 24.f, 0.01f },
                0.f,
                juce::AudioParameterFloatAttributes{}.withLabel("dB"))),
            gateThreshold(createParameterFloat(audioProcessor,
                "gate.threshold",
                "Gate Threshold",
                juce::NormalisableRange{ -96.f, 0.f, 0.01f },
                -96.f,
                juce::AudioParameterFloatAttributes{}.withLabel("dB"))),
            inputGain(createParameterFloat(audioProcessor,
                "input.gain",
                "Input Gain",
                juce::NormalisableRange{ -24.f, 24.f, 0.01f },
                0.f,
                juce::AudioParameterFloatAttributes{}.withLabel("dB"))),
            bypassCabinet(createParameterBool(audioProcessor,
                "bypassCabinet",
                "Bypass Cabinet",
                false)),
            bypassPreamp(createParameterBool(audioProcessor,
                "bypassPreamp",
                "Bypass PreAmp",
                false)),
            gain(createParameterFloat(audioProcessor,
                "gain",
                "Gain",
                juce::NormalisableRange{ 0.f, 100.f, 1.0f },
                50.f,
                juce::AudioParameterFloatAttributes{}.withLabel("%"))),
            bass(createParameterFloat(audioProcessor,
                "bass",
                "Bass",
                juce::NormalisableRange{ 0.f, 100.f, 1.0f },
                50.f,
                juce::AudioParameterFloatAttributes{}.withLabel("%"))),
            middle(createParameterFloat(audioProcessor,
                "middle",
                "Middle",
                juce::NormalisableRange{ 0.f, 100.f, 1.0f },
                50.f,
                juce::AudioParameterFloatAttributes{}.withLabel("%"))),
            treble(createParameterFloat(audioProcessor,
                "treble",
                "Treble",
                juce::NormalisableRange{ 0.f, 100.f, 1.0f },
                50.f,
                juce::AudioParameterFloatAttributes{}.withLabel("%"))),
            presence(createParameterFloat(audioProcessor,
                "high-gain-presence",
                "High Gain Presence",
                juce::NormalisableRange{ 0.f, 100.f, 1.0f },
                50.f,
                juce::AudioParameterFloatAttributes{}.withLabel("%"))),
            master(createParameterFloat(audioProcessor,
                "master",
                "Master",
                juce::NormalisableRange{ 0.f, 100.f, 1.0f },
                50.f,
                juce::AudioParameterFloatAttributes{}.withLabel("%"))),
            ampType(createParameterChoice(audioProcessor,
                "ampType",
                "Amp Type",
                juce::StringArray{ "Clean", "HighGain" },
                0)),
            delayTime(createParameterFloat(audioProcessor,
                "delayTime",
                "Delay Time",
                juce::NormalisableRange<float> { MIN_DELAY_TIME, MAX_DELAY_TIME, 0.001f, 0.25f },
                100.0f,
                juce::AudioParameterFloatAttributes{}.withLabel("ms"))),
            delayMix(createParameterFloat(audioProcessor,
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
                9)),
            rate(createParameterFloat(audioProcessor,
                "chorusRate",
                "Chorus Rate",
                juce::NormalisableRange<float>(0.1f, 7.0f, 0.1f), 
                1.f,
                juce::AudioParameterFloatAttributes{}.withLabel("Hz"))),
            width(createParameterFloat(audioProcessor,
                "chorusWidth",
                "Chorus Width",
                juce::NormalisableRange<float>(0.f, 100.f, 0.1f),
                40.f,
                juce::AudioParameterFloatAttributes{}.withLabel("%"))),
            chorusMix(createParameterFloat(audioProcessor,
                "chorusMix",
                "Chorus Mix",
                juce::NormalisableRange<float>(0.f, 100.0f, 1.0f),
                50.f,
                juce::AudioParameterFloatAttributes{}.withLabel("%"))),
            decay(createParameterFloat(audioProcessor,
                "reverbDecay",
                "Reverb Decay",
                juce::NormalisableRange<float>(0.f, 100.0f, 1.0f),
                0.0f,
                juce::AudioParameterFloatAttributes{}.withLabel("%"))),
            reverbMix(createParameterFloat(audioProcessor,
                "reverbMix",
                "Reverb Mix",
                juce::NormalisableRange<float>(0.f, 100.0f, 1.0f),
                50.f,
                juce::AudioParameterFloatAttributes{}.withLabel("%"))),
            delayBypass(createParameterBool(audioProcessor,
                "delayBypass",
                "Delay Bypass",
                true)),
            chorusBypass(createParameterBool(audioProcessor,
                "chorusBypass",
                "Chorus Bypass",
                true)),
            reverbBypass(createParameterBool(audioProcessor,
                "reverbBypass",
                "Reverb Bypass",
                true)),
            bypass(createParameterBool(audioProcessor,
                "bypass",
                "Bypass",
                false)),
            bypassEq(createParameterBool(audioProcessor,
                "bypassEq",
                "Bypass EQ",
                true)),
            hpfFq(createParameterFloat(audioProcessor,
                "hpfFq",
                "High Pass FQ",
                juce::NormalisableRange<float>(0.0f, 500.f, 1.f),
                0.f,
                juce::AudioParameterFloatAttributes{}.withLabel("Hz"))),
            lpfFq(createParameterFloat(audioProcessor,
                "lpfFq",
                "Low Pass FQ",
                juce::NormalisableRange<float>(0.0f, 500.f, 1.f),
                0.f,
                juce::AudioParameterFloatAttributes{}.withLabel("Hz"))),
            eq65hzGain(createParameterFloat(audioProcessor,
                "eq65hzGain",
                "65Hz",
                juce::NormalisableRange{ -12.f, 12.f, 0.1f },
                0.f,
                juce::AudioParameterFloatAttributes{}.withLabel("dB"))),
            eq125hzGain(createParameterFloat(audioProcessor,
                "eq125hzGain",
                "125Hz",
                juce::NormalisableRange{ -12.f, 12.f, 0.1f },
                0.f,
                juce::AudioParameterFloatAttributes{}.withLabel("dB"))),
            eq250hzGain(createParameterFloat(audioProcessor,
                "eq250hzGain",
                "250Hz",
                juce::NormalisableRange{ -12.f, 12.f, 0.1f },
                0.f,
                juce::AudioParameterFloatAttributes{}.withLabel("dB"))),
            eq500hzGain(createParameterFloat(audioProcessor,
                "eq500hzGain",
                "500Hz",
                juce::NormalisableRange{ -12.f, 12.f, 0.1f },
                0.f,
                juce::AudioParameterFloatAttributes{}.withLabel("dB"))),
            eq1khzGain(createParameterFloat(audioProcessor,
                "eq1khzGain",
                "1kHz",
                juce::NormalisableRange{ -12.f, 12.f, 0.1f },
                0.f,
                juce::AudioParameterFloatAttributes{}.withLabel("dB"))),
            eq2khzGain(createParameterFloat(audioProcessor,
                "eq2khzGain",
                "2kHz",
                juce::NormalisableRange{ -12.f, 12.f, 0.1f },
                0.f,
                juce::AudioParameterFloatAttributes{}.withLabel("dB"))),
            eq4khzGain(createParameterFloat(audioProcessor,
                "eq4khzGain",
                "4kHz",
                juce::NormalisableRange{ -12.f, 12.f, 0.1f },
                0.f,
                juce::AudioParameterFloatAttributes{}.withLabel("dB"))),
            eq8khzGain(createParameterFloat(audioProcessor,
                "eq8khzGain",
                "8kHz",
                juce::NormalisableRange{ -12.f, 12.f, 0.1f },
                0.f,
                juce::AudioParameterFloatAttributes{}.withLabel("dB"))),
            eq16khzGain(createParameterFloat(audioProcessor,
                "eq16khzGain",
                "16kHz",
                juce::NormalisableRange{ -12.f, 12.f, 0.1f },
                0.f,
                juce::AudioParameterFloatAttributes{}.withLabel("dB")))
        {
        }

        juce::AudioParameterFloat& inputGain;
        juce::AudioParameterFloat& outputGain;
        juce::AudioParameterFloat& gateThreshold;
        juce::AudioParameterBool& bypassCabinet;
        juce::AudioParameterBool& bypassPreamp;
        juce::AudioParameterFloat& gain;
        juce::AudioParameterFloat& bass;
        juce::AudioParameterFloat& middle;
        juce::AudioParameterFloat& treble;
        juce::AudioParameterFloat& presence;
        juce::AudioParameterFloat& master;
        juce::AudioParameterChoice& ampType;
        juce::AudioParameterFloat& delayTime;
        juce::AudioParameterFloat& delayMix;
        juce::AudioParameterFloat& feedback;
        juce::AudioParameterBool& sync;
        juce::AudioParameterChoice& delayNote;
        juce::AudioParameterFloat& rate;
        juce::AudioParameterFloat& width;
        juce::AudioParameterFloat& chorusMix;
        juce::AudioParameterFloat& decay;
        juce::AudioParameterFloat& reverbMix;
        juce::AudioParameterBool& delayBypass;
        juce::AudioParameterBool& chorusBypass;
        juce::AudioParameterBool& reverbBypass;
        juce::AudioParameterBool& bypass;
        juce::AudioParameterFloat& hpfFq;
        juce::AudioParameterFloat& lpfFq;
        juce::AudioParameterFloat& eq65hzGain;
        juce::AudioParameterFloat& eq125hzGain;
        juce::AudioParameterFloat& eq250hzGain;
        juce::AudioParameterFloat& eq500hzGain;
        juce::AudioParameterFloat& eq1khzGain;
        juce::AudioParameterFloat& eq2khzGain;
        juce::AudioParameterFloat& eq4khzGain;
        juce::AudioParameterFloat& eq8khzGain;
        juce::AudioParameterFloat& eq16khzGain;
        juce::AudioParameterBool& bypassEq;

    private:

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Parameters)
    };

    struct SerializableParameters {
        float inputGain;
        float outputGain;
        float gateThreshold;
        bool bypassCabinet;
        bool bypassPreamp;
        float gain;
        float bass;
        float middle;
        float treble;
        float presence;
        float master;
        juce::String ampType;
        bool delayBypass;
        float delayTime;
        float delayMix;
        float feedback;
        bool sync;
        juce::String delayNote;
        bool chorusBypass;
        float rate;
        float width;
        float chorusMix;
        bool reverbBypass;
        float decay;
        float reverbMix;
        bool bypass;
        float hpfFq;
        float lpfFq;
        float eq65hzGain;
        float eq125hzGain;
        float eq250hzGain;
        float eq500hzGain;
        float eq1khzGain;
        float eq2khzGain;
        float eq4khzGain;
        float eq8khzGain;
        float eq16khzGain;
        bool bypassEq;

        static constexpr auto marshallingVersion = MARSHALLING_VERSION;

        template <typename Archive, typename T>
        static void serialise(Archive& archive, T& t) {
            using namespace juce;
            if (archive.getVersion() != 1) {
                return;
            }

            std::string pluginName = JucePlugin_Name;
            archive(named("pluginName", pluginName));
            if (pluginName != JucePlugin_Name) {
                return;
            }

            archive(named("output.gain", t.outputGain),
                named("gate.threshold", t.gateThreshold),
                named("input.gain", t.inputGain),
                named("bypassCabinet", t.bypassCabinet),
                named("bypassPreamp", t.bypassPreamp),
                named("gain", t.gain),
                named("bass", t.bass),
                named("middle", t.middle),
                named("treble", t.treble),
                named("high-gain-presence", t.presence),
                named("master", t.master),
                named("ampType", t.ampType),
                named("delayTime", t.delayTime),
                named("delayMix", t.delayMix),
                named("delayFeedback", t.feedback),
                named("delaySync", t.sync),
                named("delayNote", t.delayNote),
                named("delayBypass", t.delayBypass),
                named("chorusRate", t.rate),
                named("chorusWidth", t.width),
                named("chorusMix", t.chorusMix),
                named("chorusBypass", t.chorusBypass),
                named("reverbBypass", t.reverbBypass),
                named("reverbDecay", t.decay),
                named("reverbMix", t.reverbMix),
                named("bypass", t.bypass),
                named("eqBypass", t.bypassEq),
                named("hpfFq", t.hpfFq),
                named("lpfFq", t.lpfFq),
                named("eq65hzGain", t.eq65hzGain),
                named("eq125hzGain", t.eq125hzGain),
                named("eq250hzGain", t.eq250hzGain),
                named("eq500hzGain", t.eq500hzGain),
                named("eq1khzGain", t.eq1khzGain),
                named("eq2khzGain", t.eq2khzGain),
                named("eq4khzGain", t.eq4khzGain),
                named("eq8khzGain", t.eq8khzGain),
                named("eq16khzGain", t.eq16khzGain));
        }
    };
};