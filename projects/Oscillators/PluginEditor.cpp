#include "PluginProcessor.h"
#include "PluginEditor.h"

OscillatorsAudioProcessorEditor::OscillatorsAudioProcessorEditor(OscillatorsAudioProcessor& p) :
    AudioProcessorEditor(&p), audioProcessor(p),
    genericParameterEditor(audioProcessor.getParameterManager())
{
    addAndMakeVisible(genericParameterEditor);
    const int numOfParams { static_cast<int>(audioProcessor.getParameterManager().getParameters().size()) };
    setSize(300, numOfParams * genericParameterEditor.parameterWidgetHeight);
}

OscillatorsAudioProcessorEditor::~OscillatorsAudioProcessorEditor()
{
}

//==============================================================================
void OscillatorsAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void OscillatorsAudioProcessorEditor::resized()
{
    genericParameterEditor.setBounds(getLocalBounds());
}