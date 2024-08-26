#include "SynthVoice.h"

namespace DSP
{

float convertMidiNoteToFreq(int MidiNote)
{
    return 440.f * std::pow(2.f, static_cast<float>(MidiNote - 69) / 12.f);
}

SynthVoice::SynthVoice()
{
}

SynthVoice::~SynthVoice()
{
}

void SynthVoice::setWaveType(Oscillator::OscType type)
{
    osc.setType(type);
}

void SynthVoice::setAttRelTime(float newAttRelTimeMs)
{
    ramp.setRampTime(newAttRelTimeMs * 0.001f);
}

// just check if this is "ours" synth sound
bool SynthVoice::canPlaySound(SynthesiserSound* ptr)
{
    if (dynamic_cast<SynthSound*>(ptr))
        return true;
    return false;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, SynthesiserSound*, int)
{
    ramp.setTarget(velocity);
    osc.setFrequency(convertMidiNoteToFreq(midiNoteNumber));
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    ramp.setTarget(0.f);
}

void SynthVoice::pitchWheelMoved(int)
{
}

void SynthVoice::controllerMoved(int, int)
{
}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    // there's no "prepare" call to synth voice, so we need to manually check
    // every buffer call if the sample rate has changed
    double newSampleRate { getSampleRate() };
    if (newSampleRate != sampleRate)
    {
        sampleRate = newSampleRate;
        osc.prepare(sampleRate);
        ramp.prepare(sampleRate);
    }

    // process the oscillator
    osc.process(outputBuffer.getWritePointer(0, startSample), static_cast<unsigned int>(numSamples));

    // apply simple ramp as VCA
    auto* outputPtr = outputBuffer.getWritePointer(0, startSample);
    ramp.applyGain(&outputPtr, 1u, static_cast<unsigned int>(numSamples));

    // if the output is stereo, just copy contents from left to right channel
    if (outputBuffer.getNumChannels() > 1)
        outputBuffer.copyFrom(1, startSample, outputBuffer, 0, startSample, numSamples);
}

}
