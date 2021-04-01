/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GrainDelayAudioProcessor::GrainDelayAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

GrainDelayAudioProcessor::~GrainDelayAudioProcessor()
{
}

//==============================================================================
const juce::String GrainDelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GrainDelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GrainDelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GrainDelayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GrainDelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GrainDelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int GrainDelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GrainDelayAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String GrainDelayAudioProcessor::getProgramName (int index)
{
    return {};
}

void GrainDelayAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void GrainDelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
//    spec.sampleRate = sampleRate;
//    spec.maximumBlockSize = samplesPerBlock;
//    spec.numChannels = 2;
//    grainDelay.prepare(spec);
 //   grainDelay.setFs(sampleRate);
}

void GrainDelayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GrainDelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void GrainDelayAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    if (tempoSyncd){
        playHead = this->getPlayHead();
        playHead->getCurrentPosition(currentPositionInfo);

        float newBPM = currentPositionInfo.bpm;
        if (bpm != newBPM){
            // update BPM
            grainDelay.setBPM(newBPM);
            bpm = newBPM;
        }
        //grainDelay.setNoteDuration(noteSelect);
    }
    else{
        grainDelay.setDelaySamples(delay);
    }

    
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
//        auto* channelData = buffer.getWritePointer (channel);
        for (int n = 0; n < buffer.getNumSamples(); ++n){
            float x = buffer.getReadPointer(channel)[n];
            x = grainDelay.processSample(x, channel);
            buffer.getWritePointer(channel)[n] = x;
        }
    }
}

//void ADSR::applyEnvelopeToBuffer(AudioBuffer<float>, int n, int numSamples) {
//    
//}

//==============================================================================
bool GrainDelayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* GrainDelayAudioProcessor::createEditor()
{
    return new GrainDelayAudioProcessorEditor (*this);
}

//==============================================================================
void GrainDelayAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void GrainDelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GrainDelayAudioProcessor();
}
