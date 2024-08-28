#pragma once

#include "../DSP/Ramp.h"
#include "../DSP/MultiTapDelayLine.h"
#include "../DSP/Allpole.h"
#include "ParametricEqualizer.h"

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
    
    // Modify the base reverberation time by stretching the pulse sequence
    void setRtModifier(float newModifier);
    
    // Set number of velvet pulses
    void setNumPulses(unsigned int newNumPulses);

    void computeDelays(unsigned int numChannels);

    void loadParams(std::string pathToParamFile);
    

private:
    double sampleRate {48000.0};
    
    DSP::MultiTapDelayLine multiTapDelayLine;
    float rtModifier {1.f};
    
    // Parameter values
    unsigned int numPulses {100u};
    std::vector<DSP::Allpole> dictionaryFilter;
    DSP::Allpole postFilter;
    DSP::ParametricEqualizer eq;
    std::vector<unsigned int> filterRouting;
    std::vector<unsigned int> pulseLocation;
    std::vector<float> pulseGain;
};

}