/*
  ==============================================================================

    GrainDelay.cpp
    Created: 27 Feb 2021 3:30:33pm
    Author:  Cameron Teel

  ==============================================================================
*/

#include "GrainDelay.h"
#include "PluginProcessor.h"

GrainDelay::GrainDelay() {
}

// This is delay in samples, Do I need this?

// Do I need processSignal and processSample?
void GrainDelay::processSignal(float *signal, const int numSamples, const int channel){
    
    for (int n = 0; n < numSamples; n++){
        float x = signal[n]; // get value at memory location
        x = processSample(x, channel);
        signal[n] = x;
    }
}

// Signal Processing Block -builds upon fractional delay
float GrainDelay::processSample(float x, int channel){
    
    if (grainSize < 1.f){
        return x;
    }
    else {
        
        float y = x + delayBuffer[index[channel]][channel];

        delayBuffer[index[channel]][channel] = y * feedbackAmount;

        if (index[channel] < grainSize){
            index[channel]++;
        }
        else {
            index[channel] = 0;
        }
        
        y = (y * wetDryAmount) + (x * (1-wetDryAmount));

    return y;
        
    }
}

void GrainDelay::setFs(float newFs) {
     Fs = newFs;
//     delaySamples = round(Fs*delayMS/1000.f);
}

void GrainDelay::setGrainSize(int newGrainSize){
    grainSize = newGrainSize;
}

void GrainDelay::setBPM(float newBPM) {
    bpm = newBPM;
}

void GrainDelay::setFeedbackAmount(float newFeedbackAmount){
    feedbackAmount = newFeedbackAmount;
}

void GrainDelay::setWetDryAmount(float newWetDryAmount){
    if (newWetDryAmount <= 1.0f)
        wetDryAmount = newWetDryAmount;
}

void GrainDelay::setNoteDuration(int newNoteSelection){
    
    switch (newNoteSelection) {
        case 0: // WHOLE
            noteDuration = 4.0f;
            break;
            
        case 1: // HALF
            noteDuration = 2.0;
            break;
            
        case 2: // QUARTER
            noteDuration = 1.0f;
            break;
            
        case 3: // EIGHTH
            noteDuration = 0.5f;
            break;
            
        case 4: // SIXTEENTH
            noteDuration = 0.25f;
            break;
            
        case 5: // THIRTY-SECOND
            noteDuration = 0.125f;
            break;
            
        case 6: // SIXTY-FOURTH
            noteDuration = 0.0625f;
            break;
            
        default:
            noteDuration = 1.0f;
            break;
    }
    
    // delay to samples conversion
    float beatSec = bpm * (1.f/60.f);
    float secBeat = 1.f/beatSec;
    float secNote = noteDuration * secBeat;
//    float sampNote = secNote * Fs;
    float msNote = secNote * 1000.f;
    setDelayMS(msNote);
//    setDelaySamples(sampNote);
}

void GrainDelay::setDelayMS(float newDelayMS) {
    if (newDelayMS <= 1000.f){
        delayMS = newDelayMS;
    }
    delayMS = newDelayMS;
    grainSize = round(Fs*delayMS/1000.f); // what were using for delay
}

