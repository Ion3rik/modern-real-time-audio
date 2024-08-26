#include "PluginProcessor.h"
#include "PluginEditor.h"

StateVariableFilterAudioProcessorEditor::StateVariableFilterAudioProcessorEditor(StateVariableFilterAudioProcessor& p) :
    AudioProcessorEditor(p), audioProcessor(p),
    genericParameterEditor(audioProcessor.getParameterManager())
{
    addAndMakeVisible(genericParameterEditor);
    const int numOfParams { static_cast<int>(audioProcessor.getParameterManager().getParameters().size()) };
    setSize(300, numOfParams * genericParameterEditor.parameterWidgetHeight);
}

StateVariableFilterAudioProcessorEditor::~StateVariableFilterAudioProcessorEditor()
{
}

//==============================================================================
void StateVariableFilterAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void StateVariableFilterAudioProcessorEditor::resized()
{
    genericParameterEditor.setBounds(getLocalBounds());
}