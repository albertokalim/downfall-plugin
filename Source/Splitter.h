/*
  ==============================================================================

    Splitter.h
    Created: 25 Mar 2026 9:27:57am
    Author:  Alberto

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Constants.h"

using AudioBuffer = juce::AudioBuffer<float>;
namespace effects {
    //For simplicity, I only consider a pair number of splits
    class Splitter {
    public:
        Splitter() = default;
        std::array<AudioBuffer, REVERB_CHANNELS>& getAudioBuffers() { return audioBuffers; }
        AudioBuffer& getAudioBuffer(int i) { return audioBuffers[i]; }
        void prepare(int numSamples);
        void split(juce::dsp::ProcessContextReplacing<float>& context);
        void clearAudioBuffers();

    private:
        std::array<AudioBuffer, REVERB_CHANNELS> audioBuffers;
    };
}
