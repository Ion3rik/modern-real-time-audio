#include "EnvelopeGenerator.h"

#include <cmath>
#include <algorithm>

namespace DSP
{

EnvelopeGenerator::EnvelopeGenerator()
{
}

EnvelopeGenerator::~EnvelopeGenerator()
{
}

void EnvelopeGenerator::prepare(double newSampleRate)
{
    sampleRate = newSampleRate;

    // update time in samples with new sample rate
    attackTimeSamples = std::rint(attackTimeMs * static_cast<float>(sampleRate * 0.001));
    decayTimeSamples = std::rint(decayTimeMs * static_cast<float>(sampleRate * 0.001));
    releaseTimeSamples = std::rint(releaseTimeMs * static_cast<float>(sampleRate * 0.001));

    // reset counters and state
    state = OFF;
    attackSamplesCounter = 0;
    decaySamplesCounter = 0;
    releaseSamplesCounter = 0;
}

void EnvelopeGenerator::process(float* output, unsigned int numSamples)
{
    for (unsigned int n = 0; n < numSamples; ++n)
    {
        switch (state)
        {
        case OFF:
            {
                currentEnvelope = 0.f;
            }
            break;

        case ATTACK:
            if (attackSamplesCounter < attackTimeSamples)
            {
                currentEnvelope += (1.f - currentEnvelope) / static_cast<float>(attackTimeSamples - attackSamplesCounter);
                currentEnvelope = std::fmin(currentEnvelope, 1.f);
                ++attackSamplesCounter;
            }
            else
            {
                attackSamplesCounter = 0;
                state = DECAY;
            }
            break;

        case DECAY:
            if (decaySamplesCounter < decayTimeSamples)
            {
                currentEnvelope += (sustainLevel - currentEnvelope) / static_cast<float>(decayTimeSamples - decaySamplesCounter);
                ++decaySamplesCounter;
            }
            else
            {
                decaySamplesCounter = 0;
                state = SUSTAIN;
            }
            break;

        case SUSTAIN:
            {
            }
            break;

        case RELEASE:
            if (releaseSamplesCounter < releaseTimeSamples)
            {
                currentEnvelope += (0.f - currentEnvelope) / static_cast<float>(releaseTimeSamples - releaseSamplesCounter);
                ++releaseSamplesCounter;
            }
            else
            {
                releaseSamplesCounter = 0;
                state = OFF;
            }
            break;

        default: break;
        }

        output[n] = currentEnvelope;
    }
}

void EnvelopeGenerator::start()
{
    state = ATTACK;
    attackSamplesCounter = 0;
    decaySamplesCounter = 0;
    releaseSamplesCounter = 0;
}

void EnvelopeGenerator::end()
{
    state = RELEASE;
    attackSamplesCounter = 0;
    decaySamplesCounter = 0;
    releaseSamplesCounter = 0;
}

void EnvelopeGenerator::setAttackTime(float newAttackTimeMs)
{
    attackTimeMs = std::fmax(newAttackTimeMs, 0.1f);
    attackTimeSamples = std::rint(attackTimeMs * static_cast<float>(sampleRate * 0.001));

    if (state == ATTACK)
        attackSamplesCounter = std::min(attackTimeSamples - 1, attackSamplesCounter);
}

void EnvelopeGenerator::setDecayTime(float newDecayTimeMs)
{
    decayTimeMs = std::fmax(newDecayTimeMs, 0.1f);
    decayTimeSamples = std::rint(decayTimeMs * static_cast<float>(sampleRate * 0.001));

    if (state == DECAY)
        decaySamplesCounter = std::min(decayTimeSamples - 1, decaySamplesCounter);
}

void EnvelopeGenerator::setSustainLevel(float newSustainLevelLinear)
{
    sustainLevel = std::clamp(newSustainLevelLinear, 0.f, 1.f);
}

void EnvelopeGenerator::setReleaseTime(float newReleaseTimeMs)
{
    releaseTimeMs = std::fmax(newReleaseTimeMs, 0.1f);
    releaseTimeSamples = std::rint(releaseTimeMs * static_cast<float>(sampleRate * 0.001));

    if (state == RELEASE)
        releaseSamplesCounter = std::min(releaseTimeSamples - 1, releaseSamplesCounter);
}

}
