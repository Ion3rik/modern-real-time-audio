#include "Allpole.h"

#include <algorithm>

namespace DSP
{

Allpole::Allpole(unsigned int newFilterOrder, unsigned int maxNumChannels) :
    filterOrder { newFilterOrder },
    allocatedChannels { maxNumChannels },
    coeffs((newFilterOrder+1), 0.f),
    states(newFilterOrder * maxNumChannels, 0.f)
{
}

Allpole::Allpole()
{
}

Allpole::~Allpole()
{
}

void Allpole::clear()
{
    std::fill(states.begin(), states.end(), 0.f);
}

void Allpole::reallocateChannels(unsigned int maxNumChannels)
{
    allocatedChannels = maxNumChannels;
    states.resize(allocatedChannels * filterOrder);
    std::fill(states.begin(), states.end(), 0.f);
}


void Allpole::setCoeffs(const std::vector<float>& newCoeffs)
{
    coeffs = newCoeffs;
}

void Allpole::process(float* const* output, const float* const* input, unsigned int numChannels, unsigned int numSamples)
{
    unsigned int channelOffset = filterOrder;
    numChannels = std::min(numChannels, allocatedChannels);
    for (unsigned int ch = 0; ch < numChannels; ++ch)
    {
        for (unsigned int n = 0; n < numSamples; ++n)
        {
            float x { input[ch][n] };

            float acc { x * coeffs[0] }; // b0
            for (unsigned int p = 0; p < filterOrder; ++p)
            {
                acc -= coeffs[p+1] * states[p + ch*channelOffset]; // a coeffs loop
            }
            
            output[ch][n] = acc;
        }
    }
}

void Allpole::process(float* output, const float* input, unsigned int numChannels)
{
    unsigned int channelOffset = filterOrder;
    numChannels = std::min(numChannels, allocatedChannels);
    for (unsigned int ch = 0; ch < numChannels; ++ch)
    {
        float x { input[ch] };
        float acc { x * coeffs[0] }; // b0
        for (unsigned int p = 0; p < filterOrder; ++p)
        {
            acc -= coeffs[p+1] * states[p + ch*channelOffset]; // a coeffs loop
        }
        
        output[ch] = acc;
    }
}

}