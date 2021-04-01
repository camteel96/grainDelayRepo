/*
  ==============================================================================

    GrainDelay.cpp
    Created: 27 Feb 2021 3:30:33pm
    Author:  Cameron Teel

  ==============================================================================
*/

#include "GrainDelay.h"
#include "PluginProcessor.h"

// Constructor here
GrainDelay::GrainDelay() {
}

//ADSR::ADSR() {}

GrainDelay::GrainDelay(float delay, float speed) {
    this->delay = delay;
    this->speed = speed;
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

        //Do I need any of this? Think...
        float lfo = depth * sin(currentAngle[channel]);;

        currentAngle[channel] += angleChange;
        if (currentAngle[channel] > 2.f * M_PI) {
            currentAngle[channel] -= 2.f * M_PI;
        }
    
    
        // Delay Buffer - Grain Size in Samples
//        int d1 = floor(delay + lfo);
        int d1 = floor(delay);
//        int d2 = d1 + MAX_BUFFERSIZE/2; // how far should write be from read index?
        int d2 = d1 + 1; // how far should write be from read index?

        float g2 = delay + lfo - (float)d1;
//        float g2 = delay - (float)d1;
        float g1 = 1.0f - g2;
        
        int readIndex = index[channel] - d1;
        if (readIndex < 0){
            readIndex += MAX_BUFFERSIZE;
        }
        
        int writeIndex = index[channel] - d2;
        if (writeIndex < 0) {
            writeIndex += MAX_BUFFERSIZE;
        }
        
        // Try both options here
        float y =  g1 * delayBuffer[readIndex][channel] + g2 * delayBuffer[writeIndex][channel] * feedbackAmount;
        //float y = g1 * delayBuffer[readIndex][channel]  * feedbackAmount;
        
        delayBuffer[index[channel]][channel] = x;
//        writeIndex = x;
//        readIndex++;
//        writeIndex++;

        if (index[channel] < MAX_BUFFERSIZE - 1){
            index[channel]++;
        }
        else {
            index[channel] = 0;
        }
    
//        if (readIndex > MAX_BUFFERSIZE){
//        readIndex = 1;
//        }
//        else if (writeIndex > MAX_BUFFERSIZE) {
//        writeIndex = 1;
//        }

        
        //Convert audio buffer to audio block
        //dsp::AudioBlock<float> block(delayBuffer);
        // set wet/dry here since I have y and x?
        y *= wetDryAmount;
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

void GrainDelay::setSpeed(float speed){
    if (speed >= 0.1f && speed <=2.0f){
        this->speed = speed;
        angleChange = speed * (1.f/Fs) * 2 * M_PI;
    }
}

void GrainDelay::setDepth(float depth){
    this->depth = depth;
}
