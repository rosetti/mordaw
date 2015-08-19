/*
  ==============================================================================

    GainProcessor.cpp
    Created: 18 Jul 2015 11:24:01am
    Author:  Matt

  ==============================================================================
*/

#include "ChannelStripProcessor.h"

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
	if (index == 1)
	{
		return _gain;
	}
	else if (index == 2)
	{
		return _panning;
	}
	else if (index == 3)
	{
		return _muteGain;
	}
	else
	{
		return 0.0f;
	}
}

void ChannelStripProcessor::setParameter(int index, float newValue)
{
	if (index == 1)
	{
		_gain = newValue;
	}
	else if (index == 2)
	{
		_panning = newValue;
	}
}

void ChannelStripProcessor::setMuteParameter()
{
	if (_muted)
	{
		_muted = false;
	}
	else
	{
		_muted = true;
	}
}

const String ChannelStripProcessor::getParameterName(int index)
{
	if (index == 1)
	{
		return "Gain";
	}
	else if (index == 2)
	{
		return "Panning";
	}
	else if (index == 3)
	{
		return "Mute";
	}
	return "Bad Selection";
}

const String ChannelStripProcessor::getParameterText(int index)
{
	if (index == 1)
	{
		return String(_gain);
	}
	else if (index == 2)
	{
		return String(_panning);
	}
	else if (index == 3)
	{
		return String(_muted);
	}
	return "Bad Selection";
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

}

void ChannelStripProcessor::releaseResources()
{

}

void ChannelStripProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer&)
{
	if (!_muted)
	{
		if (_panning == 0.5)
		{
			buffer.applyGain(_gain);
		}
		else if (_panning < 0.5)
		{
			buffer.applyGain(0, 0, buffer.getNumSamples(), _gain);
			buffer.applyGain(1, 0, buffer.getNumSamples(), _gain / _panning);
		}
		else if (_panning > 0.5)
		{
			buffer.applyGain(0, 0, buffer.getNumSamples(), _gain / (_panning - 0.5f));
			buffer.applyGain(1, 0, buffer.getNumSamples(), _gain);
		}
	}
	else if (_muted)
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

const String ChannelStripProcessor::getName() const
{
	return "Channel Strip";
}

bool ChannelStripProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* ChannelStripProcessor::createEditor()
{
	return nullptr;
}