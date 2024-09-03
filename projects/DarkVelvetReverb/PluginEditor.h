#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class DarkVelvetReverbAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    DarkVelvetReverbAudioProcessorEditor(DarkVelvetReverbAudioProcessor&);
    ~DarkVelvetReverbAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    DarkVelvetReverbAudioProcessor& audioProcessor;
    mrta::GenericParameterEditor genericParameterEditor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DarkVelvetReverbAudioProcessorEditor)
};