#include "PluginProcessor.h"
#include "DvnParams.h"
#include "PluginEditor.h"

static const std::vector<mrta::ParameterInfo> parameters
{
    { Param::ID::Room, Param::Name::Room, Param::Range::RoomLabels, 0},
};

DarkVelvetReverbAudioProcessor::DarkVelvetReverbAudioProcessor() :
    parameterManager(*this, ProjectInfo::projectName, parameters),
    dvnReverb(10u*48000u) 
{
    parameterManager.registerParameterCallback(Param::ID::Room,
    [this] (float value, bool /*force*/)
    {
        room = static_cast<Params::RirModel>(std::rint(value));
        dvnReverb.loadParams(room);
    });
}

DarkVelvetReverbAudioProcessor::~DarkVelvetReverbAudioProcessor()
{
}

void DarkVelvetReverbAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    dvnReverb.clear();
    dvnReverb.prepare(sampleRate, 10u*48000u, 2u, samplesPerBlock);
    dvnReverb.loadParams(room);
}

void DarkVelvetReverbAudioProcessor::releaseResources()
{
    dvnReverb.clear();
}

void DarkVelvetReverbAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& /*midiMessages*/)
{
    juce::ScopedNoDenormals noDenormals;
    parameterManager.updateParameters();

    const unsigned int numChannels{ static_cast<unsigned int>(buffer.getNumChannels()) };
    const unsigned int numSamples{ static_cast<unsigned int>(buffer.getNumSamples()) };

    dvnReverb.process(buffer.getArrayOfWritePointers(), buffer.getArrayOfReadPointers(), numChannels, numSamples);



}

void DarkVelvetReverbAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    parameterManager.getStateInformation(destData);
}

void DarkVelvetReverbAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    parameterManager.setStateInformation(data, sizeInBytes);
}

//==============================================================================
const juce::String DarkVelvetReverbAudioProcessor::getName() const { return JucePlugin_Name; }
bool DarkVelvetReverbAudioProcessor::acceptsMidi() const { return false; }
bool DarkVelvetReverbAudioProcessor::producesMidi() const { return false; }
bool DarkVelvetReverbAudioProcessor::isMidiEffect() const { return false; }
double DarkVelvetReverbAudioProcessor::getTailLengthSeconds() const { return 0.0; }
int DarkVelvetReverbAudioProcessor::getNumPrograms() { return 1; }
int DarkVelvetReverbAudioProcessor::getCurrentProgram() { return 0; }
void DarkVelvetReverbAudioProcessor::setCurrentProgram(int) { }
const juce::String DarkVelvetReverbAudioProcessor::getProgramName(int) { return {}; }
void DarkVelvetReverbAudioProcessor::changeProgramName(int, const juce::String&) { }
bool DarkVelvetReverbAudioProcessor::hasEditor() const { return true; }
juce::AudioProcessorEditor* DarkVelvetReverbAudioProcessor::createEditor() { return new DarkVelvetReverbAudioProcessorEditor(*this); }
//==============================================================================

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DarkVelvetReverbAudioProcessor();
}