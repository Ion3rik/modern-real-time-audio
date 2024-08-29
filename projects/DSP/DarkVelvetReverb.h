#pragma once

#include "../DSP/Ramp.h"
#include "../DSP/MultiTapDelayLine.h"
#include "../DSP/Allpole.h"
#include "ParametricEqualizer.h"
#include "VelvetConvolver.h"
#include <string>

namespace DSP 
{
class DarkVelvetReverb
{
public:

    DarkVelvetReverb(unsigned int maxLengthSamples, unsigned int maxNumPulses, unsigned int numChannels);
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
    void prepare(double newSampleRate, unsigned int maxLengthSamples, unsigned int numChannels);
    
    // Process audio with the current settings
    void process(float* const* output, const float* const* input, unsigned int numChannels, unsigned int numSamples);
    
    void loadParams(enum RirModel rirModel);

private:
    double sampleRate {48000.0};
    
    DSP::VelvetConvolver velvetConvolver;
    unsigned int numPulses;
    unsigned int numFilters;
    
    // Filters
    std::vector<DSP::Allpole> dictionaryFilter;
    DSP::Allpole postFilter;
    //DSP::ParametricEqualizer eq;


    // Params


    
};

}