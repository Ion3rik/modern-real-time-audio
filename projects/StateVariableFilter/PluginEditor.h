#pragma once

#include "PluginProcessor.h"

class StateVariableFilterAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    StateVariableFilterAudioProcessorEditor(StateVariableFilterAudioProcessor&);
    ~StateVariableFilterAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    StateVariableFilterAudioProcessor& audioProcessor;
    mrta::GenericParameterEditor genericParameterEditor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StateVariableFilterAudioProcessorEditor)
};