/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class GrainDelayAudioProcessorEditor : public juce::AudioProcessorEditor
//                                       public juce::Slider::Listener,
//                                       public::juce::Button::Listener,
//                                       public::juce::ComboBox::Listener
                                       //public::juce::Label::Listener

{
public:
    GrainDelayAudioProcessorEditor (GrainDelayAudioProcessor&);
    ~GrainDelayAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
//    void sliderValueChanged(Slider * slider) override;
//    void buttonClicked(Button * button) override;
//    void comboBoxChanged(ComboBox * comboBox) override;
    
    

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GrainDelayAudioProcessor& audioProcessor;
    
    // I want to pass in variables from my grainDelay class to my knobs, so do I do it this way?
    GrainDelay grainDelay;
    
    // create instances of knobs and sliders
    Slider delayKnob;
    ComboBox noteSelector;
    Slider feedbackKnob;
    Slider wetDryKnob;
    Slider grainSizeKnob;
    
    Label delayLabel;
    Label feedBackLabel;
    Label wetDryLabel;
    Label grainSizeLabel;

    ToggleButton tempoSyncButton;
//    ToggleButton notTempoSyncButton;
    
public:
    // Need to make slider attachment - by putting it at the bottom it gets destroyed first -destorys things from bottom to top in this window.
    // this is the one thing that funnels all your sliders to the value tree, dont need attachment for every slider, but if you other have types of interface options (boxes), youd need other attachments here
    // order is based on reverse order of our constructor, attachments should be destroyed befroe slide - could run into an issue without taht last stpe
    std::vector<std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>> sliderAttachments;
    std::vector<std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment>> buttonAttachments;
    std::vector<std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment>> comboBoxAttachments;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GrainDelayAudioProcessorEditor)
};
