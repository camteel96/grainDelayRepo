/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "GrainDelay.h"

//==============================================================================
/**
*/
class GrainDelayAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    GrainDelayAudioProcessor();
    ~GrainDelayAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    float delayMS = 100.f;
    float noteDuration = 1.f;
    GrainDelay::NoteSelection noteSelect = GrainDelay::NoteSelection::QUARTER;
    bool tempoSyncd = true;
    
//    float feedbackAmount = 50.f;
//    float wetDryAmount = 1.f; // [0-100%]
//    int grainSize = 1024.f;
    // ADSR hard coded lengths
//    float adsrAttack = 10.f;
//    float adsrDecay = 10.f;
//    float adsrSustain = 100.f;
//    float adsrRelease = 500.f;
    
    // Begin value tree state code
    AudioProcessorValueTreeState state; // state that contains all the attributes of plugin
    // Calling function for parameter layout that belongs to this value
    AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    

private:
    
    
    GrainDelay grainDelay;
    
    //ADSR adsr;
    
    AudioPlayHead* playHead;
    AudioPlayHead::CurrentPositionInfo currentPositionInfo;
    
    float bpm = 120.f;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GrainDelayAudioProcessor)
};
