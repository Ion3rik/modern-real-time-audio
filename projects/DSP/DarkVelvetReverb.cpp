
#include "DarkVelvetReverb.h"
#include "Allpole.h"
#include "MultiTapDelayLine.h"
#include "VelvetConvolver.h"
#include "DvnParams.h"


namespace DSP 
{
DarkVelvetReverb::DarkVelvetReverb(unsigned int maxLengthSamples) :
    velvetConvolver(maxLengthSamples)
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
    dictionaryFilter.clear();
    postFilter.clear();
}

void DarkVelvetReverb::prepare(double newSampleRate, unsigned int maxLengthSamples, unsigned int numChannels, unsigned int samplesPerBlock)
{
    sampleRate = newSampleRate;

    // Allocate Buffers
    velvetConvolver.prepare(maxLengthSamples, dvnParams.numPulsePerChannel, numChannels * dvnParams.numFilter);
    velvetConvolver.clear();

    dvnOutput.setSize(numChannels * numFilters, samplesPerBlock);
    dvnOutput.clear();

    postFilter.clear();
    dictionaryFilter.clear();
}

void DarkVelvetReverb::process(float* const* output, const float* const* input, unsigned int numChannels, unsigned int numSamples)
{
    numChannels = 2u; // We will always produce stereo output
   
    unsigned int Q = dvnParams.numFilter;

    unsigned int totalNumInternalChannels = numChannels * numFilters;

    // Run the velvet convolution to obtain numChannels*numFilters outputs
    auto dvnOutputWrite = dvnOutput.getArrayOfWritePointers();
    auto dvnOutputRead = dvnOutput.getArrayOfReadPointers();
    velvetConvolver.process(dvnOutputWrite, input, numChannels, numSamples);

    // Run the dictionary filters as stereo pairs
    for (unsigned int q = 0; q < Q; ++q) 
    {   
        float* const selectedChannels[] = { dvnOutputWrite[q], dvnOutputWrite[q+Q] };
        dictionaryFilter[q].process(selectedChannels, input, numChannels, numSamples); // this will process left and right channels

        // Down mix to number of channels and apply channel gains
        for (unsigned int ch = 0; ch < numChannels; ++ch)
        {
            juce::FloatVectorOperations::addWithMultiply(output[ch], dvnOutputRead[q + Q*ch], dvnParams.channelGain[ch], static_cast<int>(numSamples));
        }

        // Run the post filter
        postFilter.process(output, output, numChannels, numSamples);

    }
    
}
void DarkVelvetReverb::loadParams(Params::RirModel rirModel)
{
    dvnParams.switchRoom(rirModel);   

    dvnOutput.clear(); // clear dictionary filter outputs

    velvetConvolver.clear();
    velvetConvolver.setDelays(dvnParams.pulseLocation,
                               dvnParams.pulseGain,
                              dvnParams.numPulsePerChannel, 
                            dvnParams.numChannel);
    dictionaryFilter.clear();
    unsigned int filterOrder = dvnParams.dictionaryFilterOrder;
    for (unsigned int q = 0; q < dvnParams.numFilter; ++q) // update dictionary filter coeffs
    {
        DSP::Allpole filterq(filterOrder,2);
        dictionaryFilter.push_back(filterq);
        dictionaryFilter[q].setCoeffs(dvnParams.dictionaryFilterCoeff[q]); // set the coefficients
    }

    postFilter.setCoeffs(dvnParams.postFilterCoeff);
}

}