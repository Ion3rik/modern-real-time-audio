#include "PluginProcessor.h"
#include "PluginEditor.h"

DarkVelvetReverbAudioProcessorEditor::DarkVelvetReverbAudioProcessorEditor(DarkVelvetReverbAudioProcessor& p) :
    AudioProcessorEditor(&p), audioProcessor(p),
    genericParameterEditor(audioProcessor.getParameterManager())
{
    addAndMakeVisible(genericParameterEditor);
    const int numOfParams { static_cast<int>(audioProcessor.getParameterManager().getParameters().size()) };
    setSize(300, numOfParams * genericParameterEditor.parameterWidgetHeight);
}

DarkVelvetReverbAudioProcessorEditor::~DarkVelvetReverbAudioProcessorEditor()
{
}

//==============================================================================
void DarkVelvetReverbAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void DarkVelvetReverbAudioProcessorEditor::resized()
{
    genericParameterEditor.setBounds(getLocalBounds());
}