#pragma once

#include <JuceHeader.h>
#include "Delay.h"
#include "Meter.h"

namespace Param
{
    namespace ID
    {
        static const juce::String Enabled { "enabled" };
        static const juce::String Mix { "mix" };
        static const juce::String Time { "time" };
        static const juce::String Feedback { "feedback" };
        static const juce::String Wow { "wow" };
        static const juce::String Tone { "tone" };
        static const juce::String Distortion { "distortion" };
    }

    namespace Name
    {
        static const juce::String Enabled { "Enabled" };
        static const juce::String Mix { "Mix" };
        static const juce::String Time { "Time" };
        static const juce::String Feedback { "Feedback" };
        static const juce::String Wow { "Wow" };
        static const juce::String Tone { "Tone" };
        static const juce::String Distortion { "Distortion" };
    }

    namespace Ranges
    {
        static constexpr float MixMin { 0.f };
        static constexpr float MixMax { 1.f };
        static constexpr float MixInc { 0.001f };
        static constexpr float MixSkw { 1.0f };

        static constexpr float TimeMin { 1.f };
        static constexpr float TimeMax { 2500.f };
        static constexpr float TimeInc { 0.01f };
        static constexpr float TimeSkw { 0.5f };

        static constexpr float FeedbackMin { 0.f };
        static constexpr float FeedbackMax { 1.f };
        static constexpr float FeedbackInc { 0.001f };
        static constexpr float FeedbackSkw { 1.0f };

        static constexpr float WowMin { 0.f };
        static constexpr float WowMax { 1.f };
        static constexpr float WowInc { 0.001f };
        static constexpr float WowSkw { 1.0f };

        static constexpr float ToneMin { 200.f };
        static constexpr float ToneMax { 20000.f };
        static constexpr float ToneInc { 1.f };
        static constexpr float ToneSkw { 0.4f };

        static constexpr float DistortionMin { 0.f };
        static constexpr float DistortionMax { 18.f };
        static constexpr float DistortionInc { 0.01f };
        static constexpr float DistortionSkw { 0.75f };

        static const juce::String EnabledOff { "Off" };
        static const juce::String EnabledOn { "On" };
    }

    namespace Units
    {
        static const juce::String Ms { "ms" };
        static const juce::String Hz { "Hz" };
        static const juce::String dB { "Hz" };
    }
}

class DelayAudioProcessor : public juce::AudioProcessor
{
public:
    DelayAudioProcessor();
    ~DelayAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    void releaseResources() override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    mrta::ParameterManager& getParameterManager() { return parameterManager; }
    DSP::Meter& getInputMeter() { return inputMeter; }
    DSP::Meter& getOutputMeter() { return outputMeter; }

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
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;
    //==============================================================================

    static const unsigned int MaxDelaySizeSamples { 1 << 12 };
    static const unsigned int MaxChannels { 2 };
    static const unsigned int MaxProcessBlockSamples{ 32 };

private:
    mrta::ParameterManager parameterManager;
    DSP::Delay delay;
    DSP::Meter inputMeter;
    DSP::Meter outputMeter;
    DSP::Ramp<float> wetRamp;
    DSP::Ramp<float> dryRamp;

    float enabled { 1.f };
    float mix { 0.5f };

    juce::AudioBuffer<float> fxBuffer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DelayAudioProcessor)
};