/*
  ==============================================================================

    JsonSerializer.cpp
    Created: 1 Apr 2026 9:32:54am
    Author:  Alberto

  ==============================================================================
*/

#include "JsonSerializer.h"
//Meter todos los parametros en una clase, quería hacerlo elegante pero no me dejan ser elegante.
parameters::SerializableParameters from(parameters::Parameters& parameters) {
    parameters::SerializableParameters serializable;
    serializable.bypassCabinet = parameters.bypassCabinet.get();
    serializable.bypassPreamp = parameters.bypassPreamp.get();
    serializable.gateThreshold = parameters.gateThreshold.get();
    serializable.inputGain = parameters.inputGain.get();
    serializable.outputGain = parameters.outputGain.get();
    serializable.ampType = parameters.ampType.getCurrentChoiceName();
    serializable.bass = parameters.bass.get();
    serializable.gain = parameters.gain.get();
    serializable.master = parameters.master.get();
    serializable.middle = parameters.middle.get();
    serializable.presence = parameters.presence.get();
    serializable.treble = parameters.treble.get();
    serializable.delayBypass = parameters.delayBypass.get();
    serializable.delayNote = parameters.delayNote.getCurrentChoiceName();
    serializable.delayTime = parameters.delayTime.get();
    serializable.feedback = parameters.feedback.get();
    serializable.delayMix = parameters.delayMix.get();
    serializable.sync = parameters.sync.get();
    serializable.chorusBypass = parameters.chorusBypass.get();
    serializable.chorusMix = parameters.chorusMix.get();
    serializable.rate = parameters.rate.get();
    serializable.width = parameters.width.get();
    serializable.reverbBypass = parameters.reverbBypass.get();
    serializable.reverbMix = parameters.reverbMix.get();
    serializable.decay = parameters.decay.get();
    return serializable;
}

void JsonSerializer::serializeParameters(parameters::Parameters& parameters, juce::OutputStream& output) 
{
    const auto parametersToSerialize = from(parameters);
    const auto json = juce::ToVar::convert(parametersToSerialize);
    if (!json.has_value()) {
        return;
    }
    juce::JSON::writeToStream(output, *json, juce::JSON::FormatOptions{}.withSpacing(juce::JSON::Spacing::multiLine)
        .withMaxDecimalPlaces(2));
}

juce::Result JsonSerializer::deserializeParameters(juce::InputStream& input, parameters::Parameters& parameters)
{
    juce::var parsedResult;
    const auto result = juce::JSON::parse(input.readEntireStreamAsString(), parsedResult);

    if (result.failed()) {
        return result;
    }

    const auto parsedParameters = juce::FromVar::convert<parameters::SerializableParameters>(parsedResult);

    if (!parsedParameters.has_value()) {
        return juce::Result::fail("failed to parse parameters from JSON representation");
    }

    parameters.bypassCabinet = parsedParameters->bypassCabinet;
    parameters.bypassPreamp = parsedParameters->bypassPreamp;
    parameters.gateThreshold = parsedParameters->gateThreshold;
    parameters.inputGain = parsedParameters->inputGain;
    parameters.outputGain = parsedParameters->outputGain;
    const auto ampTypeIndex = parameters.ampType.choices.indexOf(parsedParameters->ampType);
    parameters.ampType = ampTypeIndex;
    parameters.bass = parsedParameters->bass;
    parameters.gain = parsedParameters->gain;
    parameters.master = parsedParameters->master;
    parameters.middle = parsedParameters->middle;
    parameters.presence = parsedParameters->presence;
    parameters.treble = parsedParameters->treble;
    const auto delayNoteIndex = parameters.delayNote.choices.indexOf(parsedParameters->delayNote);
    parameters.delayBypass = parsedParameters->delayBypass;
    parameters.feedback = parsedParameters->feedback;
    parameters.delayNote = delayNoteIndex;
    parameters.delayTime = parsedParameters->delayTime;
    parameters.delayMix = parsedParameters->delayMix;
    parameters.sync = parsedParameters->sync;
    parameters.chorusBypass = parsedParameters->chorusBypass;
    parameters.chorusMix = parsedParameters->chorusMix;
    parameters.rate = parsedParameters->rate;
    parameters.width = parsedParameters->width;
    parameters.reverbBypass = parsedParameters->reverbBypass;
    parameters.decay = parsedParameters->decay;
    parameters.reverbMix = parsedParameters->reverbMix;
    return juce::Result::ok();
}


