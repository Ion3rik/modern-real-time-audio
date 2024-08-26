#include "PluginProcessor.h"
#include "JuceHeader.h"
#include "Oscillator.h"
#include "PluginEditor.h"
#include "mrta_utils/Source/Parameter/ParameterInfo.h"
#include "mrta_utils/Source/Parameter/ParameterManager.h"
#include <vector>


static const std::vector<mrta::ParameterInfo> paramVector
{
    { Param::ID::AttRelTime, Param::Name::AttRelTime, "ms", 100.f, Param::Ranges::AttRelTimeMin, Param::Ranges::AttRelTimeMax, Param::Ranges::AttRelTimeInc, Param::Ranges::AttRelTimeSkw },
    { Param::ID::WaveType, Param::Name::WaveType, Param::Ranges::WaveType, 0 }
};

MidiHandlerAudioProcessor::MidiHandlerAudioProcessor() :
    paramManager(*this, ProjectInfo::projectName, paramVector)
{
    voice = new DSP::SynthVoice();
    synth.addSound(new DSP::SynthSound());
    synth.addVoice(voice);

    paramManager.registerParameterCallback(Param::ID::AttRelTime,
    [this] (float value, bool /*force*/)
    {
        voice->setAttRelTime(value);
    });

    paramManager.registerParameterCallback(Param::ID::WaveType,
    [this] (float value, bool /*force*/)
    {
        DSP::Oscillator::OscType type = static_cast<DSP::Oscillator::OscType>(std::rint(value));
        voice->setWaveType(type);
    });
}

MidiHandlerAudioProcessor::~MidiHandlerAudioProcessor()
{
}

void MidiHandlerAudioProcessor::prepareToPlay(double sampleRate, int /*samplesPerBlock*/)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);
    paramManager.updateParameters(true);
}

void MidiHandlerAudioProcessor::releaseResources()
{
}

void MidiHandlerAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    paramManager.updateParameters();

    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

void MidiHandlerAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
}

void MidiHandlerAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
}

bool MidiHandlerAudioProcessor::acceptsMidi() const
{
    return true;
}

//==============================================================================
const juce::String MidiHandlerAudioProcessor::getName() const { return JucePlugin_Name; }
bool MidiHandlerAudioProcessor::producesMidi() const { return false; }
bool MidiHandlerAudioProcessor::isMidiEffect() const { return false; }
double MidiHandlerAudioProcessor::getTailLengthSeconds() const { return 0.0; }
int MidiHandlerAudioProcessor::getNumPrograms() { return 1; }
int MidiHandlerAudioProcessor::getCurrentProgram() { return 0; }
void MidiHandlerAudioProcessor::setCurrentProgram(int) { }
const juce::String MidiHandlerAudioProcessor::getProgramName(int) { return {}; }
void MidiHandlerAudioProcessor::changeProgramName(int, const juce::String&) { }
bool MidiHandlerAudioProcessor::hasEditor() const { return true; }
juce::AudioProcessorEditor* MidiHandlerAudioProcessor::createEditor() { return new MidiHandlerAudioProcessorEditor(*this); }
//==============================================================================

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiHandlerAudioProcessor();
}