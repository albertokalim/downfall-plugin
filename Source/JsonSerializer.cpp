/*
  ==============================================================================

    JsonSerializer.cpp
    Created: 1 Apr 2026 9:32:54am
    Author:  Alberto

  ==============================================================================
*/

#include "JsonSerializer.h"

parameters::SerializableGlobalParameters fromGlobal(parameters::GlobalParameters& parameters) {
    parameters::SerializableGlobalParameters serializable;
    serializable.bypassCabinet = parameters.bypassCabinet.get();
    serializable.bypassPreamp = parameters.bypassPreamp.get();
    serializable.gateThreshold = parameters.gateThreshold.get();
    serializable.inputGain = parameters.inputGain.get();
    serializable.outputGain = parameters.outputGain.get();
    return serializable;
}

parameters::SerializablePreAmpParameters fromPreAmp(parameters::PreAmpParameters& parameters) {
    parameters::SerializablePreAmpParameters serializable;
    serializable.ampType = parameters.ampType.getCurrentChoiceName();
    serializable.bass = parameters.bass.get();
    serializable.gain = parameters.gain.get();
    serializable.master = parameters.master.get();
    serializable.middle = parameters.middle.get();
    serializable.presence = parameters.presence.get();
    serializable.treble = parameters.treble.get();
    return serializable;
}

parameters::SerializableDelayParameters fromDelay(parameters::DelayParameters& parameters) {
    parameters::SerializableDelayParameters serializable;
    serializable.bypass = parameters.bypassEffect.get();
    serializable.delayNote = parameters.delayNote.getCurrentChoiceName();
    serializable.delayTime = parameters.delayTime.get();
    serializable.feedback = parameters.feedback.get();
    serializable.mix = parameters.mix.get();
    serializable.sync = parameters.sync.get();
    return serializable;
}

parameters::SerializableChorusParameters fromChorus(parameters::ChorusParameters& parameters) {
    parameters::SerializableChorusParameters serializable;
    serializable.bypass = parameters.bypassEffect.get();
    serializable.mix = parameters.mix.get();
    serializable.rate = parameters.rate.get();
    serializable.width = parameters.width.get();
    return serializable;
}

parameters::SerializableReverbParameters fromReverb(parameters::ReverbParameters& parameters) {
    parameters::SerializableReverbParameters serializable;
    serializable.bypass = parameters.bypassEffect.get();
    serializable.mix = parameters.mix.get();
    serializable.decay = parameters.decay.get();
    return serializable;
}

void JsonSerializer::serializeGlobalParameters(parameters::GlobalParameters& parameters, juce::OutputStream& output) 
{
    const auto parametersToSerialize = fromGlobal(parameters);
    const auto json = juce::ToVar::convert(parametersToSerialize);
    if (!json.has_value()) {
        return;
    }
    juce::JSON::writeToStream(output, *json, juce::JSON::FormatOptions{}.withSpacing(juce::JSON::Spacing::multiLine)
        .withMaxDecimalPlaces(2));
}

void JsonSerializer::serializePreAmpParameters(parameters::PreAmpParameters& parameters, juce::OutputStream& output)
{
    const auto parametersToSerialize = fromPreAmp(parameters);
    const auto json = juce::ToVar::convert(parametersToSerialize);
    if (!json.has_value()) {
        return;
    }
    juce::JSON::writeToStream(output, *json, juce::JSON::FormatOptions{}.withSpacing(juce::JSON::Spacing::multiLine)
        .withMaxDecimalPlaces(2));
}

void JsonSerializer::serializeDelayParameters(parameters::DelayParameters& parameters, juce::OutputStream& output)
{
    const auto parametersToSerialize = fromDelay(parameters);
    const auto json = juce::ToVar::convert(parametersToSerialize);
    if (!json.has_value()) {
        return;
    }
    juce::JSON::writeToStream(output, *json, juce::JSON::FormatOptions{}.withSpacing(juce::JSON::Spacing::multiLine)
        .withMaxDecimalPlaces(2));
}

void JsonSerializer::serializeChorusParameters(parameters::ChorusParameters& parameters, juce::OutputStream& output)
{
    const auto parametersToSerialize = fromChorus(parameters);
    const auto json = juce::ToVar::convert(parametersToSerialize);
    if (!json.has_value()) {
        return;
    }
    juce::JSON::writeToStream(output, *json, juce::JSON::FormatOptions{}.withSpacing(juce::JSON::Spacing::multiLine)
        .withMaxDecimalPlaces(2));
}

void JsonSerializer::serializeReverbParameters(parameters::ReverbParameters& parameters, juce::OutputStream& output)
{
    const auto parametersToSerialize = fromReverb(parameters);
    const auto json = juce::ToVar::convert(parametersToSerialize);
    if (!json.has_value()) {
        return;
    }
    juce::JSON::writeToStream(output, *json, juce::JSON::FormatOptions{}.withSpacing(juce::JSON::Spacing::multiLine)
        .withMaxDecimalPlaces(2));
}

juce::Result JsonSerializer::deserializeGlobalParameters(juce::InputStream& input, parameters::GlobalParameters& parameters) 
{
    juce::var parsedResult;
    const auto result = juce::JSON::parse(input.readEntireStreamAsString(), parsedResult);

    if (result.failed()) {
        return result;
    }

    const auto parsedParameters = juce::FromVar::convert<parameters::SerializableGlobalParameters>(parsedResult);

    if (!parsedParameters.has_value()) {
        return juce::Result::fail("failed to parse parameters from JSON representation");
    }

    parameters.bypassCabinet = parsedParameters->bypassCabinet;
    parameters.bypassPreamp = parsedParameters->bypassPreamp;
    parameters.gateThreshold = parsedParameters->gateThreshold;
    parameters.inputGain = parsedParameters->inputGain;
    parameters.outputGain = parsedParameters->outputGain;
    return juce::Result::ok();
}

juce::Result JsonSerializer::deserializePreAmpParameters(juce::InputStream& input, parameters::PreAmpParameters& parameters)
{
    juce::var parsedResult;
    const auto result = juce::JSON::parse(input.readEntireStreamAsString(), parsedResult);

    if (result.failed()) {
        return result;
    }

    const auto parsedParameters = juce::FromVar::convert<parameters::SerializablePreAmpParameters>(parsedResult);

    if (!parsedParameters.has_value()) {
        return juce::Result::fail("failed to parse parameters from JSON representation");
    }

    const auto ampTypeIndex = parameters.ampType.choices.indexOf(parsedParameters->ampType);
    parameters.ampType = ampTypeIndex;
    parameters.bass = parsedParameters->bass;
    parameters.gain = parsedParameters->gain;
    parameters.master = parsedParameters->master;
    parameters.middle = parsedParameters->middle;
    parameters.presence = parsedParameters->presence;
    parameters.treble = parsedParameters->treble;
    return juce::Result::ok();
}

juce::Result JsonSerializer::deserializeDelayParameters(juce::InputStream& input, parameters::DelayParameters& parameters)
{
    juce::var parsedResult;
    const auto result = juce::JSON::parse(input.readEntireStreamAsString(), parsedResult);

    if (result.failed()) {
        return result;
    }

    const auto parsedParameters = juce::FromVar::convert<parameters::SerializableDelayParameters>(parsedResult);

    if (!parsedParameters.has_value()) {
        return juce::Result::fail("failed to parse parameters from JSON representation");
    }

    const auto delayNoteIndex = parameters.delayNote.choices.indexOf(parsedParameters->delayNote);
    parameters.bypassEffect = parsedParameters->bypass;
    parameters.feedback = parsedParameters->feedback;
    parameters.delayNote = delayNoteIndex;
    parameters.delayTime = parsedParameters->delayTime;
    parameters.mix = parsedParameters->mix;
    parameters.sync = parsedParameters->sync;
    return juce::Result::ok();
}

juce::Result JsonSerializer::deserializeChorusParameters(juce::InputStream& input, parameters::ChorusParameters& parameters)
{
    juce::var parsedResult;
    const auto result = juce::JSON::parse(input.readEntireStreamAsString(), parsedResult);

    if (result.failed()) {
        return result;
    }

    const auto parsedParameters = juce::FromVar::convert<parameters::SerializableChorusParameters>(parsedResult);

    if (!parsedParameters.has_value()) {
        return juce::Result::fail("failed to parse parameters from JSON representation");
    }

    parameters.bypassEffect = parsedParameters->bypass;
    parameters.mix = parsedParameters->mix;
    parameters.rate = parsedParameters->rate;
    parameters.width = parsedParameters->width;
    return juce::Result::ok();
}

juce::Result JsonSerializer::deserializeReverbParameters(juce::InputStream& input, parameters::ReverbParameters& parameters)
{
    juce::var parsedResult;
    const auto result = juce::JSON::parse(input.readEntireStreamAsString(), parsedResult);

    if (result.failed()) {
        return result;
    }

    const auto parsedParameters = juce::FromVar::convert<parameters::SerializableReverbParameters>(parsedResult);

    if (!parsedParameters.has_value()) {
        return juce::Result::fail("failed to parse parameters from JSON representation");
    }

    parameters.bypassEffect = parsedParameters->bypass;
    parameters.decay = parsedParameters->decay;
    parameters.mix = parsedParameters->mix;
    return juce::Result::ok();
}


