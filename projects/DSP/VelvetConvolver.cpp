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
VelvetConvolver::VelvetConvolver(unsigned int maxLengthSamples)
{
    delayBuffer.resize(maxLengthSamples, 0.f);
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

void VelvetConvolver::prepare(unsigned int maxLengthSamples, std::vector<unsigned int> maxNumPulses, unsigned int numChannels)
{
    delayBuffer.clear();
    pulseLocation.clear();
    pulseGain.clear();
    delayBuffer.resize(maxLengthSamples, 0.f);
    numPulses = maxNumPulses;
    for (unsigned int ch = 0; ch < numChannels; ++ch)
    {
        pulseGain.emplace_back(maxNumPulses[ch], 0.f);
        pulseLocation.emplace_back(maxNumPulses[ch], 0u);
    }
}

void VelvetConvolver::process(float* const* output, const float* const* input, unsigned int numChannels, unsigned int numSamples)
{
    const unsigned int delayBufferSize { static_cast<unsigned int>(delayBuffer.size()) };

    numChannels = std::min(numChannels, static_cast<unsigned int>(delayBuffer.size()));
    unsigned int workingWriteIndex { writeIndex };
    for (unsigned int ch = 0; ch < numChannels; ++ch)
    {   
        for (unsigned int m = 0; m < numPulses[ch]; ++m) // loop through each pulse
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


void VelvetConvolver::setDelays(const std::vector<std::vector<unsigned int>>& newPulseLocation, const std::vector<std::vector<float>> newPulseGain, std::vector<unsigned int> newNumPulses, unsigned int numChannels)
{
    // Resize in case number of pulses changed
    for (unsigned int ch = 0; ch < numChannels; ++ch)
    {
        pulseGain[ch].resize(newNumPulses[ch]);
        pulseLocation[ch].resize(newNumPulses[ch]);
    }

    // set the new values to the vectors
    pulseGain = newPulseGain;
    pulseLocation = newPulseLocation;
    numPulses = newNumPulses;
}
}