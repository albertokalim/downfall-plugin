/*
  ==============================================================================

    PreAmp.h
    Created: 23 Feb 2026 11:16:52am
    Author:  Alberto

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Utils.h"
#include "ProcessorBase.h"

namespace preamp {

    namespace waveshapingFunctions {
        static float asymptoticLimit(float x) {
            return (5*x) / (std::abs(5*x) + 1);
        }
    };

    class PreAmp : public ProcessorBase{
    public:
        void prepareToPlay(double, int) override {}
        void releaseResources() override {}
        void processBlock(juce::AudioSampleBuffer&, juce::MidiBuffer&) override {}

    protected:
        
    };

    class CleanAmp : public PreAmp {
    public:
        CleanAmp() {

        }

        void prepareToPlay(double sampleRate, int samplesPerBlock) override {
            juce::dsp::ProcessSpec spec;
            spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
            spec.numChannels = getTotalNumOutputChannels();
            spec.sampleRate = sampleRate;

            gainSmoother.reset(sampleRate, 0.002f);
            gainSmoother.setCurrentAndTargetValue(mapValueInRange(0.5f, MIN_DRIVE, MAX_DRIVE));

            waveShaper.reset();
            waveShaper.prepare(spec);
        }

        void processBlock(juce::AudioSampleBuffer& buffer, juce::MidiBuffer&) override {

            for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
                auto* input = buffer.getReadPointer(channel);
                auto* output = buffer.getWritePointer(channel);

                for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
                {
                    auto inputSample = input[sample];

                    inputSample = inputSample * gainSmoother.getNextValue();

                    output[sample] = waveShaper.processSample(inputSample);

                }
            }
        }

        void releaseResources() override {
            
        }

        void updateParameters(parameters::PreAmpParameters& parameters) {
            gainSmoother.setTargetValue(mapValueInRange(parameters.getGain().get() / 100.f, MIN_DRIVE, MAX_DRIVE));
        }

    private:
        
        static constexpr float MIN_DRIVE = 1.f;
        static constexpr float MAX_DRIVE = 20.f;

        juce::dsp::WaveShaper<float> waveShaper{ { waveshapingFunctions::asymptoticLimit } };
        juce::SmoothedValue<float> gainSmoother;
        float bass = 0.0f;
        float middle = 0.0f;
        float treble = 0.0f;
        float master = 0.0f;


    };
};
