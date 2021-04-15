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
    
    // Constructor
    GrainDelay();
    
    GrainDelay(float delay);
    
    enum NoteSelection { WHOLE, HALF, QUARTER, EIGHTH, SIXTEENTH };
    
    void processSignal(float * signal, const int numSamples, const int channel);
    
    float processSample(float x, int channel);
    
    void setFs(float newFs);
    
    void setDelayMS(float newDelayMS);
    void setDelaySamples(float newDelaySamples);
    
    void setFeedbackAmount(float newFeedbackAmount);
    
    void setGrainSize(int newGrainSize);
    
    void setBPM(float newBPM);
    void setNoteDuration(int newNoteSelection);
    
    void setWetDryAmount(float newWetDryAmount);


    
private:
    
    float Fs = 48000.f;
    
    float delayMS = 100.f;
    float delaySamples = round(Fs*delayMS/1000.f); // samples

    float feedbackAmount = 0.5f; // [0-1.0]
    int grainSize = 1024;
    float wetDryAmount = 1.00f; //[0-100%]
    
    float bpm = 120.f;
    
    float noteDuration = 1.f; // 1 - quarter , 2-half , 0.5-eighth , 0.25 sixteenth
    NoteSelection noteSelect = QUARTER;
    
    static const int MAX_BUFFERSIZE = 96000;  // Grain Size is shorter
    //float delayBuffer[MAX_BUFFERSIZE][2] = {0.0f}; // array initialization; higher values allocate more memory
    float delayBuffer[MAX_BUFFERSIZE][2] = {0.0f};

    int index[2] = {0};

};
    
    
    
    
    
    
    
    
    





