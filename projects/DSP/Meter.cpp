#include "Meter.h"
#include <algorithm>

namespace DSP
{

Meter::Meter()
{
}

Meter::~Meter()
{
}

void Meter::prepare(double sampleRate, unsigned int newNumChannels)
{
    numChannels = std::max(std::min(newNumChannels, MaxNumChannels - 1), 0u);
    envelopeCoeff = std::exp(-1.f / (static_cast<float>(sampleRate * 0.001) * releaseTimeMs));
    envelopeState.store({ 0.f, 0.f });
}

void Meter::process(const float* const* input, unsigned int numChannels, unsigned int numSamples)
{
    auto currentEnvelope = envelopeState.load();

    for (int ch = 0; ch < numChannels; ++ch)
    {
        for (int n = 0; n < numSamples; ++n)
        {
            float x = std::fabs(input[ch][n]);
            float currentCoeff = (x > currentEnvelope[ch]) ? 0.f : envelopeCoeff;
            float newEnvelope = (currentEnvelope[ch] - x) * currentCoeff + x;
            currentEnvelope[ch] = newEnvelope;
        }
    }

    envelopeState.store(currentEnvelope);
}

void Meter::process(const float* input, unsigned int numChannels)
{
    auto currentEnvelope = envelopeState.load();

    for (int ch = 0; ch < numChannels; ++ch)
    {
        float x = std::fabs(input[ch]);
        float currentCoeff = (x > currentEnvelope[ch]) ? 0.f : envelopeCoeff;
        currentEnvelope[ch] = (currentEnvelope[ch] - x) * currentCoeff + x;
    }

    envelopeState.store(currentEnvelope);
}

void Meter::setTimeConstant(float newReleaseTimeMs)
{
    releaseTimeMs = std::clamp(newReleaseTimeMs, 1.f, 1000.f);
    envelopeCoeff = std::exp(-1.f / (static_cast<float>(sampleRate * 0.001) * releaseTimeMs));
}

float Meter::getEnvelope(unsigned int channel) const
{
    const auto currentEnvelope = envelopeState.load();
    return currentEnvelope[std::min(std::max(channel, 0u), MaxNumChannels - 1)];
}

unsigned int Meter::getNumChannels() const
{
    return numChannels;
}

}
