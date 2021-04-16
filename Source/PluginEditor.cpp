/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GrainDelayAudioProcessorEditor::GrainDelayAudioProcessorEditor (GrainDelayAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize(400,300);
    bgImage = ImageCache::getFromMemory(BinaryData::metalbackground_jpg, BinaryData::metalbackground_jpgSize);
    
    // Delay Time Slider
    delayKnob.setRange(10.f, 1024.f, 1.f); //min,max,increment size
    delayKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 15);
    delayKnob.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    delayKnob.setLookAndFeel(&largeKnob);
    addAndMakeVisible(delayKnob);
        // Delay label
    delayLabel.setText("Delay", dontSendNotification);
    delayLabel.setColour(Label::textColourId, Colours::orangered);
    delayLabel.attachToComponent(&delayKnob, false);
    delayLabel.setJustificationType(Justification::centredBottom);
    delayLabel.setBorderSize(BorderSize<int>(1));
    addAndMakeVisible(delayLabel);
    
    // Tempo Sync button
    tempoSyncButton.setButtonText("Sync'd");
    tempoSyncButton.setColour(TextButton::buttonColourId, Colours::dimgrey);
    tempoSyncButton.setColour(TextButton::buttonOnColourId, Colours::orangered);
    tempoSyncButton.setClickingTogglesState(true);
    tempoSyncButton.onClick = [this]() {};
    addAndMakeVisible(tempoSyncButton);
    
    // Combo Box for a note selector
    noteSelector.addItem("Half", 1);
    noteSelector.addItem("Quarter", 2);
    noteSelector.addItem("8th", 3);
    noteSelector.addItem("16th", 4);
    noteSelector.addItem("32nd", 5);
    noteSelector.addItem("64th",6);
    noteSelector.setSelectedId(2); // Which note do I want to be initialized upon the plugin opening?
    addAndMakeVisible(noteSelector);
    
    // Knob for Feedback Amount
    feedbackKnob.setRange(0.f, 1.f, .01f); //min,max,increment size
    feedbackKnob.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    feedbackKnob.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    feedbackKnob.setLookAndFeel(&smallKnob);
    addAndMakeVisible(feedbackKnob);
        // Feedback label
    feedBackLabel.setText("Feedback", dontSendNotification);
    feedBackLabel.setColour(Label::textColourId, Colours::orangered);
    feedBackLabel.attachToComponent(&feedbackKnob, false);
    feedBackLabel.setJustificationType(Justification::centredBottom);
    feedBackLabel.setBorderSize(BorderSize<int>(1));
    addAndMakeVisible(feedBackLabel);
    
        // Knob for wet/dry amount
    wetDryKnob.setRange(0.f, 1.f, .01f);
    wetDryKnob.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    wetDryKnob.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    wetDryKnob.setLookAndFeel(&smallKnob);
    addAndMakeVisible(wetDryKnob);
        // wet/dry label
    wetDryLabel.setText("Dry / Wet", dontSendNotification);
    wetDryLabel.setColour(Label::textColourId, Colours::orangered);
    wetDryLabel.attachToComponent(&wetDryKnob, false);
    wetDryLabel.setJustificationType(Justification::centredBottom);
    addAndMakeVisible(feedBackLabel);
    
    // Value Tree Sliders
    sliderAttachments.emplace_back(new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.state,"delayMS",delayKnob));
    sliderAttachments.emplace_back(new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.state,"feedbackAmount",feedbackKnob));
    sliderAttachments.emplace_back(new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.state,"wetDryAmount",wetDryKnob));
    
    // Value Tree Combobox + button
    buttonAttachment = std::make_unique<AudioProcessorValueTreeState::ButtonAttachment> (audioProcessor.state,"tempoSyncd",tempoSyncButton);
    comboBoxAttachments.emplace_back(new AudioProcessorValueTreeState::ComboBoxAttachment(audioProcessor.state,"noteSelect",noteSelector));

}

GrainDelayAudioProcessorEditor::~GrainDelayAudioProcessorEditor()
{
}

//==============================================================================
void GrainDelayAudioProcessorEditor::paint (juce::Graphics& g)
{

    g.drawImageAt(bgImage, 0, 0);
    g.setColour (juce::Colours::orangered);
    g.setFont (30.0f);
    g.drawFittedText ("Grain Delay", getLocalBounds(), juce::Justification::centredTop, 1);
}

void GrainDelayAudioProcessorEditor::resized()
{
    tempoSyncButton.setBounds(190, 60, 130, 70);
    noteSelector.setBounds(270, 75, 100, 40);
    feedbackKnob.setBounds(180,150,75,75);
    wetDryKnob.setBounds(270, 150, 75, 75);
    delayKnob.setBounds(40,100,125,135);

}



