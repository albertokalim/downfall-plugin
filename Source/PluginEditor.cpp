/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DownfallPluginAudioProcessorEditor::DownfallPluginAudioProcessorEditor (DownfallPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    fileChooserButton.setButtonText("Load IR");
    fileChooserButton.setBounds(0, 0, 70, 27);

    fileChooserButton.onClick = [this] {
        fileChooser = std::make_unique<juce::FileChooser>("Select an IR File to load...",
            juce::File::getSpecialLocation(juce::File::userHomeDirectory),
            "*.wav");

        fileChooser->launchAsync(juce::FileBrowserComponent::openMode, [this](const juce::FileChooser& chooser)
            {
                juce::File newIRFile(chooser.getResult());
                audioProcessor.setIRToConvolution(newIRFile);
            });
        };

    addAndMakeVisible(fileChooserButton);
    setSize (400, 300);
}

DownfallPluginAudioProcessorEditor::~DownfallPluginAudioProcessorEditor()
{
}

//==============================================================================
void DownfallPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
}

void DownfallPluginAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();

    fileChooserButton.setTopLeftPosition(bounds.getCentreX() - fileChooserButton.getWidth() / 2, bounds.getCentreY() - fileChooserButton.getWidth() / 2);
}
