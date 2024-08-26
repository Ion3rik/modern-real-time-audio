#pragma once

#include <JuceHeader.h>

#include "SynthVoice.h"
#include "mrta_utils/Source/Parameter/ParameterManager.h"

namespace Param
{

    namespace ID
    {
        static const juce::String AttRelTime { "att_rel_time" };
        static const juce::String WaveType { "wave_type" };
    }

    namespace Name
    {
        static const juce::String AttRelTime { "Att. Rel. Time" };
        static const juce::String WaveType { "Wave Type" };
    }

    namespace Ranges
    {
        static constexpr float AttRelTimeMin { 1.f };
        static constexpr float AttRelTimeMax { 1000.f };
        static constexpr float AttRelTimeInc { 0.1f };
        static constexpr float AttRelTimeSkw { 0.5f };

        static const juce::StringArray WaveType { "Sine", "Tri. Aliased", "Saw Aliased", "Tri. AA", "Saw AA" };
    }
}

class MidiHandlerAudioProcessor : public juce::AudioProcessor
{
public:
    MidiHandlerAudioProcessor();
    ~MidiHandlerAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    void releaseResources() override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    mrta::ParameterManager& getParamManager() { return paramManager; }

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
    mrta::ParameterManager paramManager;
    juce::Synthesiser synth;
    DSP::SynthVoice* voice { nullptr };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiHandlerAudioProcessor)
};