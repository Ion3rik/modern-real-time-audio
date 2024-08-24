/*
  ==============================================================================

    MultiTapDelayLine.cpp
    Created: 22 Dec 2023 8:49:13pm
    Author:  Jon Fagerström

  ==============================================================================
*/

#include "MultiTapDelayLine.h"

#include <algorithm>
#include <cmath>
#include <JuceHeader.h>
#include <random>


MultiTapDelayLine::MultiTapDelayLine(unsigned int maxLengthSamples, unsigned int numTaps, unsigned int numChannels)
{
    for (unsigned int ch = 0; ch < numChannels; ++ch)
    {
        delayBuffer.emplace_back(maxLengthSamples, 0.f);
        delayTimes.emplace_back(numTaps, 0u);
        delayGains.emplace_back(numTaps, 0.f);
    }
    
}

MultiTapDelayLine::~MultiTapDelayLine()
{
}

void MultiTapDelayLine::clear()
{
    for (auto& b : delayBuffer)
        std::fill(b.begin(), b.end(), 0.f);
    
    for (auto& t : delayTimes)
        std::fill(t.begin(), t.end(), 0u);
    
    for (auto& g : delayGains)
        std::fill(g.begin(), g.end(), 0.f);
        
}

void MultiTapDelayLine::prepare(unsigned int maxLengthSamples, unsigned int maxTaps, unsigned int numChannels)
{
    delayBuffer.clear();
    delayTimes.clear();
    delayGains.clear();
    for (unsigned int ch = 0; ch < numChannels; ++ch)
    {
        delayBuffer.emplace_back(maxLengthSamples, 0.f);
        delayTimes.emplace_back(maxTaps, 0u);
        delayGains.emplace_back(maxTaps, 0.f);
    }
}

void MultiTapDelayLine::process(float* const* output, const float* const* input, unsigned int numChannels, unsigned int numSamples)
{
    const unsigned int delayBufferSize { static_cast<unsigned int>(delayBuffer[0].size()) };
    const unsigned int delayTaps = numPulses;

    numChannels = std::min(numChannels, static_cast<unsigned int>(delayBuffer.size()));
    //unsigned int m = static_cast<unsigned int>(delayTimes[0].back());
    for (unsigned int ch = 0; ch < numChannels; ++ch)
    {
        unsigned int workingWriteIndex { writeIndex };
        for (unsigned int n = 0; n < numSamples; ++n) // loop through each sample
        {
            const float x { input[ch][n] };
            for (unsigned int m = 0; m < delayTaps; m++) // loop through each pulse
            {   
                unsigned int workingReadIndex { (workingWriteIndex + delayBufferSize - delayTimes[ch][m]) % delayBufferSize };
                output[ch][n] += delayGains[ch][m] * delayBuffer[ch][workingReadIndex];
            }
            delayBuffer[ch][workingWriteIndex] = x; // write to the delay line
            ++workingWriteIndex; workingWriteIndex %= delayBufferSize; // increment the working write index
            
        }
    }

    writeIndex += numSamples; writeIndex %= delayBufferSize;
}

/*
void MultiTapDelayLine::process(float* output, const float* input, unsigned int numChannels)
{
    const unsigned int delayBufferSize{ static_cast<unsigned int>(delayBuffer[0].size()) };

    numChannels = std::min(numChannels, static_cast<unsigned int>(delayBuffer.size()));

    unsigned int workingWriteIndex { writeIndex };
    unsigned int workingReadIndex { (workingWriteIndex + delayBufferSize - delaySamples) % delayBufferSize };

    for (unsigned int ch = 0; ch < numChannels; ++ch)
    {
        const float x { input[ch] };
        output[ch] = delayBuffer[ch][workingReadIndex];
        delayBuffer[ch][workingWriteIndex] = x;
    }

    ++writeIndex; writeIndex %= delayBufferSize;
}


void MultiTapDelayLine::process(float* const* audioOutput, const float* const* audioInput, const float* const* modInput, unsigned int numChannels, unsigned int numSamples)
{
    const unsigned int delayBufferSize{ static_cast<unsigned int>(delayBuffer[0].size()) };

    numChannels = std::min(numChannels, static_cast<unsigned int>(delayBuffer.size()));
    for (unsigned int ch = 0; ch < numChannels; ++ch)
    {
        // Calculate base indices based on fixed delay time
        unsigned int workingWriteIndex { writeIndex };
        unsigned int workingReadIndex { (workingWriteIndex + delayBufferSize - delaySamples) % delayBufferSize };

        for (unsigned int n = 0; n < numSamples; ++n)
        {
            // Linear interpolation coefficients
            const float m { std::fmax(modInput[ch][n], 0.f) };
            const float mFloor { std::floor(m) };
            const float mFrac0 { m - mFloor };
            const float mFrac1 { 1.f - mFrac0 };

            // Calculate read indices
            const unsigned int readIndex0 { (workingReadIndex + delayBufferSize - static_cast<unsigned int>(mFloor)) % delayBufferSize };
            const unsigned int readIndex1 { (readIndex0 + delayBufferSize - 1u) % delayBufferSize };

            // Read from delay line
            const float read0 = delayBuffer[ch][readIndex0];
            const float read1 = delayBuffer[ch][readIndex1];

            // Read audio input
            const float x { audioInput[ch][n] };

            // Interpolate output
            audioOutput[ch][n] = read0 * mFrac1 + read1 * mFrac0;

            // Write input
            delayBuffer[ch][workingWriteIndex] = x;

            // Increament indices
            ++workingWriteIndex; workingWriteIndex %= delayBufferSize;
            ++workingReadIndex; workingReadIndex %= delayBufferSize;
        }
    }

    // Update persistent write index
    writeIndex += numSamples; writeIndex %= delayBufferSize;
}

void MultiTapDelayLine::process(float* audioOutput, const float* audioInput, const float* modInput, unsigned int numChannels)
{
    const unsigned int delayBufferSize{ static_cast<unsigned int>(delayBuffer[0].size()) };

    // Calculate base indices based on fixed delay time
    unsigned int workingWriteIndex { writeIndex };
    unsigned int workingReadIndex { (workingWriteIndex + delayBufferSize - delaySamples) % delayBufferSize };

    numChannels = std::min(numChannels, static_cast<unsigned int>(delayBuffer.size()));
    for (unsigned int ch = 0; ch < numChannels; ++ch)
    {
        // Linear interpolation coefficients
        const float m { std::fmax(modInput[ch], 0.f) };
        const float mFloor { std::floor(m) };
        const float mFrac0 { m - mFloor };
        const float mFrac1 { 1.f - mFrac0 };

        // Calculate read indeces
        const unsigned int readIndex0 { (workingReadIndex + delayBufferSize - static_cast<unsigned int>(mFloor)) % delayBufferSize };
        const unsigned int readIndex1 { (readIndex0 + delayBufferSize - 1u) % delayBufferSize };

        // Read from delay line
        const float read0 = delayBuffer[ch][readIndex0];
        const float read1 = delayBuffer[ch][readIndex1];

        // Read audio input
        const float x { audioInput[ch] };

        // Interpolate output
        audioOutput[ch] = read0 * mFrac1 + read1 * mFrac0;

        // Write input
        delayBuffer[ch][workingWriteIndex] = x;
    }

    // Update persistent write index
    ++writeIndex; writeIndex %= delayBufferSize;
}
 */

void MultiTapDelayLine::computeDelays(unsigned int totalDelay, unsigned int newNumTaps, unsigned int numChannels)
{
    numChannels = std::min(numChannels, static_cast<unsigned int>(delayBuffer[0].size()));
    totalDelay = std::min(totalDelay, static_cast<unsigned int>(delayBuffer[1].size()));
    newNumTaps = std::min(newNumTaps, static_cast<unsigned int>(delayTimes[1].size()));
    numPulses = newNumTaps;
    
    float alpha = -log(pow(10, -3)) * (1.f / totalDelay);
    
    float Td = totalDelay * (1.f / newNumTaps); // grid size

    // Create a random number generator (Mersenne Twister engine)
    std::random_device rd;  // Obtain a seed from the hardware
    std::mt19937 gen(rd()); // Seed the generator

    // Define a uniform distribution in the range [0, 1]
    std::uniform_real_distribution<> dis(0.0, 1.0);
    
    for (unsigned int ch = 0; ch < numChannels; ++ch)
    {
        for (unsigned int m = 0; m < newNumTaps; m++)
        {
            float r1 = dis(gen);
            float r2 = dis(gen);
            delayTimes[ch][m] = roundf( m * Td + r1 * (Td-1));
            delayGains[ch][m] = roundf( 2.f * r1 - 1.f) * exp(-alpha * delayTimes[ch][m]); // sign * exponential decay term
        }
    }

}



