/*
  ==============================================================================

    GrainDelay.h
    Created: 27 Feb 2021 3:30:33pm
    Author:  Cameron Teel

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <cmath>

using namespace std;

class GrainDelay {
    
public:
    // Keep an eye on what we do and don't need here

    // Constructor
    GrainDelay();
    
    GrainDelay(float delay);
    
    enum NoteSelection { WHOLE, HALF, QUARTER, EIGHTH, SIXTEENTH };
    
    void processSignal(float * signal, const int numSamples, const int channel);
    
    //void prepare(float newFs);
    
    float processSample(float x, int channel);
    
    void setFs(float Fs);
    
    //void setDelayMS(float newDelayMS);
    void setDelaySamples(float delay);
    
    void setGrainSize(int grainSize);
    
    void setBPM(float newBPM);
    void setNoteDuration(NoteSelection newNoteSelection);
    
    void setWetDryAmount(float newWetDryAmount);
    
    // Destructor - began not building because of this
    //~GrainDelay();
    
    // Things that I'm tyring to change with knobs, so they need to be public? These also reside in pluginProcessor.h
//    float delay = 5.f; // samples
//    float feedbackAmount = 0.5f; // [0-1.0]
//    //const static int grainSize = 1024;
//    float wetDryAmount = 1.00f; //[0-100%]

    
private:
    
    float delay = 5.f; // samples
    float feedbackAmount = 0.5f; // [0-1.0]
    int grainSize = 1024;
    float wetDryAmount = 1.00f; //[0-100%]
//    float delay = 5.f; // samples
//    float feedbackAmount = 0.5f; // [0-1.0]
//    int grainSize = MAX_BUFFERSIZE;
//    float wetDryAmount = 1.00f; //[0-100%]

    
    float Fs = 48000.f;
    
    float bpm = 120.f;
    
    float noteDuration = 1.f; // 1 - quarter , 2-half , 0.5-eighth , 0.25 sixteenth
    NoteSelection noteSelect = QUARTER;
    
    //float delay = 5.f; // samples
    //int delaySamples = round(noteDuration*Fs*delayMS/1000.f);
    
    //float wetDryAmount = 1.00f; //[0-100%]
    //float feedbackAmount = 0.5f // [0-1.0]
    
    static const int MAX_BUFFERSIZE = 96000;  // Grain Size is shorter
    //float delayBuffer[MAX_BUFFERSIZE][2] = {0.0f}; // array initialization; higher values allocate more memory
    float delayBuffer[MAX_BUFFERSIZE][2] = {0.0f};

    int index[2] = {0};
    
    //ADSR
    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParams;
    
    
    
    
};
    
    
    
    
    
    
    
    
    





