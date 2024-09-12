
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

void DarkVelvetReverb::prepare(double newSampleRate, unsigned int maxLengthSamples, unsigned int numChannels, unsigned int samplesPerBlock, Params::RirModel rirModel)
{
    sampleRate = newSampleRate;

    // Load new params
    dvnParams.switchRoom(rirModel); 

    // Update the velvet convolver
    velvetConvolver.prepare(maxLengthSamples, dvnParams.numPulsePerChannel, numChannels * dvnParams.numFilter);
    velvetConvolver.clear();
    velvetConvolver.setDelays(dvnParams.pulseLocation,
                               dvnParams.pulseGain,
                              dvnParams.numPulsePerChannel, 
                            dvnParams.numChannel * dvnParams.numFilter);

    // Init dvn output buffer
    dvnOutput.setSize(numChannels * dvnParams.numFilter, samplesPerBlock);
    dvnOutput.clear();

    // Update filters
    postFilter.clear();
    dictionaryFilter.clear();

    for (unsigned int q = 0; q < dvnParams.numFilter; ++q) // update dictionary filter coeffs
    {
        
        dictionaryFilter.emplace_back(dvnParams.dictionaryFilterOrder, 2);
        dictionaryFilter[q].setCoeffs(dvnParams.dictionaryFilterCoeff[q]); // set the coefficients
    }
    postFilter = DSP::Allpole(dvnParams.postFilterOrder,2); // init the post filter
    postFilter.setCoeffs(dvnParams.postFilterCoeff);
}

void DarkVelvetReverb::prepare(Params::RirModel rirModel)
{
    // Load new params
    dvnParams.switchRoom(rirModel); 

    // Update the velvet convolver
    velvetConvolver.clear();
    velvetConvolver.setDelays(dvnParams.pulseLocation,
                               dvnParams.pulseGain,
                              dvnParams.numPulsePerChannel, 
                            dvnParams.numChannel * dvnParams.numFilter);

    // clear the dvn output buffer
    dvnOutput.clear();

    // Update filters
    postFilter.clear();
    dictionaryFilter.clear();

    for (unsigned int q = 0; q < dvnParams.numFilter; ++q) // update dictionary filter coeffs
    {
        dictionaryFilter.emplace_back(dvnParams.dictionaryFilterOrder, 2);
        dictionaryFilter[q].setCoeffs(dvnParams.dictionaryFilterCoeff[q]); // set the coefficients
    }

    postFilter = DSP::Allpole(dvnParams.dictionaryFilterOrder,2); // init the post filter
    postFilter.setCoeffs(dvnParams.postFilterCoeff);
}

void DarkVelvetReverb::process(float* const* output, const float* const* input, unsigned int numChannels, unsigned int numSamples)
{
    unsigned int Q = dvnParams.numFilter;
    dvnOutput.clear();
   // dvnOutput.setSize(2*Q, numSamples);
    numChannels = 2u; // We will always produce stereo output
   
    

    unsigned int totalNumInternalChannels = numChannels * dvnParams.numFilter;

    // Run the velvet convolution to obtain numChannels*numFilters outputs
    auto dvnOutputWrite = dvnOutput.getArrayOfWritePointers();
    auto dvnOutputRead = dvnOutput.getArrayOfReadPointers();

    
    velvetConvolver.process(dvnOutputWrite, input, totalNumInternalChannels, numSamples);
    // Run the dictionary filters as stereo pairs
    
    for (unsigned int q = 0; q < Q; ++q) 
    {   
        float* const outputChannels[] = { dvnOutputWrite[q], dvnOutputWrite[q+Q] };
        const float* const inputChannels[] = { dvnOutputRead[q], dvnOutputRead[q+Q] };
        dictionaryFilter[q].process(outputChannels, inputChannels, numChannels, numSamples); // this will process left and right channels

        // Down mix to number of channels and apply channel gains
        for (unsigned int ch = 0; ch < numChannels; ++ch)
        {
            if (q == 0)
                juce::FloatVectorOperations::copyWithMultiply(output[ch], inputChannels[ch], dvnParams.channelGain[ch], static_cast<int>(numSamples));
            else
                juce::FloatVectorOperations::addWithMultiply(output[ch], inputChannels[ch], dvnParams.channelGain[ch], static_cast<int>(numSamples));
        }

    }

    
    
    // Run the post filter
    postFilter.process(output, input, numChannels, numSamples);
    
}

void DarkVelvetReverb::modDelays(const float modifier)
{
    velvetConvolver.modDelays(modifier);
}

}