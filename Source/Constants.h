/*
  ==============================================================================

    Constants.h
    Created: 28 Mar 2026 11:04:21am
    Author:  Alberto

  ==============================================================================
*/

#pragma once
#include <cstdint>

static constexpr float MIN_DRIVE = 1.f;
static constexpr float MAX_DRIVE = 20.f;
static constexpr float MAX_BAND_GAIN = 4.f;
static constexpr float MIN_BAND_GAIN = 0.001f;
static constexpr float HPF_CENTER_FQ = 20.0f;
static constexpr float HPF_Q_FACTOR = 0.9f;
static constexpr float BASS_CENTER_FQ = 100.f;
static constexpr float BASS_Q_FACTOR = 0.6f;
static constexpr float MID_CENTER_FQ = 500.f;
static constexpr float MID_Q_FACTOR = 0.9f;
static constexpr float TREBLE_CENTER_FQ = 5000.f;
static constexpr float TREBLE_Q_FACTOR = 0.6f;

static constexpr float DEFAULT_SAMPLE_RATE = 44100.f;

static constexpr uint16_t DIFF_STEPS = 4u;
static constexpr uint16_t REVERB_CHANNELS = 8u;