#include "PluginProcessor.h"
#include "PluginEditor.h"

EnvelopeGeneratorAudioProcessorEditor::EnvelopeGeneratorAudioProcessorEditor(EnvelopeGeneratorAudioProcessor& p) :
    AudioProcessorEditor(p), audioProcessor(p),
    genericParameterEditor(audioProcessor.getParameterManager())
{
    addAndMakeVisible(genericParameterEditor);
    const int numOfParams { static_cast<int>(audioProcessor.getParameterManager().getParameters().size()) };
    setSize(300, numOfParams * genericParameterEditor.parameterWidgetHeight);
}

EnvelopeGeneratorAudioProcessorEditor::~EnvelopeGeneratorAudioProcessorEditor()
{
}

//==============================================================================
void EnvelopeGeneratorAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void EnvelopeGeneratorAudioProcessorEditor::resized()
{
    genericParameterEditor.setBounds(getLocalBounds());
}