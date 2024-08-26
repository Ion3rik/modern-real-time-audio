#pragma once

#include "../DSP/Ramp.h"
#include "./MultiTapDelayLine.h"

namespace DSP 
{
class DarkVelvetReverb
{
public:
    DarkVelvetReverb(unsigned int maxLengthSamples, unsigned int maxTaps, unsigned int numChannels);
    ~DarkVelvetReverb();
    
    // remove default ctor
    DarkVelvetReverb() = delete;
    
    // remove copy semantics
    DarkVelvetReverb(const DarkVelvetReverb&) = delete;
    const DarkVelvetReverb& operator=(const DarkVelvetReverb&) = delete;
    
    // remove move semantics
    DarkVelvetReverb(const DarkVelvetReverb&&) = delete;
    const DarkVelvetReverb& operator=(const DarkVelvetReverb&&) = delete;
    
    // Clear memory buffer
    void clear();
    
    // Recompute the velvet sequences, reallocate necessary memory and clear its contents
    void prepare(double newSampleRate, unsigned int maxLengthSamples, unsigned int maxTaps, unsigned int numChannels);
    
    // Process audio with the current settings
    void process(float* const* output, const float* const* input, unsigned int numChannels, unsigned int numSamples);
    
    // Set a broadband reverberation time in seconds
    void setReverberationTime(float newReverberationTimeS);
    
    // Set number of velvet pulses
    void setNumPulses(unsigned int newNumPulses);

    void computeDelays(unsigned int numChannels);
    

private:
    double sampleRate {48000.0};
    
    MultiTapDelayLine multiTapDelayLine;
    
    // Parameter ramps
    DSP::Ramp<float> reverberationTimeRamp;
    
    // Parameter values
    float reverberationTimeS {1.f};
    unsigned int numPulses {100u};
};

}