/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DarkVelvetReverbAudioProcessorEditor::DarkVelvetReverbAudioProcessorEditor (DarkVelvetReverbAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), genericParameterEditor(audioProcessor.getParameterManager())
{
    int height = static_cast<int>(audioProcessor.getParameterManager().getParameters().size()) * genericParameterEditor.parameterWidgetHeight;
    setSize(300, height);
    addAndMakeVisible(genericParameterEditor);
}

DarkVelvetReverbAudioProcessorEditor::~DarkVelvetReverbAudioProcessorEditor()
{
}

//==============================================================================
void DarkVelvetReverbAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void DarkVelvetReverbAudioProcessorEditor::resized()
{
    genericParameterEditor.setBounds(getLocalBounds());
}
