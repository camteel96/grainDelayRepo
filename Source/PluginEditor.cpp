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
    // Look up code to see what you can do to have some fun here! Use Tarr's code as the placeholder
    setSize (400, 300);
    
    // Delay Time Slider
    delayKnob.addListener(this);
    delayKnob.setBounds(12,45,125,125);
    delayKnob.setValue(audioProcessor.delay);
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
    notTempoSyncButton.addListener(this);
    notTempoSyncButton.setBounds(35, 170, 100, 40);
    notTempoSyncButton.setButtonText("Sync off");
    notTempoSyncButton.setToggleState(!audioProcessor.tempoSyncd, dontSendNotification);
    notTempoSyncButton.setRadioGroupId(1);
    addAndMakeVisible(notTempoSyncButton);
    
    // Tempo Sync button
    tempoSyncButton.addListener(this);
    tempoSyncButton.setBounds(35, 195, 100, 40);
    tempoSyncButton.setButtonText("Sync'd");
    tempoSyncButton.setToggleState(audioProcessor.tempoSyncd, dontSendNotification);
    tempoSyncButton.setRadioGroupId(1); // listens to 'not tempo synced button' and they turn each other off
    addAndMakeVisible(tempoSyncButton);
    
    // Combo Box for a note selector
    noteSelector.addListener(this);
    noteSelector.addItem("Half", 1);
    noteSelector.addItem("Quarter", 2);
    noteSelector.addItem("8th", 3);
    noteSelector.addItem("16th", 4);
    noteSelector.setSelectedId(2); // Which note do I want to be initialized upon the plugin opening?
    noteSelector.setBounds(25, 235, 100, 40);
    addAndMakeVisible(noteSelector);
    
    delayKnob.setEnabled(!audioProcessor.tempoSyncd);
    noteSelector.setEnabled(audioProcessor.tempoSyncd);
    
    // Knob for Feedback Amount
    feedbackKnob.addListener(this);
    feedbackKnob.setBounds(170,25,70,70);
    feedbackKnob.setValue(audioProcessor.feedbackAmount);
    feedbackKnob.setRange(0.f, 100.f, 1.f); //min,max,increment size
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
    wetDryKnob.addListener(this);
    wetDryKnob.setBounds(250, 25, 70, 70);
    wetDryKnob.setValue(audioProcessor.wetDryAmount);
    wetDryKnob.setRange(0.f, 100.f, 1.f);
    wetDryKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 15);
    wetDryKnob.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    addAndMakeVisible(wetDryKnob);
        // wet/dry label
    wetDryLabel.setText("Wet / Dry", dontSendNotification);
    wetDryLabel.attachToComponent(&wetDryKnob, false);
    wetDryLabel.setJustificationType(Justification::centredBottom);
    addAndMakeVisible(feedBackLabel);
    
    
    // Grain Size Slider
    grainSizeKnob.addListener(this);
    grainSizeKnob.setBounds(210,120,70,70);
    grainSizeKnob.setValue(audioProcessor.grainSize);
    grainSizeKnob.setRange(10.f, 1024.f, 1.f); //min,max,increment size
    grainSizeKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 15);
    grainSizeKnob.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    addAndMakeVisible(grainSizeKnob);
        // Grain size label
    grainSizeLabel.setText("Grain Size", dontSendNotification);
    grainSizeLabel.attachToComponent(&grainSizeKnob, false);
    grainSizeLabel.setJustificationType(Justification::centredBottom);
    addAndMakeVisible(grainSizeLabel);
    
//        // Multiple Sliders for ADSR Settings //
//    adsrAttack.addListener(this);
//    adsrAttack.setBounds(140,215,80,80);
//    adsrAttack.setValue(audioProcessor.adsrAttack);
//    adsrAttack.setRange(10.f, 100.f, 1.f); //min,max,increment size
//    adsrAttack.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 15);
//    adsrAttack.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
//    addAndMakeVisible(adsrAttack);
//        // Attack Label
//    attackLabel.setText("Attack", dontSendNotification);
//    attackLabel.attachToComponent(&adsrAttack, false);
//    attackLabel.setJustificationType(Justification::centredBottom);
//    addAndMakeVisible(attackLabel);
//
//    adsrDecay.addListener(this);
//    adsrDecay.setBounds(195,215,80,80);
//    adsrDecay.setValue(audioProcessor.adsrDecay);
//    adsrDecay.setRange(10.f, 100.f, 1.f); //min,max,increment size
//    adsrDecay.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 15);
//    adsrDecay.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
//    addAndMakeVisible(adsrDecay);
//        // Decay Label
//    decayLabel.setText("Decay", dontSendNotification);
//    decayLabel.attachToComponent(&adsrDecay, false);
//    decayLabel.setJustificationType(Justification::centredBottom);
//    addAndMakeVisible(decayLabel);
//
//    adsrSustain.addListener(this);
//    adsrSustain.setBounds(250,215,80,80);
//    adsrSustain.setValue(audioProcessor.adsrSustain);
//    adsrSustain.setRange(10.f, 500.f, 1.f); //min,max,increment size
//    adsrSustain.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 15);
//    adsrSustain.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
//    addAndMakeVisible(adsrSustain);
//        // Sustain Label
//    sustainLabel.setText("Sustain", dontSendNotification);
//    sustainLabel.attachToComponent(&adsrSustain, false);
//    sustainLabel.setJustificationType(Justification::centredBottom);
//    addAndMakeVisible(sustainLabel);
//
//    adsrRelease.addListener(this);
//    adsrRelease.setBounds(305,215,80,80);
//    adsrRelease.setValue(audioProcessor.adsrRelease);
//    adsrRelease.setRange(10.f, 1024.f, 1.f); //min,max,increment size
//    adsrRelease.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 15);
//    adsrRelease.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
//    addAndMakeVisible(adsrRelease);
//        // Release Label
//    releaseLabel.setText("Release", dontSendNotification);
//    releaseLabel.attachToComponent(&adsrRelease, false);
//    releaseLabel.setJustificationType(Justification::centredBottom);
//    addAndMakeVisible(releaseLabel);
    
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
    g.drawFittedText ("My GrainDelay Plugin", getLocalBounds(), juce::Justification::centredTop, 1);
}

void GrainDelayAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}


void GrainDelayAudioProcessorEditor::sliderValueChanged(Slider *slider) {
    
    if (slider == &delayKnob) {
//        grainDelay.delay = delayKnob.getValue();
        audioProcessor.delay = delayKnob.getValue();
    }
    if (slider == &feedbackKnob) {
//        grainDelay.feedbackAmount = feedbackKnob.getValue();
        audioProcessor.feedbackAmount = feedbackKnob.getValue();

    }
    if (slider == &grainSizeKnob) {
        audioProcessor.grainSize = grainSizeKnob.getValue();
//        audioProcessor.grainSize = grainSizeKnob.getValue();
    }
    
}

void GrainDelayAudioProcessorEditor::buttonClicked(Button *button) {
    if (button == &tempoSyncButton) {
        audioProcessor.tempoSyncd = true;
        delayKnob.setEnabled(false);
        noteSelector.setEnabled(true); // when flipping the state, change what you want to be greyed out and not
    }
    if (button == &notTempoSyncButton) {
        audioProcessor.tempoSyncd = false;
        delayKnob.setEnabled(true);
        noteSelector.setEnabled(false);
    }
}

void GrainDelayAudioProcessorEditor::comboBoxChanged(ComboBox *comboBox) {
    if (comboBox == &noteSelector) {
        if (noteSelector.getSelectedId() == 1){
            // half-note
            audioProcessor.noteDuration = 2.f;
        }
        if (noteSelector.getSelectedId() == 2){
            // Quarter
            audioProcessor.noteDuration = 1.f;
        }
        if (noteSelector.getSelectedId() == 3){
            // 8th
            audioProcessor.noteDuration = 0.5f;
        }
        if (noteSelector.getSelectedId() == 4){
            // 16th
            audioProcessor.noteDuration = 0.25f;
        }
    }
}


