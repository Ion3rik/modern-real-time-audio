/*
  ==============================================================================

    VelvetConvolver.cpp
    Created: 22 Dec 2023 8:49:13pm
    Author:  Jon Fagerström

  ==============================================================================
*/

#include "VelvetConvolver.h"
#include "DvnParams.h"
#include "juce_core/system/juce_PlatformDefs.h"

#include <algorithm>
#include <cmath>
#include <iterator>

namespace DSP 
{
VelvetConvolver::VelvetConvolver(unsigned int maxLengthSamples, unsigned int numChannels)
{
    delayBuffer.resize(maxLengthSamples, 0.f);
    numChannels = numChannels;
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
    pulseLocationOg.clear();
    pulseGain.clear();
    delayBuffer.resize(maxLengthSamples, 0.f);
    numPulses = maxNumPulses;
    for (unsigned int ch = 0; ch < numChannels; ++ch)
    {
        pulseGain.emplace_back(maxNumPulses[ch], 0.f);
        pulseLocation.emplace_back(maxNumPulses[ch], 0u);
        pulseLocationOg.emplace_back(maxNumPulses[ch], 0u);
    }
}

void VelvetConvolver::process(float* const* output, const float* const* input, unsigned int numChannels, unsigned int numSamples)
{
    const unsigned int delayBufferSize { static_cast<unsigned int>(delayBuffer.size()) };
    unsigned int workingWriteIndex { writeIndex };
    for (unsigned int n = 0; n < numSamples; ++n) // loop through each sample
    {
        const float x =  input[0][n]; // take mono input only
        delayBuffer[workingWriteIndex] = x;
        for (unsigned int ch = 0; ch < numChannels; ++ch) // loop through each output channel
        {   
            float acc = 0.f;
            for (unsigned int m = 0; m < numPulses[ch]; m = m+densityDivider) // loop through each pulse (skip some based on divider)
            {
                unsigned int workingReadIndex { (workingWriteIndex + delayBufferSize - pulseLocation[ch][m]) % delayBufferSize };
                acc += pulseGain[ch][m] * delayBuffer[workingReadIndex];
            }
            output[ch][n] = acc;
        }
        ++workingWriteIndex; workingWriteIndex %= delayBufferSize; // increment the working write index
    }
    writeIndex += numSamples; writeIndex %= delayBufferSize;
}

// PULSE LOOP OUTER
/*void VelvetConvolver::process(float* const* output, const float* const* input, unsigned int numChannels, unsigned int numSamples)
{
    //std::vector test = {5461, 7282, 88000};
    const unsigned int delayBufferSize { static_cast<unsigned int>(delayBuffer.size()) };
    for (unsigned int ch = 0; ch < numChannels; ++ch) // loop through each output channel
    {   
        for (unsigned int m = 0; m < numPulses[ch]; m = m+densityDivider) // loop through each pulse
        {
            unsigned int workingWriteIndex { writeIndex };
            unsigned int workingReadIndex { (workingWriteIndex + delayBufferSize - pulseLocation[ch][m]) % delayBufferSize };
            for (unsigned int n = 0; n < numSamples; ++n) // loop through each sample
            {
                //if (m == 0)
                delayBuffer[workingWriteIndex] =  input[0][n]; // take mono input only

                output[ch][n] += pulseGain[ch][m] * delayBuffer[workingReadIndex];
                ++workingWriteIndex; workingWriteIndex %= delayBufferSize; // increment the working write index
                ++workingReadIndex; workingReadIndex %= delayBufferSize;
            }
        }
            
    }
    writeIndex += numSamples; writeIndex %= delayBufferSize;
}*/

void VelvetConvolver::setDelays(const std::vector<std::vector<unsigned int>>& newPulseLocation, const std::vector<std::vector<float>> newPulseGain, std::vector<unsigned int> newNumPulses, unsigned int numChannels)
{
    // Resize in case number of pulses changed
    for (unsigned int ch = 0; ch < numChannels; ++ch)
    {
        pulseGain[ch].resize(newNumPulses[ch]);
        pulseLocation[ch].resize(newNumPulses[ch]);
        pulseLocationOg[ch].resize(newNumPulses[ch]);
    }

    // set the new values to the vectors
    pulseGain = newPulseGain;
    pulseLocationOg = newPulseLocation;
    pulseLocation = newPulseLocation;
    numPulses = newNumPulses;
}

void VelvetConvolver::modDelays(const float modifier)
{
    pulseLocation = pulseLocationOg; // copy the og values
    unsigned int bufferSize = delayBuffer.size();
    for (auto& ch : pulseLocation)
    {
        for (auto& pulse : ch)
        {
            pulse =  static_cast<unsigned int>(pulse * modifier);
            pulse = std::min(pulse, bufferSize); // limit to the size of the buffer
        }
    }
}

void VelvetConvolver::flipPulseGains()
{
    unsigned int Q = numPulses.size() / numChannels;
    for (unsigned int ch = 0; ch < numChannels; ++ch)
    {
        std::reverse(pulseGain.begin()+ch*Q, pulseGain.begin()+(ch+1)*Q-1);
    }
    for (auto& ch : pulseGain)
    {
        std::reverse(ch.begin(), ch.end());
    }
}

void VelvetConvolver::setDensityDivider(const unsigned int divider)
{
    densityDivider = std::max(divider, 1u); // has to be at least 1
}

unsigned int VelvetConvolver::getTotalNumPulses()
{
    unsigned int totalNumPulses = 0;
    for (auto chPulses : numPulses)
    {
        totalNumPulses += chPulses;
    }
    return totalNumPulses;
}

unsigned int VelvetConvolver::getLongestDelay()
{
    unsigned int longestDelay = 0;
    for (auto ch : pulseLocation)
    {
        longestDelay = std::max(longestDelay, static_cast<unsigned int>(ch.back()));
    }
    return longestDelay;
}

}