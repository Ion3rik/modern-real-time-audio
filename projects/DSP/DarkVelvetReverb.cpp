
#include "DarkVelvetReverb.h"
#include "MultiTapDelayLine.h"

namespace DSP 
{
DarkVelvetReverb::DarkVelvetReverb(unsigned int maxLengthSamples, unsigned int maxTaps, unsigned int numChannels) :
    multiTapDelayLine(maxLengthSamples, maxTaps, numChannels)
{

}

DarkVelvetReverb::~DarkVelvetReverb()
{
    
}

void DarkVelvetReverb::clear()
{
    multiTapDelayLine.clear();
}

void DarkVelvetReverb::prepare(double newSampleRate, unsigned int maxLengthSamples, unsigned int maxTaps, unsigned int numChannels)
{
    sampleRate = newSampleRate;
    
    multiTapDelayLine.prepare(maxLengthSamples, maxTaps, numChannels);
    

}

void DarkVelvetReverb::process(float* const* output, const float* const* input, unsigned int numChannels, unsigned int numSamples)
{
    multiTapDelayLine.process(output, input, numChannels, numSamples); // process in place
}

void DarkVelvetReverb::setReverberationTime(float newReverberationTimeS)
{
    reverberationTimeS = std::fmax(newReverberationTimeS, 0.f);
    //reverberationTimeRamp.setTarget(reverberationTimeS);
}

void DarkVelvetReverb::setNumPulses(unsigned int newNumPulses)
{
    numPulses = std::max(newNumPulses, 0u);
}

void DarkVelvetReverb::computeDelays(unsigned int numChannels)
{
    unsigned int delayTimeSamples = static_cast<unsigned int>(reverberationTimeS * sampleRate);
    multiTapDelayLine.computeDelays(delayTimeSamples, numPulses, numChannels);
}
}