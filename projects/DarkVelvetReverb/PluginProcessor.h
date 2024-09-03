#pragma once

#include <JuceHeader.h>

#include "DarkVelvetReverb.h"
#include "RingMod.h"

namespace Param
{
    namespace ID
    {
        static const juce::String Room { "room" };
    }

    namespace Name
    {
        static const juce::String Room { "Room" };
    }

    namespace Range
    {
        static const juce::StringArray RoomLabels { "Room 1", "Room 2", "Room 3" };
    }
}

class DarkVelvetReverbAudioProcessor : public juce::AudioProcessor
{
public:
    DarkVelvetReverbAudioProcessor();
    ~DarkVelvetReverbAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    void releaseResources() override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    mrta::ParameterManager& getParameterManager() { return parameterManager; }

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    const juce::String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int) override;
    const juce::String getProgramName(int) override;
    void changeProgramName(int, const juce::String&) override;
    //==============================================================================

private:
    mrta::ParameterManager parameterManager;
    DSP::DarkVelvetReverb dvnReverb;
    Params::RirModel room;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DarkVelvetReverbAudioProcessor)
};