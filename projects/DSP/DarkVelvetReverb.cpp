
#include "DarkVelvetReverb.h"
#include "MultiTapDelayLine.h"
#include "VelvetConvolver.h"

namespace DSP 
{
DarkVelvetReverb::DarkVelvetReverb(unsigned int maxLengthSamples, unsigned int maxNumPulses, unsigned int numChannels) :
    velvetConvolver(maxLengthSamples, maxNumPulses, numChannels)
    //eq(2u, numChannels)
{
    
}

DarkVelvetReverb::~DarkVelvetReverb()
{
    
}

void DarkVelvetReverb::clear()
{
    velvetConvolver.clear();
}

void DarkVelvetReverb::prepare(double newSampleRate, unsigned int maxLengthSamples, unsigned int numChannels)
{
    sampleRate = newSampleRate;
    
    velvetConvolver.prepare(maxLengthSamples, numPulses, numChannels);
    

}

void DarkVelvetReverb::process(float* const* output, const float* const* input, unsigned int numChannels, unsigned int numSamples)
{
    // For now a simple velvet convolution
    velvetConvolver.process(output, input, numChannels, numSamples); 
}

void DarkVelvetReverb::loadParams(std::string pathToParamFile)
{
    // Set all the member variables from the file
    
}

}