/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DarkVelvetReverb.h"
#include "VelvetReverb.h"

//==============================================================================
/**
*/

namespace Param
{
    namespace ID
    {
        static const juce::String ReverberationTime { "reverberation_time" };
        static const juce::String NumPulses { "number_of_pulses" };
    }
    namespace Name
    {
        static const juce::String ReverberationTime { "Reverberation Time" };
        static const juce::String NumPulses { "Number of Pulses" };
    }
    namespace Ranges
    {
        
        static constexpr float ReverberationTimeMin {0.f};
        static constexpr float ReverberationTimeMax {5.f};
        static constexpr float ReverberationTimeInc {0.01f};
        static constexpr float ReverberationTimeSkw {0.5f};
    
        static constexpr unsigned int NumPulsesMin {1u};
        static constexpr unsigned int NumPulsesMax {1000u};
        static constexpr unsigned int NumPulsesInc {1u};
        static constexpr float NumPulsesSkw {0.5f};
    }
    namespace Units
    {
    static const juce::String S { "s" };
    
    }
}
class VelvetReverbAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    VelvetReverbAudioProcessor();
    ~VelvetReverbAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    
    mrta::ParameterManager& getParameterManager() { return parameterManager; }

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    mrta::ParameterManager parameterManager;
    DSP::DarkVelvetReverb velvetReverb;
    
    juce::AudioBuffer<float> fxBuffer;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VelvetReverbAudioProcessor)
};
