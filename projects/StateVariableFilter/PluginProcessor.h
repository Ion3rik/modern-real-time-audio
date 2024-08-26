#pragma once

#include <JuceHeader.h>

#include "StateVariableFilter.h"
#include "Ramp.h"

namespace Param
{
    namespace ID
    {
        static const juce::String Freq { "freq" };
        static const juce::String Reso { "reso" };
        static const juce::String Mode { "mode" };
    }

    namespace Name
    {
        static const juce::String Freq { "Frequency" };
        static const juce::String Reso { "Resonance" };
        static const juce::String Mode { "Mode" };
    }

    namespace Unit
    {
        static const juce::String Hz { "Hz" };
    }

    namespace Range
    {
        static constexpr float FreqMin { 100.f };
        static constexpr float FreqMax { 10000.f };
        static constexpr float FreqInc { 1.f };
        static constexpr float FreqSkw { 0.4f };

        static constexpr float ResoMin { 0.5f };
        static constexpr float ResoMax { 5.f };
        static constexpr float ResoInc { 0.01f };
        static constexpr float ResoSkw { 0.4f };

        static constexpr float ModeMin { -1.f };
        static constexpr float ModeMax { 1.f };
        static constexpr float ModeInc { 0.01f };
        static constexpr float ModeSkw { 1.f };
    }
}

class StateVariableFilterAudioProcessor : public juce::AudioProcessor
{
public:
    StateVariableFilterAudioProcessor();
    ~StateVariableFilterAudioProcessor() override;

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

    float freqHz { 1000.f };
    float reso { 0.7071f };
    float mode { 0.5f };

    DSP::StateVariableFilter svfLeft;
    DSP::StateVariableFilter svfRight;
    DSP::Ramp<float> freqRamp;
    DSP::Ramp<float> resoRamp;
    DSP::Ramp<float> lpfRamp;
    DSP::Ramp<float> bpfRamp;
    DSP::Ramp<float> hpfRamp;

    juce::AudioBuffer<float> freqInBuffer;
    juce::AudioBuffer<float> resoInBuffer;

    juce::AudioBuffer<float> lpfOutBuffer;
    juce::AudioBuffer<float> bpfOutBuffer;
    juce::AudioBuffer<float> hpfOutBuffer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StateVariableFilterAudioProcessor)
};