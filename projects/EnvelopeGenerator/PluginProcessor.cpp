#include "PluginProcessor.h"
#include "PluginEditor.h"

static const std::vector<mrta::ParameterInfo> parameters
{
    { Param::ID::Trigger,      Param::Name::Trigger,      Param::Range::TriggerOff, Param::Range::TriggerOn, false },
    { Param::ID::Analog,       Param::Name::Analog,       Param::Range::AnalogOff,  Param::Range::AnalogOn, false },
    { Param::ID::AttackTime,   Param::Name::AttackTime,   Param::Unit::Ms,  50.0f, Param::Range::TimeMin,    Param::Range::TimeMax,    Param::Range::TimeInc,    Param::Range::TimeSkw },
    { Param::ID::DecayTime,    Param::Name::DecayTime,    Param::Unit::Ms,  25.0f, Param::Range::TimeMin,    Param::Range::TimeMax,    Param::Range::TimeInc,    Param::Range::TimeSkw },
    { Param::ID::SustainLevel, Param::Name::SustainLevel, "",                0.7f, Param::Range::SustainMin, Param::Range::SustainMax, Param::Range::SustainInc, Param::Range::SustainSkw },
    { Param::ID::ReleaseTime,  Param::Name::ReleaseTime,  Param::Unit::Ms, 100.0f, Param::Range::TimeMin,    Param::Range::TimeMax,    Param::Range::TimeInc,    Param::Range::TimeSkw },
};

EnvelopeGeneratorAudioProcessor::EnvelopeGeneratorAudioProcessor() :
    parameterManager(*this, ProjectInfo::projectName, parameters)
{
    parameterManager.registerParameterCallback(Param::ID::Trigger,
    [this] (float value, bool /*force*/)
    {
        if (value > 0.5f)
            env.start();
        else
            env.end();
    });

    parameterManager.registerParameterCallback(Param::ID::Analog,
    [this] (float value, bool /*force*/)
    {
        env.setAnalogStyle(value > 0.5f);
    });

    parameterManager.registerParameterCallback(Param::ID::AttackTime,
    [this] (float value, bool /*force*/)
    {
        env.setAttackTime(value);
    });

    parameterManager.registerParameterCallback(Param::ID::DecayTime,
    [this] (float value, bool /*force*/)
    {
        env.setDecayTime(value);
    });

    parameterManager.registerParameterCallback(Param::ID::SustainLevel,
    [this] (float value, bool /*force*/)
    {
        env.setSustainLevel(value);
    });

    parameterManager.registerParameterCallback(Param::ID::ReleaseTime,
    [this] (float value, bool /*force*/)
    {
        env.setReleaseTime(value);
    });
}

EnvelopeGeneratorAudioProcessor::~EnvelopeGeneratorAudioProcessor()
{
}

void EnvelopeGeneratorAudioProcessor::prepareToPlay(double sampleRate, int /*samplesPerBlock*/)
{
    parameterManager.updateParameters(true);
    env.prepare(sampleRate);
}

void EnvelopeGeneratorAudioProcessor::releaseResources()
{
}

void EnvelopeGeneratorAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& /*midiMessages*/)
{
    juce::ScopedNoDenormals noDenormals;
    parameterManager.updateParameters();

    const unsigned int numChannels{ static_cast<unsigned int>(buffer.getNumChannels()) };
    const unsigned int numSamples{ static_cast<unsigned int>(buffer.getNumSamples()) };

    env.process(buffer.getWritePointer(0), numSamples);

    if (numChannels > 1)
        buffer.copyFrom(1, 0, buffer, 0, 0, numSamples);
}

void EnvelopeGeneratorAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    parameterManager.getStateInformation(destData);
}

void EnvelopeGeneratorAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    parameterManager.setStateInformation(data, sizeInBytes);
}

//==============================================================================
const juce::String EnvelopeGeneratorAudioProcessor::getName() const { return JucePlugin_Name; }
bool EnvelopeGeneratorAudioProcessor::acceptsMidi() const { return true; }
bool EnvelopeGeneratorAudioProcessor::producesMidi() const { return false; }
bool EnvelopeGeneratorAudioProcessor::isMidiEffect() const { return false; }
double EnvelopeGeneratorAudioProcessor::getTailLengthSeconds() const { return 0.0; }
int EnvelopeGeneratorAudioProcessor::getNumPrograms() { return 1; }
int EnvelopeGeneratorAudioProcessor::getCurrentProgram() { return 0; }
void EnvelopeGeneratorAudioProcessor::setCurrentProgram(int) { }
const juce::String EnvelopeGeneratorAudioProcessor::getProgramName(int) { return {}; }
void EnvelopeGeneratorAudioProcessor::changeProgramName(int, const juce::String&) { }
bool EnvelopeGeneratorAudioProcessor::hasEditor() const { return true; }
juce::AudioProcessorEditor* EnvelopeGeneratorAudioProcessor::createEditor() { return new EnvelopeGeneratorAudioProcessorEditor(*this); }
//==============================================================================

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new EnvelopeGeneratorAudioProcessor();
}