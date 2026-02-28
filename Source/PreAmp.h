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

    static constexpr float MIN_DRIVE = 1.f;
    static constexpr float MAX_DRIVE = 20.f;
    static constexpr float MAX_BAND_GAIN = 6.f;
    static constexpr float MIN_BAND_GAIN = -6.f;
    static constexpr float HPF_CENTER_FQ = 20.0f;
    static constexpr float HPF_Q_FACTOR = 0.9f;
    static constexpr float BASS_CENTER_FQ = 100.f;
    static constexpr float BASS_Q_FACTOR = 0.6f;
    static constexpr float MID_CENTER_FQ = 500.f;
    static constexpr float MID_Q_FACTOR = 0.9f;
    static constexpr float TREBLE_CENTER_FQ = 5000.f;
    static constexpr float TREBLE_Q_FACTOR = 0.6f; 

    namespace waveshapingFunctions {
        static float asymptoticLimit(float x) {
            return (5.f * x) / (std::abs(5.f * x) + 1.f);
        }

        static float tanh(float x) {
            return std::tanh(4.98f*x);
        }

        static float polynomial(float x) {
            float input = std::clamp(x, -1.f, 1.f);
            return 3/2 * std::sin(input) - 1/2 * std::pow(std::sin(input), 3);
        }
    };

    class PreAmp {
    public:
        virtual void prepare(juce::dsp::ProcessSpec& spec) = 0;
        virtual void process(juce::dsp::ProcessContextReplacing<float>& context) = 0;
        virtual void reset() = 0;

        void updateCommonParameters(parameters::PreAmpParameters& parameters) {

            gain.setGainLinear(mapValueInRange(parameters.getGain().get() / 100.f, MIN_DRIVE, MAX_DRIVE));

            bassSmoother.setTargetValue(mapValueInRange(parameters.getBass().get() / 100.f, MIN_BAND_GAIN, MAX_BAND_GAIN));
            *bassEQ.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 
                BASS_CENTER_FQ, 
                BASS_Q_FACTOR, 
                juce::Decibels::decibelsToGain(bassSmoother.getNextValue()));

            middleSmoother.setTargetValue(mapValueInRange(parameters.getMiddle().get() / 100.f, MIN_BAND_GAIN, MAX_BAND_GAIN));
            *middleEQ.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 
                MID_CENTER_FQ, 
                MID_Q_FACTOR, 
                juce::Decibels::decibelsToGain(middleSmoother.getNextValue()));

            trebleSmoother.setTargetValue(mapValueInRange(parameters.getTreble().get() / 100.f, MIN_BAND_GAIN, MAX_BAND_GAIN));
            *trebleEQ.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate,
                TREBLE_CENTER_FQ,
                TREBLE_Q_FACTOR,
                juce::Decibels::decibelsToGain(trebleSmoother.getNextValue()));

            master.setGainLinear(parameters.getMaster().get() / 100.f);
        }

    protected:
        using IIRFilter = juce::dsp::IIR::Filter<float>;
        using IIRCoefs = juce::dsp::IIR::Coefficients<float>;

        double sampleRate = 44100;

        juce::dsp::Gain<float> gain;
        juce::SmoothedValue<float> bassSmoother;
        juce::SmoothedValue<float> middleSmoother;
        juce::SmoothedValue<float> trebleSmoother;
        juce::dsp::Gain<float> master;

        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> bassEQ;
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> middleEQ;
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> trebleEQ;

        void prepareCommonParameters(juce::dsp::ProcessSpec& spec) {

            sampleRate = spec.sampleRate;

            gain.reset();
            gain.prepare(spec);
            gain.setGainLinear(mapValueInRange(0.5f, MIN_DRIVE, MAX_DRIVE));

            bassSmoother.reset(spec.sampleRate, 0.002f);
            bassSmoother.setCurrentAndTargetValue(mapValueInRange(0.5f, MIN_BAND_GAIN, MAX_BAND_GAIN));

            middleSmoother.reset(spec.sampleRate, 0.002f);
            middleSmoother.setCurrentAndTargetValue(mapValueInRange(0.5f, MIN_BAND_GAIN, MAX_BAND_GAIN));

            trebleSmoother.reset(spec.sampleRate, 0.002f);
            trebleSmoother.setCurrentAndTargetValue(mapValueInRange(0.5f, MIN_BAND_GAIN, MAX_BAND_GAIN));

            bassEQ.reset();
            *bassEQ.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, BASS_CENTER_FQ, BASS_Q_FACTOR, 0.f);
            bassEQ.prepare(spec);

            middleEQ.reset();
            *middleEQ.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, MID_CENTER_FQ, MID_Q_FACTOR, 0.f);
            middleEQ.prepare(spec);

            trebleEQ.reset();
            *trebleEQ.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, TREBLE_CENTER_FQ, TREBLE_Q_FACTOR, 0.f);
            trebleEQ.prepare(spec);

            master.reset();
            master.prepare(spec);
            master.setGainLinear(0.5f);
        }
    };

    class CleanAmp : public PreAmp {
    public:
        CleanAmp() {

        }

        void prepare(juce::dsp::ProcessSpec& spec) override {
            prepareCommonParameters(spec);

            waveShaper.reset();
            waveShaper.prepare(spec);

            highPassFilter.reset();
            *highPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(spec.sampleRate, HPF_CENTER_FQ, HPF_Q_FACTOR);
            highPassFilter.prepare(spec);
        }

        void process(juce::dsp::ProcessContextReplacing<float>& context) override {

            gain.process(context);

            waveShaper.process(context);
            highPassFilter.process(context);

            bassEQ.process(context);
            middleEQ.process(context);
            trebleEQ.process(context);

            master.process(context);
        }

        void reset() override {
            
        }

    private:
        juce::dsp::WaveShaper<float> waveShaper{ { waveshapingFunctions::asymptoticLimit } };
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> highPassFilter;
    };

    class HighGainAmp : public PreAmp {
    public:
        HighGainAmp() {

        }
        void prepare(juce::dsp::ProcessSpec& spec) override {

            oversampler = std::unique_ptr<juce::dsp::Oversampling<float>>(new juce::dsp::Oversampling<float>(spec.numChannels,
                2,
                juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR)
            );

            prepareCommonParameters(spec);

            oversampler->reset();
            oversampler->initProcessing(spec.maximumBlockSize);

            juce::dsp::ProcessSpec osSpec;
            osSpec.maximumBlockSize = spec.maximumBlockSize;
            osSpec.numChannels = spec.numChannels;
            osSpec.sampleRate = spec.sampleRate * oversampler->getOversamplingFactor();

            waveShaperFirst.reset();
            waveShaperFirst.prepare(osSpec);

            highPassFilterFirst.reset();
            *highPassFilterFirst.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(osSpec.sampleRate, HPF_CENTER_FQ, HPF_Q_FACTOR);
            highPassFilterFirst.prepare(osSpec);

            waveShaperSecond.reset();
            waveShaperSecond.prepare(osSpec);

            highPassFilterSecond.reset();
            *highPassFilterSecond.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(osSpec.sampleRate, HPF_CENTER_FQ, HPF_Q_FACTOR);
            highPassFilterSecond.prepare(osSpec);

            waveShaperThird.reset();
            waveShaperThird.prepare(osSpec);

            highPassFilterThird.reset();
            *highPassFilterThird.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(osSpec.sampleRate, 30.f, 0.6f);
            highPassFilterThird.prepare(osSpec);

            midBoostFilter.reset();
            *midBoostFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, 710.71f, 0.9f, 6.13f);
            midBoostFilter.prepare(spec);
        }

        void process(juce::dsp::ProcessContextReplacing<float>& context) override {

            gain.process(context);

            auto upSampleBlock = oversampler->processSamplesUp(context.getOutputBlock());
            juce::dsp::ProcessContextReplacing<float> upSampleContext(upSampleBlock);

            waveShaperFirst.process(upSampleContext);
            highPassFilterFirst.process(upSampleContext);

            waveShaperSecond.process(upSampleContext);
            highPassFilterSecond.process(upSampleContext);

            waveShaperThird.process(upSampleContext);
            highPassFilterThird.process(upSampleContext);

            oversampler->processSamplesDown(context.getOutputBlock());

            midBoostFilter.process(context);

            bassEQ.process(context);
            middleEQ.process(context);
            trebleEQ.process(context);

            master.process(context);
        }

        void reset() override {

        }

    private:
        std::unique_ptr<juce::dsp::Oversampling<float>> oversampler;

        juce::dsp::WaveShaper<float> waveShaperFirst{ { waveshapingFunctions::asymptoticLimit } };
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> highPassFilterFirst;

        juce::dsp::WaveShaper<float> waveShaperSecond{ { waveshapingFunctions::tanh } };
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> highPassFilterSecond;

        juce::dsp::WaveShaper<float> waveShaperThird{ { waveshapingFunctions::polynomial } };
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> highPassFilterThird;

        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> midBoostFilter;
    };
};
