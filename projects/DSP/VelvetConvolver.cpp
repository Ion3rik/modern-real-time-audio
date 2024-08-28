/*
  ==============================================================================

    VelvetConvolver.cpp
    Created: 22 Dec 2023 8:49:13pm
    Author:  Jon Fagerström

  ==============================================================================
*/

#include "VelvetConvolver.h"

#include <algorithm>
#include <cmath>

namespace DSP 
{
VelvetConvolver::VelvetConvolver(unsigned int maxLengthSamples, unsigned int maxNumPulses, unsigned int numChannels)
{
    delayBuffer.resize(maxLengthSamples, 0.f);
    for (unsigned int ch = 0; ch < numChannels; ++ch)
    {
        pulseGain.emplace_back(maxNumPulses, 0.f);
        pulseLocation.emplace_back(maxNumPulses, 0u);
    }
    
}

VelvetConvolver::~VelvetConvolver()
{
}

void VelvetConvolver::clear()
{
    delayBuffer.assign(delayBuffer.size(),0.f);
    for (auto& t : pulseLocation)
        std::fill(t.begin(), t.end(), 0.f);
    
    for (auto& g : pulseGain)
        std::fill(g.begin(), g.end(), 0.f);
        
}

void VelvetConvolver::prepare(unsigned int maxLengthSamples, unsigned int maxNumPulses, unsigned int numChannels)
{
    delayBuffer.clear();
    pulseLocation.clear();
    pulseGain.clear();
    delayBuffer.resize(maxLengthSamples, 0.f);
    for (unsigned int ch = 0; ch < numChannels; ++ch)
    {
        pulseGain.emplace_back(maxNumPulses, 0.f);
        pulseLocation.emplace_back(maxNumPulses, 0u);
    }
}

void VelvetConvolver::process(float* const* output, const float* const* input, unsigned int numChannels, unsigned int numSamples)
{
    const unsigned int delayBufferSize { static_cast<unsigned int>(delayBuffer.size()) };

    numChannels = std::min(numChannels, static_cast<unsigned int>(delayBuffer.size()));
    unsigned int workingWriteIndex { writeIndex };
    for (unsigned int ch = 0; ch < numChannels; ++ch)
    {
        for (unsigned int m = 0; m < numPulses; ++m) // loop through each pulse
        {
            unsigned int workingReadIndex { (workingWriteIndex + delayBufferSize - pulseLocation[ch][m]) % delayBufferSize };
            float g = pulseGain[ch][m]; // read the gain

            for (unsigned int n = 0; n < numSamples; ++n) // loop through each sample
            {
                const float x = input[0][n]; // lets take only mono input

                output[ch][n] += g * delayBuffer[workingReadIndex];
                
                if (m == 0)
                    delayBuffer[workingWriteIndex] = x; // write to the delay line
                ++workingWriteIndex; workingWriteIndex %= delayBufferSize; // increment the working write index
                ++workingReadIndex; workingReadIndex %= delayBufferSize; // increment the working write index
            }
        }
    }
    writeIndex += numSamples; writeIndex %= delayBufferSize;
}

unsigned int VelvetConvolver::getNumPulses() const
{
    return numPulses;
}


void VelvetConvolver::setDelays(const unsigned int* const* newPulseLocation, const float* const* newPulseGain, unsigned int newNumPulses, unsigned int numChannels)
{
    // Resize in case number of pulses changed
    pulseGain.resize(newNumPulses);
    pulseLocation.resize(newNumPulses);

    // set the new values to the vectors
    for (size_t ch = 0; ch < numChannels; ++ch)
    {
        pulseGain.emplace_back(newPulseGain[ch], newPulseGain[ch] + newNumPulses);
        pulseLocation.emplace_back(newPulseLocation[ch], newPulseLocation[ch] + newNumPulses);
    }
    numPulses = newNumPulses;
}
}