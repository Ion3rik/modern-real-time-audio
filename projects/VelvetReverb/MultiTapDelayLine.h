/*
  ==============================================================================

    MultiTapDelayLine.h
    Created: 22 Dec 2023 8:49:13pm
    Author:  Jon Fagerström

  ==============================================================================
*/

#pragma once

#include <vector>


class MultiTapDelayLine
{
public:
    MultiTapDelayLine(unsigned int maxLengthSamples, unsigned int numTaps, unsigned int numChannels);
    ~MultiTapDelayLine();

    // No default ctor
    MultiTapDelayLine() = delete;

    // No copy semantics
    MultiTapDelayLine(const MultiTapDelayLine&) = delete;
    const MultiTapDelayLine& operator=(const MultiTapDelayLine&) = delete;

    // No move semantics
    MultiTapDelayLine(MultiTapDelayLine&&) = delete;
    const MultiTapDelayLine& operator=(MultiTapDelayLine&&) = delete;

    // Clear the contents of the delay buffer
    void clear();

    // Reallocate delay buffer for the new channel count and clear its contents
    void prepare(unsigned int maxLengthSamples, unsigned int maxTaps, unsigned int numChannels);

    // Process audio with the currently (fixed) set delay time
    void process(float* const* output, const float* const* input, unsigned int numChannels, unsigned int numSamples);
    
    unsigned int getNumPulses() const;
    /*
    // Single sample flavour of the fixed delay time processing
    void process(float* output, const float* input, unsigned int numChannels);

    
    // Process audio thru the delay line with audio rate modulation
    // The modulation input is a audio rate signal with the time modulation in samples
    // on top of the currently set delay time
    // The modulation input supports fractional values and uses linear interpolation
    void process(float* const* audioOutput, const float* const* audioInput, const float* const* modInput,
                 unsigned int numChannels, unsigned int numSamples);

    // Single sample flavour of the modulated delay time processing
    void process(float* audioOutput, const float* audioInput, const float* modInput, unsigned int numChannels);
    */

    // Set the current delay times in samples and their gains
    void computeDelays(unsigned int totalDelay, unsigned int newNumTaps, unsigned int numChannels);

private:
    unsigned int numPulses { 100u };
    std::vector<std::vector<float>> delayBuffer;
    std::vector<std::vector<unsigned int>> delayTimes;
    std::vector<std::vector<float>> delayGains;
    unsigned int writeIndex { 0u };
};

