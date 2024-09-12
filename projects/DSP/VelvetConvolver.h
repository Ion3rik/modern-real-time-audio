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
    VelvetConvolver(unsigned int maxLengthSamples);
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
    void prepare(unsigned int maxLengthSamples, std::vector<unsigned int> maxNumPulses, unsigned int numChannels);

    // Process audio with the currently (fixed) set delay time
    void process(float* const* output, const float* const* input, unsigned int numChannels, unsigned int numSamples);

     // Set the current delay times in samples and their gains
    void setDelays(const std::vector<std::vector<unsigned int>>&  newPulseLocation, const std::vector<std::vector<float>> newPulseGain, std::vector<unsigned int> numPulses, unsigned int numChannels);

    // Apply delay modifier
    void modDelays(const float modifier);

private:
    std::vector<unsigned int> numPulses;
    std::vector<float> delayBuffer;
    std::vector<std::vector<unsigned int>> pulseLocation;
    std::vector<std::vector<unsigned int>> pulseLocationOg;
    std::vector<std::vector<float>> pulseGain;
    unsigned int writeIndex { 0u };
};
}
