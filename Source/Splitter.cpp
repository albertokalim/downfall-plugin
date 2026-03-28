/*
  ==============================================================================

    Splitter.cpp
    Created: 25 Mar 2026 9:27:57am
    Author:  Alberto

  ==============================================================================
*/

#include "Splitter.h"

void effects::Splitter::prepare(int numSamples)
{
    for (auto& buffer : audioBuffers) {
        buffer.setSize(1, numSamples, false, true, true);
        buffer.clear();
    }
}

void effects::Splitter::split(juce::dsp::ProcessContextReplacing<float>& context)
{
    auto& block = context.getOutputBlock();
    const float* source = block.getChannelPointer(0);

    const int numSamples = (int)block.getNumSamples();

    for (int i = 0; i < REVERB_CHANNELS; ++i) {
        audioBuffers[i].copyFrom(0, 0, source, numSamples);
    }
}

void effects::Splitter::clearAudioBuffers()
{
    for (int i = 0; i < REVERB_CHANNELS; ++i) {
        audioBuffers[i].clear();
    }
}
