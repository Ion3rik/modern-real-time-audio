#include "Delay.h"

#include <algorithm>
#include <cmath>

namespace DSP
{

Delay::Delay(float maxTimeMs, unsigned int numChannels) :
    delayLine(static_cast<unsigned int>(std::ceil(std::fmax(maxTimeMs, 1.f) * static_cast<float>(0.001 * sampleRate))), numChannels),
    filter(1),
    preDistortionRamp(0.02f),
    postDistortionRamp(0.02f),
    timeRamp(0.5f),
    wowRamp(0.02f),
    feedbackRamp(0.02f)
{
}

Delay::~Delay()
{
}

void Delay::prepare(double newSampleRate, float maxTimeMs, unsigned int numChannels)
{
    sampleRate = newSampleRate;

    delayLine.prepare(static_cast<unsigned int>(std::round(maxTimeMs * static_cast<float>(0.001 * sampleRate))), numChannels);
    delayLine.setDelaySamples(1); // Keep at least 1 sample minimum fixed delay

    filter.setBandType(0, ParametricEqualizer::LowPass);
    filter.setBandResonance(0, static_cast<float>(M_SQRT1_2));
    filter.setBandFrequency(0, toneFrequency);
    filter.prepare(sampleRate, numChannels);

    const auto distortionLin = std::pow(10.f, 0.05f * distortion);
    preDistortionRamp.prepare(sampleRate, true, distortionLin);
    postDistortionRamp.prepare(sampleRate, true, 1.f / distortionLin);
    timeRamp.prepare(sampleRate, true, delayTimeMs * static_cast<float>(sampleRate * 0.001));
    wowRamp.prepare(sampleRate, true, wow * WowDepthMax * static_cast<float>(sampleRate));
    feedbackRamp.prepare(sampleRate, true, feedback * 0.98f);

    phaseState[0] = 0.f;
    phaseState[1] = static_cast<float>(M_PI / 2.0);
    phaseInc = static_cast<float>(2.0 * M_PI / sampleRate) * WowFreqHz;

    clear();
}

void Delay::clear()
{
    delayLine.clear();
    filter.clear();

    feedbackState[0] = 0.f;
    feedbackState[1] = 0.f;
}

void Delay::process(float* const* output, const float* const* input, unsigned int numChannels, unsigned int numSamples)
{
    for (unsigned int n = 0; n < numSamples; ++n)
    {
        // Process LFO acording to mod type
        float lfo[2] { 0.f, 0.f };

        // squared sine modulation
        lfo[0] = std::pow(0.5f + 0.5f * std::sin(phaseState[0]), 2.f);
        lfo[1] = std::pow(0.5f + 0.5f * std::sin(phaseState[1]), 2.f);

        // Increment and wrap phase states
        phaseState[0] = std::fmod(phaseState[0] + phaseInc, static_cast<float>(2 * M_PI));
        phaseState[1] = std::fmod(phaseState[1] + phaseInc, static_cast<float>(2 * M_PI));

        // Apply wow and time ramps
        wowRamp.applyGain(lfo, numChannels);
        timeRamp.applySum(lfo, numChannels);

        // Apply feedback ramp
        feedbackRamp.applyGain(feedbackState, numChannels);

        // Sum feedback
        float delayIn[2] { 0.f, 0.f };
        for (unsigned int ch = 0; ch < numChannels; ++ch)
            delayIn[ch] = input[ch][n] + feedbackState[ch];

        // Apply distortion
        preDistortionRamp.applyGain(delayIn, numChannels);
        float delayInDistortion[2];
        for (unsigned int ch = 0; ch < numChannels; ++ch)
            delayInDistortion[ch] = std::tanh(delayIn[ch]);
        postDistortionRamp.applyGain(delayInDistortion, numChannels);

        // Apply tone filter
        float delayInDistortionFilter[2] { 0.f, 0.f };
        filter.process(delayInDistortionFilter, delayInDistortion, numChannels);

        // Process delay
        delayLine.process(feedbackState, delayInDistortionFilter, lfo, numChannels);

        // Write to output buffers
        for (unsigned int ch = 0; ch < numChannels; ++ch)
            output[ch][n] = feedbackState[ch];
    }
}

void Delay::setDelayTime(float newDelayMs)
{
    delayTimeMs = std::fmax(newDelayMs, 1.f);
    timeRamp.setTarget(delayTimeMs * static_cast<float>(sampleRate * 0.001));
}

void Delay::setWow(float wowNorm)
{
    wow = std::fmax(wowNorm, 0.f);
    wowRamp.setTarget(wow * WowDepthMax * static_cast<float>(sampleRate));
}

void Delay::setFeedback(float feedbackNorm)
{
    feedback = std::clamp(feedbackNorm, 0.f, 1.f);
    feedbackRamp.setTarget(feedback * 0.98f);
}

void Delay::setToneFrequency(float toneFreqHz)
{
    toneFrequency = std::clamp(toneFreqHz, 20.f, 20000.f);
    filter.setBandFrequency(0, toneFrequency);
}

void Delay::setDistortion(float distortionDb)
{
    distortion = std::clamp(distortionDb, 0.f, 24.f);
    const auto distortionLin = std::pow(10.f, 0.05f * distortion);
    preDistortionRamp.setTarget(distortionLin);
    postDistortionRamp.setTarget(1.f / distortionLin);
}

}