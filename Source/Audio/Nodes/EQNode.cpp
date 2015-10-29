/*
  ==============================================================================

    EQNode.cpp
    Created: 29 Oct 2015 6:25:49pm
    Author:  dtl

  ==============================================================================
*/

#include "EQNode.h"

EQNode::EQNode()
{
    
    lastUIWidth = 400;
    lastUIHeight = 200;
    
    lastPosInfo.resetToDefault();
    
}

EQNode::~EQNode()
{
}

//==============================================================================
void EQNode::prepareToPlay (double newSampleRate, int /*samplesPerBlock*/)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void EQNode::releaseResources()
{

}

void EQNode::reset()
{
    
}

void EQNode::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{

}

//==============================================================================
AudioProcessorEditor* EQNode::createEditor()
{
    return nullptr;
}

//==============================================================================
void EQNode::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // Here's an example of how you can use XML to make it easy and more robust:
    
    // Create an outer XML element..
    XmlElement xml ("MYPLUGINSETTINGS");
    
    // add some attributes to it..
    xml.setAttribute ("uiWidth", lastUIWidth);
    xml.setAttribute ("uiHeight", lastUIHeight);
    xml.setAttribute ("gain", gain->getValue());
    
    // then use this helper function to stuff it into the binary blob and return it..
    copyXmlToBinary (xml, destData);
}

void EQNode::setStateInformation (const void* data, int sizeInBytes)
{

}

const String EQNode::getInputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

const String EQNode::getOutputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

bool EQNode::isInputChannelStereoPair (int /*index*/) const
{
    return true;
}

bool EQNode::isOutputChannelStereoPair (int /*index*/) const
{
    return true;
}

bool EQNode::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool EQNode::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool EQNode::silenceInProducesSilenceOut() const
{
    return false;
}

double EQNode::getTailLengthSeconds() const
{
    return 0.0;
}