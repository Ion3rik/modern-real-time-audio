#include "PluginProcessor.h"
#include "Oscillator.h"
#include "PluginEditor.h"

static const std::vector<mrta::ParameterInfo> parameters
{
    { Param::ID::OscType, Param::Name::OscType, Param::Range::OscTypeLabels, 0 },
    { Param::ID::OscRate, Param::Name::OscRate, Param::Unit::Hz, 440.f, Param::Range::OscRateMin, Param::Range::OscRateMax, Param::Range::OscRateInc, Param::Range::OscRateSkw },
    { Param::ID::Volume,  Param::Name::Volume,  Param::Unit::dB, -12.f, Param::Range::VolumeMin,  Param::Range::VolumeMax,  Param::Range::VolumeInc,  Param::Range::VolumeSkw },
};

OscillatorsAudioProcessor::OscillatorsAudioProcessor() :
    parameterManager(*this, ProjectInfo::projectName, parameters)
{
    parameterManager.registerParameterCallback(Param::ID::OscRate,
    [this] (float value, bool /*force*/)
    {
        oscLeft.setFrequency(value);
        oscRight.setFrequency(value);
    });

    parameterManager.registerParameterCallback(Param::ID::OscType,
    [this] (float value, bool /*force*/)
    {
        DSP::Oscillator::OscType type = static_cast<DSP::Oscillator::OscType>(std::rint(value));
        oscLeft.setType(type);
        oscRight.setType(type);
    });

    parameterManager.registerParameterCallback(Param::ID::Volume,
    [this] (float value, bool force)
    {
        volumeDb = value;
        float volumeLin = std::pow(10.f, 0.05f * volumeDb);
        volumeRamp.setTarget(volumeLin, force);
    });
}

OscillatorsAudioProcessor::~OscillatorsAudioProcessor()
{
}

void OscillatorsAudioProcessor::prepareToPlay(double sampleRate, int /*samplesPerBlock*/)
{
    parameterManager.updateParameters(true);
    oscLeft.prepare(sampleRate);
    oscRight.prepare(sampleRate);
    volumeRamp.prepare(sampleRate, true, std::pow(10.f, 0.05f * volumeDb));
}

void OscillatorsAudioProcessor::releaseResources()
{
}

void OscillatorsAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& /*midiMessages*/)
{
    juce::ScopedNoDenormals noDenormals;
    parameterManager.updateParameters();

    const unsigned int numChannels{ static_cast<unsigned int>(buffer.getNumChannels()) };
    const unsigned int numSamples{ static_cast<unsigned int>(buffer.getNumSamples()) };

    oscLeft.process(buffer.getWritePointer(0), numSamples);
    if (numChannels == 2)
        oscRight.process(buffer.getWritePointer(1), numSamples);

    volumeRamp.applyGain(buffer.getArrayOfWritePointers(), numChannels, numSamples);
}

void OscillatorsAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    parameterManager.getStateInformation(destData);
}

void OscillatorsAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    parameterManager.setStateInformation(data, sizeInBytes);
}

//==============================================================================
const juce::String OscillatorsAudioProcessor::getName() const { return JucePlugin_Name; }
bool OscillatorsAudioProcessor::acceptsMidi() const { return false; }
bool OscillatorsAudioProcessor::producesMidi() const { return false; }
bool OscillatorsAudioProcessor::isMidiEffect() const { return false; }
double OscillatorsAudioProcessor::getTailLengthSeconds() const { return 0.0; }
int OscillatorsAudioProcessor::getNumPrograms() { return 1; }
int OscillatorsAudioProcessor::getCurrentProgram() { return 0; }
void OscillatorsAudioProcessor::setCurrentProgram(int) { }
const juce::String OscillatorsAudioProcessor::getProgramName(int) { return {}; }
void OscillatorsAudioProcessor::changeProgramName(int, const juce::String&) { }
bool OscillatorsAudioProcessor::hasEditor() const { return true; }
juce::AudioProcessorEditor* OscillatorsAudioProcessor::createEditor() { return new OscillatorsAudioProcessorEditor(*this); }
//==============================================================================

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OscillatorsAudioProcessor();
}