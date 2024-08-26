#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class OscillatorsAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    OscillatorsAudioProcessorEditor(OscillatorsAudioProcessor&);
    ~OscillatorsAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    OscillatorsAudioProcessor& audioProcessor;
    mrta::GenericParameterEditor genericParameterEditor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscillatorsAudioProcessorEditor)
};