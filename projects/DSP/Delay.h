#pragma once

#include "DelayLine.h"
#include "ParametricEqualizer.h"
#include "Ramp.h"

namespace DSP
{

class Delay
{
public:
    Delay(float maxTimeMs, unsigned int numChannels);
    ~Delay();

    // No default ctors
    Delay() = delete;

    // No copy semantics
    Delay(const Delay&) = delete;
    const Delay& operator=(const Delay&) = delete;

    // No move semantics
    Delay(Delay&&) = delete;
    const Delay& operator=(Delay&&) = delete;

    // Update sample rate, reallocates and clear internal buffers
    void prepare(double sampleRate, float maxTimeMs, unsigned int numChannels);

    // Clear contents of internal buffer
    void clear();

    // Process audio
    void process(float* const* output, const float* const* input, unsigned int numChannels, unsigned int numSamples);

    // Set delay time in ms
    void setDelayTime(float newDelayMs);

    // Set tape wow normalised
    void setWow(float wowNorm);

    // Set feedback normalised
    void setFeedback(float feedbackNorm);

    // Set tone frequency in Hz
    void setToneFrequency(float toneFreqHz);

    // Set distortion in dB
    void setDistortion(float distortionDb);

private:
    double sampleRate { 48000.0 };

    DSP::DelayLine delayLine;
    DSP::ParametricEqualizer filter;

    DSP::Ramp<float> preDistortionRamp;
    DSP::Ramp<float> postDistortionRamp;
    DSP::Ramp<float> timeRamp;
    DSP::Ramp<float> wowRamp;
    DSP::Ramp<float> feedbackRamp;

    float feedbackState[2] { 0.f, 0.f };
    float phaseState[2] { 0.f, 0.f };
    float phaseInc { 0.f };

    float delayTimeMs { 0.f };
    float feedback { 0.f };
    float wow { 0.f };
    float toneFrequency { 5000.f };
    float distortion { 0.f };

    static constexpr float WowFreqHz { 2.f };
    static constexpr float WowDepthMax { 0.002f };
};

}