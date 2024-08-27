/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

static const std::vector<mrta::ParameterInfo> parameterInfos
{
    { Param::ID::ReverberationTime,   Param::Name::ReverberationTime,   Param::Units::S, .5f,  Param::Ranges::ReverberationTimeMin,
        Param::Ranges::ReverberationTimeMax,   Param::Ranges::ReverberationTimeInc,   Param::Ranges::ReverberationTimeSkw },
    { Param::ID::NumPulses,   Param::Name::NumPulses,   "",  100u,  Param::Ranges::NumPulsesMin,
        Param::Ranges::NumPulsesMax,   Param::Ranges::NumPulsesInc,   Param::Ranges::NumPulsesSkw },
    
    
};

//==============================================================================
DarkVelvetReverbAudioProcessor::DarkVelvetReverbAudioProcessor() : 
    parameterManager(*this, ProjectInfo::projectName, parameterInfos),
    darkVelvetReverb(Param::Ranges::ReverberationTimeMax * 48000, Param::Ranges::NumPulsesMax, 2)
{

    
    parameterManager.registerParameterCallback(Param::ID::ReverberationTime,
    [this] (float newValue, bool /*force*/)
    {
        darkVelvetReverb.setReverberationTime(newValue);
        darkVelvetReverb.computeDelays(fxBuffer.getNumChannels());
    });
    
    parameterManager.registerParameterCallback(Param::ID::NumPulses,
    [this] (float newValue, bool /*force*/)
    {
        darkVelvetReverb.setNumPulses(newValue);
        darkVelvetReverb.computeDelays(fxBuffer.getNumChannels());
    });
}

DarkVelvetReverbAudioProcessor::~DarkVelvetReverbAudioProcessor()
{
}

//==============================================================================
const juce::String DarkVelvetReverbAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DarkVelvetReverbAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DarkVelvetReverbAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DarkVelvetReverbAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DarkVelvetReverbAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DarkVelvetReverbAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DarkVelvetReverbAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DarkVelvetReverbAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DarkVelvetReverbAudioProcessor::getProgramName (int index)
{
    return {};
}

void DarkVelvetReverbAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DarkVelvetReverbAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    const unsigned int numChannels { static_cast<unsigned int>(std::max(getMainBusNumInputChannels(), getMainBusNumOutputChannels())) };
    
    darkVelvetReverb.prepare(sampleRate, 48000, 2000, numChannels);
    
    parameterManager.updateParameters(true);
    
    fxBuffer.setSize(static_cast<int>(numChannels), samplesPerBlock);
    fxBuffer.clear();
}

void DarkVelvetReverbAudioProcessor::releaseResources()
{
    darkVelvetReverb.clear();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DarkVelvetReverbAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DarkVelvetReverbAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    parameterManager.updateParameters();

    const unsigned int numChannels { static_cast<unsigned int>(buffer.getNumChannels()) };
    const unsigned int numSamples { static_cast<unsigned int>(buffer.getNumSamples()) };

    for (int ch = 0; ch < static_cast<int>(numChannels); ++ch)
        fxBuffer.copyFrom(ch, 0, buffer, ch, 0, static_cast<int>(numSamples));

    darkVelvetReverb.process(fxBuffer.getArrayOfWritePointers(), fxBuffer.getArrayOfReadPointers(), numChannels, numSamples);

    for (int ch = 0; ch < static_cast<int>(numChannels); ++ch)
        buffer.addFrom(ch, 0, fxBuffer, ch, 0, static_cast<int>(numSamples));
}

//==============================================================================
bool DarkVelvetReverbAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DarkVelvetReverbAudioProcessor::createEditor()
{
    return new DarkVelvetReverbAudioProcessorEditor (*this);
}

//==============================================================================
void DarkVelvetReverbAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DarkVelvetReverbAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DarkVelvetReverbAudioProcessor();
}
