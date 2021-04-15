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
    setSize (400, 300);
    
    // Delay Time Slider
    delayKnob.setRange(10.f, 1024.f, 1.f); //min,max,increment size
    delayKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 75, 25);
    delayKnob.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    addAndMakeVisible(delayKnob);
        // Delay label
    delayLabel.setText("Delay", dontSendNotification);
    delayLabel.attachToComponent(&delayKnob, false);
    delayLabel.setJustificationType(Justification::centredBottom);
    delayLabel.setBorderSize(BorderSize<int>(1));
    addAndMakeVisible(delayLabel);
    
    // Not tempo-synced button
//    notTempoSyncButton.setBounds(35, 170, 100, 40);
//    notTempoSyncButton.setButtonText("Sync off");
//    notTempoSyncButton.setToggleState(!audioProcessor.tempoSyncd, dontSendNotification);
//    notTempoSyncButton.setRadioGroupId(1);
//    addAndMakeVisible(notTempoSyncButton);
    
    // Tempo Sync button
    tempoSyncButton.setButtonText("Sync'd");
    tempoSyncButton.setRadioGroupId(1); // listens to 'not tempo synced button' and they turn each other off
    addAndMakeVisible(tempoSyncButton);
    
    // Combo Box for a note selector
    noteSelector.addItem("Half", 1);
    noteSelector.addItem("Quarter", 2);
    noteSelector.addItem("8th", 3);
    noteSelector.addItem("16th", 4);
    noteSelector.addItem("32nd", 5);
    noteSelector.setSelectedId(2); // Which note do I want to be initialized upon the plugin opening?
    noteSelector.setBounds(25, 235, 100, 40);
    addAndMakeVisible(noteSelector);
    
    // Knob for Feedback Amount
    feedbackKnob.setRange(0.f, 1.f, .01f); //min,max,increment size
    feedbackKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 15);
    feedbackKnob.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    addAndMakeVisible(feedbackKnob);
        // Feedback label
    feedBackLabel.setText("Feedback", dontSendNotification);
    feedBackLabel.attachToComponent(&feedbackKnob, false);
    feedBackLabel.setJustificationType(Justification::centredBottom);
    feedBackLabel.setBorderSize(BorderSize<int>(1));
    addAndMakeVisible(feedBackLabel);
    
    // Knob for wet/dry amount
    wetDryKnob.setRange(0.f, 1.f, .01f);
    wetDryKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 15);
    wetDryKnob.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    addAndMakeVisible(wetDryKnob);
        // wet/dry label
    wetDryLabel.setText("Dry / Wet", dontSendNotification);
    wetDryLabel.attachToComponent(&wetDryKnob, false);
    wetDryLabel.setJustificationType(Justification::centredBottom);
    addAndMakeVisible(feedBackLabel);
    
        // Grain Size Slider
    grainSizeKnob.setRange(10.f, 1024.f, 1.f); //min,max,increment size
    grainSizeKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 15);
    grainSizeKnob.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    addAndMakeVisible(grainSizeKnob);
        // Grain size label
    grainSizeLabel.setText("Grain Size", dontSendNotification);
    grainSizeLabel.attachToComponent(&grainSizeKnob, false);
    grainSizeLabel.setJustificationType(Justification::centredBottom);
    addAndMakeVisible(grainSizeLabel);
    
    // Value Tree Sliders
    sliderAttachments.emplace_back(new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.state,"delayMS",delayKnob));
    sliderAttachments.emplace_back(new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.state,"grainSize",grainSizeKnob));
    sliderAttachments.emplace_back(new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.state,"feedbackAmount",feedbackKnob));
    sliderAttachments.emplace_back(new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.state,"wetDryAmount",wetDryKnob));
    
    // Value Tree Combobox + button
    buttonAttachments.emplace_back(new AudioProcessorValueTreeState::ButtonAttachment(audioProcessor.state,"tempoSyncd",tempoSyncButton));
    comboBoxAttachments.emplace_back(new AudioProcessorValueTreeState::ComboBoxAttachment(audioProcessor.state,"noteSelect",noteSelector));

}

GrainDelayAudioProcessorEditor::~GrainDelayAudioProcessorEditor()
{
}

//==============================================================================
void GrainDelayAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::teal);
    g.setFont (20.0f);
    g.drawFittedText ("Grain Delay", getLocalBounds(), juce::Justification::centredTop, 1);
}

void GrainDelayAudioProcessorEditor::resized()
{
    tempoSyncButton.setBounds(35, 195, 100, 40);
    grainSizeKnob.setBounds(210,150,100,100);
    wetDryKnob.setBounds(250, 25, 70, 70);
    feedbackKnob.setBounds(170,25,70,70);
    delayKnob.setBounds(12,45,125,125);


}


//void GrainDelayAudioProcessorEditor::sliderValueChanged(Slider *slider) {
//    
//    if (slider == &delayKnob) {
////        grainDelay.delay = delayKnob.getValue();
//        audioProcessor.delay = delayKnob.getValue();
//    }
//    if (slider == &feedbackKnob) {
////        grainDelay.feedbackAmount = feedbackKnob.getValue();
//        audioProcessor.feedbackAmount = feedbackKnob.getValue();
//
//    }
//    if (slider == &grainSizeKnob) {
//        audioProcessor.grainSize = grainSizeKnob.getValue();
////        audioProcessor.grainSize = grainSizeKnob.getValue();
//    }
//    
//}
//
//void GrainDelayAudioProcessorEditor::buttonClicked(Button *button) {
//    if (button == &tempoSyncButton) {
//        audioProcessor.tempoSyncd = true;
//        delayKnob.setEnabled(false);
//        noteSelector.setEnabled(true); // when flipping the state, change what you want to be greyed out and not
//    }
//    if (button == &notTempoSyncButton) {
//        audioProcessor.tempoSyncd = false;
//        delayKnob.setEnabled(true);
//        noteSelector.setEnabled(false);
//    }
//}
//
//void GrainDelayAudioProcessorEditor::comboBoxChanged(ComboBox *comboBox) {
//    if (comboBox == &noteSelector) {
//        if (noteSelector.getSelectedId() == 1){
//            // half-note
//            audioProcessor.noteDuration = 2.f;
//        }
//        if (noteSelector.getSelectedId() == 2){
//            // Quarter
//            audioProcessor.noteDuration = 1.f;
//        }
//        if (noteSelector.getSelectedId() == 3){
//            // 8th
//            audioProcessor.noteDuration = 0.5f;
//        }
//        if (noteSelector.getSelectedId() == 4){
//            // 16th
//            audioProcessor.noteDuration = 0.25f;
//        }
//    }
//}


