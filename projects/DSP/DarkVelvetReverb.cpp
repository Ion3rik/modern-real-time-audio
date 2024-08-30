
#include "DarkVelvetReverb.h"
#include "Allpole.h"
#include "MultiTapDelayLine.h"
#include "VelvetConvolver.h"
#include "DvnParams.h"


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
    dvnOutput.clear();
}

void DarkVelvetReverb::prepare(double newSampleRate, unsigned int maxLengthSamples, unsigned int numChannels, unsigned int samplesPerBlock)
{
    sampleRate = newSampleRate;

    // Allocate Buffers
    velvetConvolver.prepare(maxLengthSamples, dvnParams.numPulses, numChannels * dvnParams.numFilters);
    velvetConvolver.clear();

    dvnOutput.setSize(numChannels * numFilters, samplesPerBlock);
    dvnOutput.clear();
}

void DarkVelvetReverb::process(float* const* output, const float* const* input, unsigned int numChannels, unsigned int numSamples)
{
    // Load params
    unsigned int Q = dvnParams.numFilters;

    unsigned int totalNumInternalChannels = numChannels * numFilters;

    unsigned int velvetChannelOffset = numFilters; 

    // Run the velvet convolution to obtain numChannels*numFilters outputs
    velvetConvolver.process(dvnOutput.getArrayOfWritePointers(), input, numChannels, numSamples);


    for (unsigned int q = 0; q < totalNumInternalChannels; ++q) // filter loop
    {   
        // Process qth dictionary filter
        auto pointerForCurrentFilter = dvnOutput.getArrayOfWritePointers();
        dictionaryFilter[q].process(, input, numChannels, numSamples);
        for (int ch = 0; ch < numChannels; ++ch)
        {
            velvetConvolver.
        }
    }
    
    velvetConvolver.process(output, input, numChannels, numSamples); 
}

void DarkVelvetReverb::loadParams(Params::RirModel rirModel)
{
    dvnParams.switchRoom(rirModel);   
}

}