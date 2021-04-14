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
//GrainDelay::GrainDelay(float delay) {
//    this->delay = delay;
//}

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
    
    if (delaySamples < 1.f){
        return x;
    }
    else {
    
        // Try both options here
        //float y =  g1 * delayBuffer[readIndex][channel] + g2 * delayBuffer[writeIndex][channel] * feedbackAmount;
        
        float y = x + delayBuffer[index[channel]][channel] * feedbackAmount;
        
        delayBuffer[index[channel]][channel] = x;

        // I think here is where I can alter my grain Size without changing size of the array
        if (index[channel] < grainSize){
            index[channel]++;
        }
        else {
            index[channel] = 0;
        }
        
        // set wet/dry here since I have y and x?
        y = (y * wetDryAmount) + (x * (1-wetDryAmount));

    return y;
        
    }
}

void GrainDelay::setFs(float Fs) {
    this->Fs = Fs;
     delaySamples = round(Fs*delayMS/1000.f);
}

void GrainDelay::setGrainSize(int grainSize){
    this->grainSize = grainSize;
}

void GrainDelay::setBPM(float newBPM) {
    bpm = newBPM;
}

void GrainDelay::setDelaySamples(float delaySamples){
    if (delaySamples >= 1.f){
        this->delaySamples = delaySamples;
    }
    else{
        this->delaySamples = 0.f;
    }
}

void GrainDelay::setWetDryAmount(float newWetDryAmount){
    if (newWetDryAmount <= 1.0f)
    this->wetDryAmount = newWetDryAmount;
}

void GrainDelay::setNoteDuration(int newNoteSelection){
    
    //noteDuration = newNoteDuration;
    
    
    // case 0 = whole note, 1 = half note,,,,, put down there
    switch (newNoteSelection) {
        case 0:
            noteDuration = 4.0f;
            break;
            
        case 1:
            noteDuration = 2.0;
            break;
            
        case 2:
            noteDuration = 1.0f;
            break;
            
        case 3:
            noteDuration = 0.5f;
            break;
            
        case 4:
            noteDuration = 0.25f;
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
}


void GrainDelay::setDelayMS(float newDelayMS) {
    if (newDelayMS <= 1000.f){
        delayMS = newDelayMS;
    }
    delayMS = newDelayMS;
    grainSize = round(Fs*delayMS/1000.f); // what were using for delay
}

