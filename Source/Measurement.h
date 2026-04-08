/*
  ==============================================================================

    Measurement.h
    Created: 8 Apr 2026 6:23:52pm
    Author:  Alberto

  ==============================================================================
*/

#pragma once

#include <atomic>

struct Measurement
{
    void reset() noexcept
    {
        value.store(0.0f);
    }

    void updateIfGreater(float newValue) noexcept
    {
        auto oldValue = value.load();
        while (newValue > oldValue &&
            !value.compare_exchange_weak(oldValue, newValue)); //compare and swap loop.
    }

    float readAndReset() noexcept
    {
        return value.exchange(0.0f);
    }

    std::atomic<float> value;
};