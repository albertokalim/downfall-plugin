/*
  ==============================================================================

    Splitter.cpp
    Created: 25 Mar 2026 9:27:57am
    Author:  Alberto

  ==============================================================================
*/

#include "Splitter.h"

effects::Splitter::Splitter(int _n)
{
    n = _n;
}

effects::Splitter::~Splitter()
{

}

void effects::Splitter::prepare(int numSamples)
{
    if (audioBuffers.size() == 0) { 
        for (int i = 0; i < n; ++i) {
            std::unique_ptr<AudioBuffer> audioBuffer = std::make_unique<AudioBuffer>();
            audioBuffer->setSize(1, numSamples, false, true, true);
            audioBuffer->clear();
            audioBuffers.push_back(std::move(audioBuffer));
        }
    }
}

void effects::Splitter::split(juce::dsp::ProcessContextReplacing<float>& context)
{
    auto& block = context.getOutputBlock();
    float* source = context.getOutputBlock().getChannelPointer(0);
    const int numSamples = (int)block.getNumSamples();

    for (int i = 0; i < n; ++i) {
        audioBuffers[i]->copyFrom(0, 0, source, numSamples);
    }
}

void effects::Splitter::clearAudioBuffers()
{
    for (int i = 0; i < n; ++i) {
        if (audioBuffers[i] != nullptr) {
            audioBuffers[i]->clear();
        }
    }
}
