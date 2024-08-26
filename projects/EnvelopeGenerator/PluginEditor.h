#pragma once

#include "PluginProcessor.h"

class EnvelopeGeneratorAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    EnvelopeGeneratorAudioProcessorEditor(EnvelopeGeneratorAudioProcessor&);
    ~EnvelopeGeneratorAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    EnvelopeGeneratorAudioProcessor& audioProcessor;
    mrta::GenericParameterEditor genericParameterEditor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EnvelopeGeneratorAudioProcessorEditor)
};