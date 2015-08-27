/*
  ==============================================================================

    ExportProcessor.cpp
    Created: 28 Aug 2015 12:24:23am
    Author:  dtl

  ==============================================================================
*/

#include "ExportProcessor.h"
#include "../Utility/Conversion.h"

//==============================================================================
ExportProcessor::ExportProcessor()
{
    _gain = 1.0f;
    _panning = 0.5f;
    _muteGain = 0.0f;
    _muted = false;
}

ExportProcessor::~ExportProcessor()
{
}

int ExportProcessor::getNumParameters()
{
    return 3;
}

float ExportProcessor::getParameter(int index)
{
     return 0.0f;
}

void ExportProcessor::setParameter(int index, float newValue)
{
}

void ExportProcessor::setMuteParameter()
{
}

const String ExportProcessor::getParameterName(int index)
{
    
}

const String ExportProcessor::getParameterText(int index)
{

}

const String ExportProcessor::getInputChannelName(int channelIndex) const
{
    return String(channelIndex + 1);
}

const String ExportProcessor::getOutputChannelName(int channelIndex) const
{
    return String(channelIndex + 1);
}

bool ExportProcessor::isInputChannelStereoPair(int) const
{
    return true;
}

bool ExportProcessor::isOutputChannelStereoPair(int) const
{
    return true;
}

bool ExportProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool ExportProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool ExportProcessor::silenceInProducesSilenceOut() const
{
    return false;
}


//==============================================================================
void ExportProcessor::prepareToPlay(double, int)
{
    
}

void ExportProcessor::releaseResources()
{
    
}

void ExportProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer&)
{

}

void ExportProcessor::getStateInformation(MemoryBlock&)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ExportProcessor::setStateInformation(const void*, int)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

const void ExportProcessor::setID(int ident)
{
}

const int ExportProcessor::getID()
{
    return _id;
}

bool ExportProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* ExportProcessor::createEditor()
{
    return nullptr;
}