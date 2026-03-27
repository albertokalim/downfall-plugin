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
    const auto& block = context.getInputBlock();

    const int numSamples = (int)block.getNumSamples();
    const int numChannels = (int)block.getNumChannels();

    //Revisar
    const float* source = block.getChannelPointer(0);

    auto& firstBuffer = audioBuffers[0];

    if (numChannels > 1) {
        const float* srcR = block.getChannelPointer(1);
        float* dest = firstBuffer.getWritePointer(0);

        // Mixdown manual rápido: (L + R) * 0.5
        for (int i = 0; i < numSamples; ++i) {
            dest[i] = (source[i] + srcR[i]) * 0.5f;
        }
    }
    else {
        // Si ya es mono, copia directa
        firstBuffer.copyFrom(0, 0, source, numSamples);
    }

    // 3. Ahora que audioBuffers[0] tiene el audio mono, 
    // simplemente lo copiamos a los otros 3 buffers.
    for (int i = 1; i < 4u; ++i) {
        audioBuffers[i].copyFrom(0, 0, firstBuffer, 0, 0, numSamples);
    }
}

void effects::Splitter::clearAudioBuffers()
{
    for (int i = 0; i < 4u; ++i) {
        audioBuffers[i].clear();
    }
}
