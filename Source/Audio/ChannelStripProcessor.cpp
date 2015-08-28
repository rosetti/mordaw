/*
  ==============================================================================

    GainProcessor.cpp
    Created: 18 Jul 2015 11:24:01am
    Author:  Matt

  ==============================================================================
*/

#include "ChannelStripProcessor.h"
#include "../Utility/Conversion.h"

//==============================================================================
ChannelStripProcessor::ChannelStripProcessor()
{
	_gain = 1.0f;
	_panning = 0.5f;
	_muteGain = 0.0f;
	_muted = false;
}

ChannelStripProcessor::~ChannelStripProcessor()
{
}

int ChannelStripProcessor::getNumParameters()
{
	return 3;
}

float ChannelStripProcessor::getParameter(int index)
{
    switch(index)
    {
        case StripParameter::GAIN:
            return _gain;
        case StripParameter::PAN:
            return _panning;
        case StripParameter::MUTE:
            return _muteGain;
        default:
            return 0.0f;
    }
}

void ChannelStripProcessor::setParameter(int index, float newValue)
{
    if (index == StripParameter::GAIN)
		_gain = newValue;
    else if (index == StripParameter::PAN)
		_panning = newValue;
}

void ChannelStripProcessor::setMuteParameter()
{
	if (_muted)
		_muted = false;
	else
		_muted = true;
}

const String ChannelStripProcessor::getParameterName(int index)
{
    switch (index)
    {
        case StripParameter::GAIN:
            return "Gain";
        case StripParameter::PAN:
            return "Panning";
        case StripParameter::MUTE:
            return "Mute";
        default:
            return "Bad Selection";
    }
}

const String ChannelStripProcessor::getParameterText(int index)
{
    switch (index)
    {
        case StripParameter::GAIN:
            return String(_gain);
        case StripParameter::PAN:
            return String(_panning);
        case StripParameter::MUTE:
            return String(_muted);
        default:
            return "Bad Selection";
    }
}

const String ChannelStripProcessor::getInputChannelName(int channelIndex) const
{
	return String(channelIndex + 1);
}

const String ChannelStripProcessor::getOutputChannelName(int channelIndex) const
{
	return String(channelIndex + 1);
}

bool ChannelStripProcessor::isInputChannelStereoPair(int) const
{
	return true;
}

bool ChannelStripProcessor::isOutputChannelStereoPair(int) const
{
	return true;
}

bool ChannelStripProcessor::acceptsMidi() const
{
	#if JucePlugin_WantsMidiInput
		return true;
	#else
		return false;
	#endif
}

bool ChannelStripProcessor::producesMidi() const
{
	#if JucePlugin_ProducesMidiOutput
		return true;
	#else
		return false;
	#endif
}

bool ChannelStripProcessor::silenceInProducesSilenceOut() const
{
	return false;
}


//==============================================================================
void ChannelStripProcessor::prepareToPlay(double, int)
{
    setPlayConfigDetails(2, 2, getSampleRate(), getBlockSize());
}

void ChannelStripProcessor::releaseResources()
{

}

void ChannelStripProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer&)
{
	if (!_muted)
	{
		buffer.applyGain(0, 0, buffer.getNumSamples(), _gain*(1.0f - _panning));
		buffer.applyGain(1, 0, buffer.getNumSamples(), _gain*_panning);
	}
	if (_muted)
	{
		buffer.applyGain(0.0);
	}

	for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
		buffer.clear(i, 0, buffer.getNumSamples());
}

void ChannelStripProcessor::getStateInformation(MemoryBlock&)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.
}

void ChannelStripProcessor::setStateInformation(const void*, int)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.
}

const void ChannelStripProcessor::setID(int ident)
{
    _id = ident;
}

const int ChannelStripProcessor::getID()
{
    return _id;
}

bool ChannelStripProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* ChannelStripProcessor::createEditor()
{
	return nullptr;
}