/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Components/LargeKnob.h"
#include "Components/SmallKnob.h"


//==============================================================================
/**
*/
class GrainDelayAudioProcessorEditor : public juce::AudioProcessorEditor

{
public:
    GrainDelayAudioProcessorEditor (GrainDelayAudioProcessor&);
    ~GrainDelayAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GrainDelayAudioProcessor& audioProcessor;
    
    // Background image
    Image bgImage;
    LargeKnob largeKnob; // class for look and feel
    SmallKnob smallKnob;
    LookAndFeel_V3 lookAndFeel3;
        
    // create instances of knobs and sliders
    Slider delayKnob;
    ComboBox noteSelector;
    Slider feedbackKnob;
    Slider wetDryKnob;
    
    Label delayLabel;
    Label feedBackLabel;
    Label wetDryLabel;
    Label grainSizeLabel;
    Label temposyncLabel;

    ToggleButton tempoSyncButton;
    
public:
    // Need to make slider attachment - by putting it at the bottom it gets destroyed first -destorys things from bottom to top in this window.
    // this is the one thing that funnels all your sliders to the value tree, dont need attachment for every slider, but if you other have types of interface options (boxes), youd need other attachments here
    // order is based on reverse order of our constructor, attachments should be destroyed befroe slide - could run into an issue without taht last stpe
    std::vector<std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>> sliderAttachments;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> buttonAttachment;
    std::vector<std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment>> comboBoxAttachments;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GrainDelayAudioProcessorEditor)
};
