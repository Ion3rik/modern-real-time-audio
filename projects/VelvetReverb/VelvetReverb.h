/*
  ==============================================================================

    velvetReverb.h
    Created: 18 Dec 2023 3:58:18pm
    Author:  Jon Fagerström

  ==============================================================================
*/

#pragma once

#include "../DSP/Ramp.h"
#include "./MultiTapDelayLine.h"


class VelvetReverb
{
public:
    VelvetReverb(unsigned int maxLengthSamples, unsigned int maxTaps, unsigned int numChannels);
    ~VelvetReverb();
    
    // remove default ctor
    VelvetReverb() = delete;
    
    // remove copy semantics
    VelvetReverb(const VelvetReverb&) = delete;
    const VelvetReverb& operator=(const VelvetReverb&) = delete;
    
    // remove move semantics
    VelvetReverb(const VelvetReverb&&) = delete;
    const VelvetReverb& operator=(const VelvetReverb&&) = delete;
    
    // Clear memory buffer
    void clear();
    
    // Recompute the velvet sequences, reallocate necessary memory and clear its contents
    void prepare(double newSampleRate, unsigned int maxLengthSamples, unsigned int maxTaps, unsigned int numChannels);
    
    // Process audio with the current settings
    void process(float* const* output, const float* const* input, unsigned int numChannels, unsigned int numSamples);
    
    // Set a broadband reverberation time in seconds
    void setReverberationTime(float newReverberationTimeS);
    
    // Set a constant velvet noise density in pulses/s
    void setNumPulses(unsigned int newNumPulses);
    

private:
    double sampleRate {48000.0};
    
    MultiTapDelayLine multiTapDelayLine;
    
    // Parameter ramps
    DSP::Ramp<float> reverberationTimeRamp;
    
    // Parameter values
    float reverberationTimeS {1.f};
    unsigned int numPulses {100u};
};


