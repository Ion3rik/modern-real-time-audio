#include "PluginProcessor.h"
#include "Meter.h"
#include "PluginEditor.h"

#include <algorithm>

static const std::vector<mrta::ParameterInfo> Parameters
{
    { Param::ID::Enabled,    Param::Name::Enabled,    Param::Ranges::EnabledOff, Param::Ranges::EnabledOn, true },
    { Param::ID::Mix,        Param::Name::Mix,        Param::Units::Ms,  0.5f,   Param::Ranges::MixMin,        Param::Ranges::MixMax,        Param::Ranges::MixInc,        Param::Ranges::MixSkw },
    { Param::ID::Time,       Param::Name::Time,       Param::Units::Ms,  500.f,  Param::Ranges::TimeMin,       Param::Ranges::TimeMax,       Param::Ranges::TimeInc,       Param::Ranges::TimeSkw },
    { Param::ID::Feedback,   Param::Name::Feedback,   "",                0.5f,   Param::Ranges::FeedbackMin,   Param::Ranges::FeedbackMax,   Param::Ranges::FeedbackInc,   Param::Ranges::FeedbackSkw },
    { Param::ID::Wow,        Param::Name::Wow,        "",                0.5f,   Param::Ranges::WowMin,        Param::Ranges::WowMax,        Param::Ranges::WowInc,        Param::Ranges::WowSkw },
    { Param::ID::Tone,       Param::Name::Tone,       Param::Units::Hz,  5000.f, Param::Ranges::ToneMin,       Param::Ranges::ToneMax,       Param::Ranges::ToneInc,       Param::Ranges::ToneSkw },
    { Param::ID::Distortion, Param::Name::Distortion, Param::Units::dB,  3.f,    Param::Ranges::DistortionMin, Param::Ranges::DistortionMax, Param::Ranges::DistortionInc, Param::Ranges::DistortionSkw }
};

DelayAudioProcessor::DelayAudioProcessor() :
    parameterManager(*this, ProjectInfo::projectName, Parameters),
    delay(Param::Ranges::TimeMax, 2),
    wetRamp(0.05f),
    dryRamp(0.05f)
{
    parameterManager.registerParameterCallback(Param::ID::Enabled,
    [this](float newValue, bool force)
    {
        enabled = newValue;
        wetRamp.setTarget(std::clamp(enabled * mix, 0.f, 1.f), force);
        dryRamp.setTarget(std::clamp((1.f - mix) * enabled + (1.f - enabled), 0.f, 1.f), force);
    });

    parameterManager.registerParameterCallback(Param::ID::Mix,
    [this] (float value, bool force)
    {
        mix = value;
        wetRamp.setTarget(std::clamp(enabled * mix, 0.f, 1.f), force);
        dryRamp.setTarget(std::clamp((1.f - mix) * enabled + (1.f - enabled), 0.f, 1.f), force);
    });

    parameterManager.registerParameterCallback(Param::ID::Time,
    [this] (float value, bool /*force*/)
    {
        delay.setDelayTime(value);
    });

    parameterManager.registerParameterCallback(Param::ID::Feedback,
    [this] (float value, bool /*force*/)
    {
        delay.setFeedback(value);
    });

    parameterManager.registerParameterCallback(Param::ID::Wow,
    [this] (float value, bool /*force*/)
    {
        delay.setWow(value);
    });

    parameterManager.registerParameterCallback(Param::ID::Tone,
    [this] (float value, bool /*force*/)
    {
        delay.setToneFrequency(value);
    });

    parameterManager.registerParameterCallback(Param::ID::Distortion,
    [this] (float value, bool /*force*/)
    {
        delay.setDistortion(value);
    });
}

DelayAudioProcessor::~DelayAudioProcessor()
{
}

void DelayAudioProcessor::prepareToPlay(double newSampleRate, int samplesPerBlock)
{
    const unsigned int numChannels { static_cast<unsigned int>(std::max(getMainBusNumInputChannels(), getMainBusNumOutputChannels())) };

    delay.prepare(newSampleRate, Param::Ranges::TimeMax, numChannels);
    inputMeter.prepare(newSampleRate, numChannels);
    outputMeter.prepare(newSampleRate, numChannels);
    wetRamp.prepare(newSampleRate);
    dryRamp.prepare(newSampleRate);

    parameterManager.updateParameters(true);

    fxBuffer.setSize(static_cast<int>(numChannels), samplesPerBlock);
    fxBuffer.clear();
}

void DelayAudioProcessor::releaseResources()
{
    delay.clear();
}

void DelayAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& /*midiMessages*/)
{
    juce::ScopedNoDenormals noDenormals;
    parameterManager.updateParameters();

    const unsigned int numChannels { static_cast<unsigned int>(buffer.getNumChannels()) };
    const unsigned int numSamples { static_cast<unsigned int>(buffer.getNumSamples()) };

    inputMeter.process(buffer.getArrayOfReadPointers(), numChannels, numSamples);

    for (int ch = 0; ch < static_cast<int>(numChannels); ++ch)
        fxBuffer.copyFrom(ch, 0, buffer, ch, 0, static_cast<int>(numSamples));

    delay.process(fxBuffer.getArrayOfWritePointers(), fxBuffer.getArrayOfReadPointers(), numChannels, numSamples);
    wetRamp.applyGain(fxBuffer.getArrayOfWritePointers(), numChannels, numSamples);
    dryRamp.applyGain(buffer.getArrayOfWritePointers(), numChannels, numSamples);

    for (int ch = 0; ch < static_cast<int>(numChannels); ++ch)
        buffer.addFrom(ch, 0, fxBuffer, ch, 0, static_cast<int>(numSamples));

    outputMeter.process(buffer.getArrayOfReadPointers(), numChannels, numSamples);
}

void DelayAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    parameterManager.getStateInformation(destData);
}

void DelayAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    parameterManager.setStateInformation(data, sizeInBytes);
}

//==============================================================================
bool DelayAudioProcessor::hasEditor() const { return true; }
juce::AudioProcessorEditor* DelayAudioProcessor::createEditor() { return new DelayAudioProcessorEditor(*this); }
const juce::String DelayAudioProcessor::getName() const { return JucePlugin_Name; }
bool DelayAudioProcessor::acceptsMidi() const { return false; }
bool DelayAudioProcessor::producesMidi() const { return false; }
bool DelayAudioProcessor::isMidiEffect() const { return false; }
double DelayAudioProcessor::getTailLengthSeconds() const { return 0.0; }
int DelayAudioProcessor::getNumPrograms() { return 1; }
int DelayAudioProcessor::getCurrentProgram() { return 0; }
void DelayAudioProcessor::setCurrentProgram(int) { }
const juce::String DelayAudioProcessor::getProgramName (int) { return {}; }
void DelayAudioProcessor::changeProgramName (int, const juce::String&) { }
//==============================================================================

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayAudioProcessor();
}