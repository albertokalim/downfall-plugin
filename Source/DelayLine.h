/*
  ==============================================================================

    DelayLine.h
    Created: 15 Mar 2026 12:25:48pm
    Author:  Alberto

  ==============================================================================
*/

#pragma once
#include <memory>
#include <JuceHeader.h>

namespace effects {
    class DelayLine
    {
    public:
        void setMaximumDelayInSamples(int maxLengthInSamples);
        void reset() noexcept;
        void write(float input) noexcept;
        float read(float delayInSamples) const noexcept;

        int getBufferLength() const noexcept
        {
            return bufferLength;
        }

    private:
        std::unique_ptr<float[]> buffer; //unique_ptr previene de memory leaks y es más seguro.
        int bufferLength = 0;
        int writeIndex = 0; // where the most recent value was written
    };
};