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

//ADSR::ADSR() {}

// This is delay in samples
GrainDelay::GrainDelay(float delay) {
    this->delay = delay;
}

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
    
    if (delay < 1.f){
        return x;
    }
    else {
    
        // Delay Buffer - Grain Size in Samples
//        int d1 = floor(delay + lfo);
        int d1 = floor(delay);
        
//        int d2 = d1 + MAX_BUFFERSIZE/2; // how far should write be from read index?
        //int d2 = d1 + 1; // how far should write be from read index?

        float g2 = delay - (float)d1; //fraction
//        float g2 = delay - (float)d1;
        float g1 = 1.0f - g2;
        
//        int readIndex = index[channel] - d1;
//        if (readIndex < 0){
//            readIndex += MAX_BUFFERSIZE;
//        }
//
//        int writeIndex = index[channel] - d2;
//        if (writeIndex < 0) {
//            writeIndex += MAX_BUFFERSIZE;
//        }
        
        // Try both options here
        //float y =  g1 * delayBuffer[readIndex][channel] + g2 * delayBuffer[writeIndex][channel] * feedbackAmount;
        float y = g1 * delayBuffer[index[channel]][channel] * feedbackAmount;

        delayBuffer[index[channel]][channel] = x;
//        writeIndex = x;
//        readIndex++;
//        writeIndex++;

        // I think here is where I can alter my grain Size without changing size of the array
        if (index[channel] < grainSize){
            index[channel]++;
        }
        else {
            index[channel] = 0;
        }
        
        // set wet/dry here since I have y and x?
        //y *= wetDryAmount;
        //y -= x * wetValue;

    return y;
        
    }
}

//Attempting to Apply ADSR
//ADSR::applyEnvelopeToBuffer(AudioBuffer<float> *delayBuffer, int n, int numSamples);

void GrainDelay::setFs(float Fs) {
    this->Fs = Fs;
    // delaySamples = round(Fs*delayMS/1000.f);
}

void GrainDelay::setGrainSize(int grainSize){
    this->grainSize = grainSize;
}

void GrainDelay::setBPM(float newBPM) {
    bpm = newBPM;
}

void GrainDelay::setDelaySamples(float delay){
    if (delay >= 1.f){
        this->delay = delay;
    }
    else{
        this->delay = 0.f;
    }
}

void GrainDelay::setWetDryAmount(float newWetDryAmount){
    if (newWetDryAmount <= 1.0f)
    wetDryAmount = newWetDryAmount;
}

void GrainDelay::setNoteDuration(NoteSelection newNoteSelection){
    
    noteSelect = newNoteSelection;
    //noteDuration = newNoteDuration;
    
    switch (noteSelect) {
        case WHOLE:
            noteDuration = 4.0f;
            break;
            
        case HALF:
            noteDuration = 2.0;
            break;
            
        case QUARTER:
            noteDuration = 1.0f;
            break;
            
        case EIGHTH:
            noteDuration = 0.5f;
            break;
            
        case SIXTEENTH:
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
    float sampNote = secNote * Fs;
    //float msNote = secNote * 1000.f;
    setDelaySamples(sampNote);
}


//void GrainDelay::setDelayMS(float newDelayMS) {
//    if (newDelayMS <= 1000.f){
//        delayMS = newDelayMS;
//    }
//    delayMS = newDelayMS;
//    delaySamples = round(Fs*delayMS/1000.f);
//}

//void ADSR::setParameters(float attack, float decay, float sustain, float release){
//
//
//
//
//
//}
