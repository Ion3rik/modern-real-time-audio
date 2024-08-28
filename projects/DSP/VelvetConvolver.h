/*
  ==============================================================================

    VelvetConvolver.h
    Created: 22 Dec 2023 8:49:13pm
    Author:  Jon Fagerström

  ==============================================================================
*/

#pragma once

#include <vector>

namespace DSP
{
class VelvetConvolver
{
public:
    VelvetConvolver(unsigned int maxLengthSamples, unsigned int maxNumPulses, unsigned int numChannels);
    ~VelvetConvolver();

    // No default ctor
    VelvetConvolver() = delete;

    // No copy semantics
    VelvetConvolver(const VelvetConvolver&) = delete;
    const VelvetConvolver& operator=(const VelvetConvolver&) = delete;

    // No move semantics
    VelvetConvolver(VelvetConvolver&&) = delete;
    const VelvetConvolver& operator=(VelvetConvolver&&) = delete;

    // Clear the contents of the delay buffer
    void clear();

    // Reallocate delay buffer for the new channel count and clear its contents
    void prepare(unsigned int maxLengthSamples, unsigned int maxNumPulses, unsigned int numChannels);

    // Process audio with the currently (fixed) set delay time
    void process(float* const* output, const float* const* input, unsigned int numChannels, unsigned int numSamples);
    
    unsigned int getNumPulses() const;

     // Set the current delay times in samples and their gains
    void setDelays(const unsigned int* const* newDelays, const float* const* newGains, unsigned int numPulses, unsigned int numChannels);

private:
    unsigned int numPulses { 0u };
    std::vector<float> delayBuffer;
    std::vector<std::vector<unsigned int>> pulseLocation;
    std::vector<std::vector<float>> pulseGain;
    unsigned int writeIndex { 0u };
};
}
