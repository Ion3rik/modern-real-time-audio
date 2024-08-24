/*
  ==============================================================================

    velvetReverb.cpp
    Created: 18 Dec 2023 3:58:18pm
    Author:  Jon Fagerström

  ==============================================================================
*/

#include "VelvetReverb.h"


VelvetReverb::VelvetReverb(unsigned int maxLengthSamples, unsigned int maxTaps, unsigned int numChannels) :
    multiTapDelayLine(maxLengthSamples, maxTaps, numChannels)
{

}

VelvetReverb::~VelvetReverb()
{
    
}

void VelvetReverb::clear()
{
    multiTapDelayLine.clear();
}

void VelvetReverb::prepare(double newSampleRate, unsigned int maxLengthSamples, unsigned int maxTaps, unsigned int numChannels)
{
    sampleRate = newSampleRate;
    
    multiTapDelayLine.prepare(maxLengthSamples, maxTaps, numChannels);
    
    unsigned int rt = round(reverberationTimeS * sampleRate);
    
    unsigned int M = round(rt * (1.f / (newSampleRate * (1.f / densityPulsesPerS))));
    
    multiTapDelayLine.computeDelays(rt, 10, numChannels);
    
    reverberationTimeRamp.prepare(newSampleRate, true, reverberationTimeS);
    densityRamp.prepare(newSampleRate, true, densityPulsesPerS);
    
}

void VelvetReverb::process(float* const* output, const float* const* input, unsigned int numChannels, unsigned int numSamples)
{
    multiTapDelayLine.process(output, input, numChannels, numSamples); // process in place
}

void VelvetReverb::setReverberationTime(float newReverberationTimeS)
{
    reverberationTimeS = std::fmax(newReverberationTimeS, 0.f);
    reverberationTimeRamp.setTarget(reverberationTimeS);
}

void VelvetReverb::setDensity(float newDensityPulsesPerS)
{
    densityPulsesPerS = std::fmax(newDensityPulsesPerS, 0.f);
    densityRamp.setTarget(densityPulsesPerS);
}
