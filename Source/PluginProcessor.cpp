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
                       ), state(*this, nullptr, "DelayParams", createParameterLayout())
#endif
{
}

GrainDelayAudioProcessor::~GrainDelayAudioProcessor()
{
}

AudioProcessorValueTreeState::ParameterLayout GrainDelayAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> params; // syntax for parameter value, pointer destroys itself when we're done with it
    StringArray myChoices;
    myChoices.add("half");
    myChoices.add("quarter");
    myChoices.add("eighth");
    myChoices.add("sixteenth");
    // make new audio parameter float for our delay knob
    params.push_back(std::make_unique<AudioParameterFloat> ("delayMS","Delay",1.f,1000.f,1.f));
    params.push_back(std::make_unique<AudioParameterFloat> ("grainSize","Grain Size",1.f,1024.f,1.f));
    params.push_back(std::make_unique<AudioParameterFloat> ("wetDryAmount","Wet / Dry",0.f,1.f,.01f));
    params.push_back(std::make_unique<AudioParameterFloat> ("feedbackAmount","Feedback",0.f,100.f,1.f));
    params.push_back(std::make_unique<AudioParameterBool> ("tempoSyncd","Tempo Sync'd",false));
    params.push_back(std::make_unique<AudioParameterChoice> ("noteSelect", "Note Selector", myChoices, 1));



    
    // we've got to return parameter layout, all of them in this case, go through all and terurn the whole set of them
    return {params.begin(), params.end()};
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
    grainDelay.setFs(sampleRate);
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

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    bool tempoSyncd = (bool) state.getRawParameterValue("tempoSyncd"); // takes value and turns it into a bool
    if (tempoSyncd){
        playHead = this->getPlayHead();
        playHead->getCurrentPosition(currentPositionInfo);

        float newBPM = currentPositionInfo.bpm;
        if (bpm != newBPM){
            // update BPM
            grainDelay.setBPM(newBPM);
            bpm = newBPM;
        }
        int noteSelect = (int)*state.getRawParameterValue("noteSelect");
        grainDelay.setNoteDuration(noteSelect);
    }
    else{
        float delayMS = *state.getRawParameterValue("grainSize");
        grainDelay.setDelayMS(delayMS);
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
   // handle entire state oft he plugin and set to this xml elements that goes out to DAW
    auto currentState = state.copyState(); // make copy of state
    std::unique_ptr<XmlElement> xml (currentState.createXml()); // use this copy to make XML element
    copyXmlToBinary(*xml, destData); // copy over xmp to destination data
    // no matter how many parameters I have, just need these three lines
    
}

void GrainDelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xml (getXmlFromBinary(data, sizeInBytes)); // take data as binary and turn it into xml
    if (xml && xml->hasTagName(state.state.getType())) { // making sure its not the null pointer
        state.replaceState(ValueTree::fromXml(*xml)); // *xml = dereference xml
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GrainDelayAudioProcessor();
}
