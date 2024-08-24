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
    { Param::ID::Density,   Param::Name::Density,   Param::Units::Rho,  100.f,  Param::Ranges::DensityMin,
        Param::Ranges::DensityMax,   Param::Ranges::DensityInc,   Param::Ranges::DensitySkw },
    
    
};

//==============================================================================
VelvetReverbAudioProcessor::VelvetReverbAudioProcessor() : 
    parameterManager(*this, ProjectInfo::projectName, parameterInfos),
    velvetReverb(960000, 2000, 2)

{

    
    parameterManager.registerParameterCallback(Param::ID::ReverberationTime,
    [this] (float newValue, bool /*force*/)
    {
        velvetReverb.setReverberationTime(newValue);
    });
    
    parameterManager.registerParameterCallback(Param::ID::Density,
    [this] (float newValue, bool /*force*/)
    {
        velvetReverb.setDensity(newValue);
    });
}

VelvetReverbAudioProcessor::~VelvetReverbAudioProcessor()
{
}

//==============================================================================
const juce::String VelvetReverbAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool VelvetReverbAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool VelvetReverbAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool VelvetReverbAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double VelvetReverbAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int VelvetReverbAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int VelvetReverbAudioProcessor::getCurrentProgram()
{
    return 0;
}

void VelvetReverbAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String VelvetReverbAudioProcessor::getProgramName (int index)
{
    return {};
}

void VelvetReverbAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void VelvetReverbAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    const unsigned int numChannels { static_cast<unsigned int>(std::max(getMainBusNumInputChannels(), getMainBusNumOutputChannels())) };
    
    velvetReverb.prepare(sampleRate, 48000, 2000, numChannels);
    
    parameterManager.updateParameters(true);
    
    fxBuffer.setSize(static_cast<int>(numChannels), samplesPerBlock);
    fxBuffer.clear();
}

void VelvetReverbAudioProcessor::releaseResources()
{
    velvetReverb.clear();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool VelvetReverbAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void VelvetReverbAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    parameterManager.updateParameters();

    const unsigned int numChannels { static_cast<unsigned int>(buffer.getNumChannels()) };
    const unsigned int numSamples { static_cast<unsigned int>(buffer.getNumSamples()) };

    for (int ch = 0; ch < static_cast<int>(numChannels); ++ch)
        fxBuffer.copyFrom(ch, 0, buffer, ch, 0, static_cast<int>(numSamples));

    velvetReverb.process(fxBuffer.getArrayOfWritePointers(), fxBuffer.getArrayOfReadPointers(), numChannels, numSamples);

    for (int ch = 0; ch < static_cast<int>(numChannels); ++ch)
        buffer.addFrom(ch, 0, fxBuffer, ch, 0, static_cast<int>(numSamples));
}

//==============================================================================
bool VelvetReverbAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* VelvetReverbAudioProcessor::createEditor()
{
    return new VelvetReverbAudioProcessorEditor (*this);
}

//==============================================================================
void VelvetReverbAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void VelvetReverbAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VelvetReverbAudioProcessor();
}
