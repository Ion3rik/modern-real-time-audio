#include "PluginProcessor.h"
#include "DvnParams.h"
#include "ParametricEqualizer.h"
#include "PluginEditor.h"

static const std::vector<mrta::ParameterInfo> parameters
{
    //{ Param::ID::Room, Param::Name::Room, Param::Range::RoomLabels, 0},
    { Param::ID::RtMod, Param::Name::RtMod, "", 1.f, Param::Range::RtModMin, Param::Range::RtModMax, Param::Range::RtModInc, Param::Range::RtModSkw },
    { Param::ID::DensityMod, Param::Name::DensityMod, "", 2.f, Param::Range::DensityModMin, Param::Range::DensityModMax, Param::Range::DensityModInc, Param::Range::DensityModSkw },
    { Param::ID::Lowpass, Param::Name::Lowpass, "", 20000.f, Param::Range::LowpassMin, Param::Range::LowpassMax, Param::Range::LowpassInc, Param::Range::LowpassSkw },
    { Param::ID::Highpass, Param::Name::Highpass, "", 0.f, Param::Range::HighpassMin, Param::Range::HighpassMax, Param::Range::HighpassInc, Param::Range::HighpassSkw },
    { Param::ID::ReverseDecay,  Param::Name::ReverseDecay,  Param::Range::ReverseDecay, Param::Range::NormalDecay, true },
};

DarkVelvetReverbAudioProcessor::DarkVelvetReverbAudioProcessor() :
    parameterManager(*this, ProjectInfo::projectName, parameters),
    dvnReverb(10u*44100u, getTotalNumOutputChannels()),
    eq(2, getTotalNumOutputChannels())
{
    eq.setBandType(0, DSP::ParametricEqualizer::LowPass);
    eq.setBandType(1, DSP::ParametricEqualizer::HighPass);
    eq.setBandFrequency(0, 20000.f);
    eq.setBandFrequency(1, 0);
    //parameterManager.registerParameterCallback(Param::ID::Room,
    //[this] (float value, bool /*force*/)
    //{
    //    room = static_cast<Params::RirModel>(std::rint(value));
    //    dvnReverb.prepare(room);
    //});

    parameterManager.registerParameterCallback(Param::ID::RtMod,
    [this] (float value, bool /*force*/)
    {
        dvnReverb.modDelays(value);
    });

    parameterManager.registerParameterCallback(Param::ID::DensityMod,
    [this] (float value, bool /*force*/)
    {
        dvnReverb.setDensityDivider(static_cast<unsigned int>(value));
    });

    parameterManager.registerParameterCallback(Param::ID::Lowpass,
    [this] (float value, bool /*force*/)
    {
        eq.setBandFrequency(0, value);
    });

    parameterManager.registerParameterCallback(Param::ID::Highpass,
    [this] (float value, bool /*force*/)
    {
        eq.setBandFrequency(1, value);
    });
        parameterManager.registerParameterCallback(Param::ID::ReverseDecay,
    [this](float newValue, bool force)
    {
        dvnReverb.flipPulseGains();
    });
}


DarkVelvetReverbAudioProcessor::~DarkVelvetReverbAudioProcessor()
{
}

void DarkVelvetReverbAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    const unsigned int numChannels { static_cast<unsigned int>(std::max(getMainBusNumInputChannels(), getMainBusNumOutputChannels())) };
    dvnReverb.clear();
    dvnReverb.prepare(sampleRate, 3u*48000u, 2u, samplesPerBlock, room);

    fxBuffer.setSize(static_cast<int>(numChannels), samplesPerBlock);
    fxBuffer.clear();

    eq.setBandType(0, DSP::ParametricEqualizer::LowPass);
    eq.setBandType(1, DSP::ParametricEqualizer::HighPass);

    eq.prepare(sampleRate, numChannels);
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

    //for (int ch = 0; ch < static_cast<int>(numChannels); ++ch)
        //fxBuffer.copyFrom(ch, 0, buffer, ch, 0, static_cast<int>(numSamples));

    dvnReverb.process(buffer.getArrayOfWritePointers(), buffer.getArrayOfReadPointers(), numChannels, numSamples);
    
    //for (int ch = 0; ch < static_cast<int>(numChannels); ++ch)
    //    buffer.copyFrom(ch, 0, fxBuffer, ch, 0, static_cast<int>(numSamples));

    // Process Eq
    eq.process(buffer.getArrayOfWritePointers(), buffer.getArrayOfReadPointers(), numChannels, numSamples);
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