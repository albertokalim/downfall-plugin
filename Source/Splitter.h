/*
  ==============================================================================

    Splitter.h
    Created: 25 Mar 2026 9:27:57am
    Author:  Alberto

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <atomic>
#include "Constants.h"

using AudioBuffer = juce::AudioBuffer<float>;
namespace effects {
    //For simplicity, I only consider a pair number of splits
    class Splitter {
    public:
        Splitter(int _n);
        ~Splitter();
        std::vector<std::unique_ptr<AudioBuffer>>& getAudioBuffers() { return audioBuffers; }
        void prepare(int numSamples);
        void split(juce::dsp::ProcessContextReplacing<float>& context);
        void clearAudioBuffers();
        int getN() { return n; }

    private:
        int n = 0;
        std::vector<std::unique_ptr<AudioBuffer>> audioBuffers{};
    };
}
