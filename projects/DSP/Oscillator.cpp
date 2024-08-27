#include "Oscillator.h"
#include <algorithm>
#include <cmath>

namespace DSP
{

Oscillator::Oscillator()
{
}

Oscillator::~Oscillator()
{
}

void Oscillator::prepare(double newSampleRate)
{
    sampleRate = newSampleRate;

    // update phase increment for new sample rate
    phaseInc = static_cast<float>(1.0 / sampleRate) * frequency;

    differentiatorCoeff = static_cast<float>(sampleRate) / (4.f * frequency * (1.f - frequency / static_cast<float>(sampleRate)));

    // reset states
    phaseState = 0.f;
    differentiatorState = 0.f;
}

void Oscillator::process(float* output, unsigned int numSamples)
{
    for (unsigned int n = 0; n < numSamples; ++n)
    {
        switch (type)
        {
        case Sin:
            output[n] = std::sin(static_cast<float>(2.0 * M_PI) * phaseState);
            break;

        case TriAliased:
            output[n] = 4.f * std::fabs(phaseState - 0.5f) - 1.f;
            break;

        case SawAliased:
            output[n] = 2.f * phaseState - 1.f;
            break;

        case TriAA:
            output[n] = dpwTri();
            break;

        case SawAA:
            output[n] = dpwSaw();
            break;

        default: break;
        }

        phaseState = std::fmod(phaseState + phaseInc, 1.f);
    }
}

float Oscillator::process()
{
    float osc { 0.f };

    switch (type)
    {
    case Sin:
        osc = std::sin(static_cast<float>(2.0 * M_PI) * phaseState);
        break;

    case TriAliased:
        osc = 4.f * std::fabs(phaseState - 0.5f) - 1.f;
        break;

    case SawAliased:
        osc = 2.f * phaseState - 1.f;
        break;

    case TriAA:
        osc = dpwTri();
        break;

    case SawAA:
        osc = dpwSaw();
        break;

    default: break;
    }

    phaseState = std::fmod(phaseState + phaseInc, 1.f);

    return osc;
}

void Oscillator::setFrequency(float freqHz)
{
    frequency = std::clamp(freqHz, 0.1f, 10000.f);

    phaseInc = static_cast<float>(1.0 / sampleRate) * frequency;
    differentiatorCoeff = static_cast<float>(sampleRate) / (4.f * frequency * (1.f - frequency / static_cast<float>(sampleRate)));
}

void Oscillator::setType(OscType newType)
{
    type = newType;

    // reset states
    phaseState = 0.f;
    differentiatorState = 0.f;
}

float Oscillator::dpwSaw()
{
    // unipolar to bipolar
    float bipolar = 2.f * phaseState - 1.f;

    // power of 2
    bipolar *= bipolar;

    // diffierentiator
    float output = bipolar - differentiatorState;
    differentiatorState = bipolar;

    // apply compensation coeff and return
    return output * differentiatorCoeff;
}

float Oscillator::dpwTri()
{
    // unipolar to bipolar
    float bipolar = 2.f * phaseState - 1.f;

    // power of 2
    float parabola = bipolar * bipolar;

    // add 1 offset
    float offset = 1.f - parabola;

    // square wave
    float square = std::copysign(1.f, bipolar) * offset;

    // diffierentiator - clip above 0.f to about spike
    float output = std::fmin(square - differentiatorState, 0.f);
    differentiatorState = square;

    // apply compensation coeff and return
    return 2.f * (output * differentiatorCoeff) + 1.f;
}

}
