/*
  ==============================================================================

    Effect.cpp
    Created: 14 Mar 2026 12:17:08pm
    Author:  Alberto

  ==============================================================================
*/

#include "Effect.h"
namespace effects {
    static std::array<double, 16> noteLengthMultipliers =
    {
        0.125, // 0 = 1/32
        0.5 / 3.0, // 1 = 1/16 triplet
        0.1875, // 2 = 1/32 dotted
        0.25, // 3 = 1/16
        1.0 / 3.0, // 4 = 1/8 triplet
        0.375, // 5 = 1/16 dotted
        0.5, // 6 = 1/8
        2.0 / 3.0, // 7 = 1/4 triplet
        0.75, // 8 = 1/8 dotted
        1.0, // 9 = 1/4
        4.0 / 3.0, // 10 = 1/2 triplet
        1.5, // 11 = 1/4 dotted
        2.0, // 12 = 1/2
        8.0 / 3.0, // 13 = 1/1 triplet
        3.0, // 14 = 1/2 dotted
        4.0, // 15 = 1/1
    };

    void Tempo::reset() noexcept
    {
        bpm = 120.0;
    }

    void Tempo::update(const juce::AudioPlayHead* playhead) noexcept
    {
        reset();

        if (playhead == nullptr) { return; }

        const auto opt = playhead->getPosition(); //Devuelve un optional

        if (!opt.hasValue()) { return; }

        const auto& pos = *opt; //Desreferenciar

        if (pos.getBpm().hasValue()) { //También es un optional
            bpm = *pos.getBpm();
        }
    }

    double Tempo::getMillisecondsForNoteLength(int index) const noexcept
    {
        return 60000.0 * noteLengthMultipliers[size_t(index)] / bpm;
    }

    void DelayFX::prepare(juce::dsp::ProcessSpec& spec)
    {
        sampleRate = spec.sampleRate;

        tempo.reset();

        delayLine.prepare(spec);
        double numSamples = MAX_DELAY_TIME / 1000.0 * spec.sampleRate;
        int maxDelayInSamples = int(std::ceil(numSamples));
        delayLine.setMaximumDelayInSamples(maxDelayInSamples);
        delayLine.reset();

        delayTimeSmoother.reset(spec.sampleRate, 0.002f);
        delayTimeSmoother.setCurrentAndTargetValue(0.f);

        mixSmoother.reset(spec.sampleRate, 0.002f);
        mixSmoother.setCurrentAndTargetValue(1.0f);

        feedbackSmoother.reset(spec.sampleRate, 0.002f);
        feedbackSmoother.setCurrentAndTargetValue(0.f);
    }

    void DelayFX::update(parameters::FXParameters& parameters)
    {
        bypass = parameters.getBypassEffect().get();

        parameters::DelayParameters& delayParams = dynamic_cast<parameters::DelayParameters&>(parameters);

        delayTimeSmoother.setTargetValue(delayParams.getDelayTime().get());
        mixSmoother.setTargetValue(delayParams.getMix().get() / 100.f);
        feedbackSmoother.setTargetValue(delayParams.getFeedback().get() / 100.f);
        sync = delayParams.getSync().get();

        delayNoteIndex = delayParams.getDelayNote().getIndex();
    }

    void DelayFX::process(juce::dsp::ProcessContextReplacing<float>& context)
    {
        if (bypass) {
            return;
        }

        float syncedTime = float(tempo.getMillisecondsForNoteLength(delayNoteIndex));
        if (syncedTime > MAX_DELAY_TIME) {
            syncedTime = MAX_DELAY_TIME;
        }

        jassert(context.getInputBlock().getNumChannels() != context.getOutputBlock().getNumChannels()
            && context.getOutputBlock().getNumChannels() > 2);

        auto inputBlock = context.getInputBlock();
        auto outputBlock = context.getOutputBlock();
        for (int sample = 0; sample < context.getInputBlock().getNumSamples(); ++sample)
        {
            float delayTime = sync ? syncedTime : delayTimeSmoother.getNextValue();
            float delayInSamples = delayTime / 1000.0f * sampleRate;
            delayLine.setDelay(delayInSamples);

            float dryL = inputBlock.getSample(0, sample);
            float dryR = inputBlock.getSample(1, sample);

            float mono = (dryL + dryR) * 0.5f;

            delayLine.pushSample(0, mono + feedbackR);
            delayLine.pushSample(1, mono + feedbackL);

            float wetL = delayLine.popSample(0);
            float wetR = delayLine.popSample(1);

            feedbackL = wetL * feedbackSmoother.getNextValue();
            feedbackR = wetR * feedbackSmoother.getNextValue();

            float mixL = dryL + wetL * mixSmoother.getNextValue();
            float mixR = dryR + wetR * mixSmoother.getNextValue();

            outputBlock.setSample(0, sample, mixL);
            outputBlock.setSample(1, sample, mixR);
        }
    }

    void DelayFX::reset()
    {
    }
}

