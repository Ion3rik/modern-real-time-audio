#include "PluginEditor.h"
#include "mrta_utils/Source/GUI/GenericParameterEditor.h"

MidiHandlerAudioProcessorEditor::MidiHandlerAudioProcessorEditor(MidiHandlerAudioProcessor& p) :
    juce::AudioProcessorEditor(p), audioProcessor(p),
    paramEditor(audioProcessor.getParamManager())
{
    addAndMakeVisible(paramEditor);
    setSize(300, 300);
}


MidiHandlerAudioProcessorEditor::~MidiHandlerAudioProcessorEditor()
{
}

//==============================================================================
void MidiHandlerAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MidiHandlerAudioProcessorEditor::resized()
{
    paramEditor.setBounds(getLocalBounds());
}