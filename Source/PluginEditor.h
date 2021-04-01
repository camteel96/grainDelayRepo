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
class GrainDelayAudioProcessorEditor : public juce::AudioProcessorEditor,
                                       public juce::Slider::Listener,
                                       public::juce::Button::Listener,
                                       public::juce::ComboBox::Listener
                                       //public::juce::Label::Listener

{
public:
    GrainDelayAudioProcessorEditor (GrainDelayAudioProcessor&);
    ~GrainDelayAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void sliderValueChanged(Slider * slider) override;
    void buttonClicked(Button * button) override;
    void comboBoxChanged(ComboBox * comboBox) override;
    
    //LookAndFeel_V4();
    
    

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
    Slider adsrAttack;
    Slider adsrDecay;
    Slider adsrSustain;
    Slider adsrRelease;
    
    Label delayLabel;
    Label feedBackLabel;
    Label wetDryLabel;
    Label grainSizeLabel;
    
    Label attackLabel;
    Label decayLabel;
    Label sustainLabel;
    Label releaseLabel;
    
    ToggleButton tempoSyncButton;
    ToggleButton notTempoSyncButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GrainDelayAudioProcessorEditor)
};
