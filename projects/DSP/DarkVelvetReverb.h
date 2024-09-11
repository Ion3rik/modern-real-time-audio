#pragma once

#include "../DSP/Ramp.h"
#include "../DSP/MultiTapDelayLine.h"
#include "../DSP/Allpole.h"
#include "DvnParams.h"
#include "ParametricEqualizer.h"
#include "VelvetConvolver.h"
#include <string>
#include "JuceHeader.h"

namespace DSP 
{
class DarkVelvetReverb
{
public:

    DarkVelvetReverb(unsigned int maxLengthSamples);
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
    void prepare(double newSampleRate, unsigned int maxLengthSamples, unsigned int numChannels, unsigned int samplesPerBlock, Params::RirModel rirModel);
    void prepare(Params::RirModel rirModel);
    
    // Process audio with the current settings
    void process(float* const* output, const float* const* input, unsigned int numChannels, unsigned int numSamples);
    

private:
    double sampleRate {48000.0};
    
    DSP::VelvetConvolver velvetConvolver; // number of channels should be numChannels * numFilters
    juce::AudioBuffer<float> dvnOutput;
    
    // Filters
    std::vector<DSP::Allpole> dictionaryFilter;
    DSP::Allpole postFilter;

    // Params
    Params::DvnParams dvnParams;

    
};

}