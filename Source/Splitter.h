/*
  ==============================================================================

    Splitter.h
    Created: 25 Mar 2026 9:27:57am
    Author:  Alberto

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

using AudioBuffer = juce::AudioBuffer<float>;
namespace effects {
    //For simplicity, I only consider a pair number of splits
    class Splitter {
    public:
        Splitter() = default;
        std::array<AudioBuffer, 4u>& getAudioBuffers() { return audioBuffers; }
        AudioBuffer& getAudioBuffer(int i) { return audioBuffers[i]; }
        void prepare(int numSamples);
        void split(juce::dsp::ProcessContextReplacing<float>& context);

    private:
        std::array<AudioBuffer, 4u> audioBuffers;

        void clearAudioBuffers();
    };
}
