/*
 ==============================================================================
 
 GainProcessor.cpp
 Created: 18 Jul 2015 11:24:01am
 Author:  Matt
 
 ==============================================================================
 */

#include "ChannelStripNode.h"
#include "../../Utility/Conversion.h"

//==============================================================================

/*
 A Channel Stip is used to control the volume (gain), panning and mute settings of individual tracks
 */
ChannelStripNode::ChannelStripNode()
{
    //Set the initial gain value to standard
    _gain = 0.7f;
    //Set the initial panning value to 0.5 (center)
    _panning = 0.5f;
    //Set the volume used if mute is enabled
    _muteGain = 0.0f;
    //Set the mute value to false initially
    _muted = false;
}

ChannelStripNode::~ChannelStripNode()
{
}

/*
 A function to get the number of parameters this processor contains
 @return The number of parameters
 */
int ChannelStripNode::getNumParameters()
{
    return 3;
}

/*
 Gets the value of a particular parameter
 @param index The index value of the parameter to be returned
 @return The value of the chosen parameter
 */
float ChannelStripNode::getParameter(int index)
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

/*
 Sets the value of a chosen parameter
 @param index The index value of the parameter to be modified
 @param newValue The float value to set the chosen parameter to
 */
void ChannelStripNode::setParameter(int index, float newValue)
{
    if (index == StripParameter::GAIN)
        _gain = newValue;
    else if (index == StripParameter::PAN)
        _panning = newValue;
}

/*
 Switches mute on and off dependent upon its current state
 */
void ChannelStripNode::setMuteParameter()
{
    if (_muted)
        _muted = false;
    else
        _muted = true;
}

/*
 Gets the current mute setting
 @return The current mute state
 */
bool ChannelStripNode::getMuteParameter()
{
    return _muted;
}

/*
 Returns the name of a chosen parameter
 @param index The index value of the parameter to be returned
 @return The name of the selected parameter
 */
const String ChannelStripNode::getParameterName(int index)
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

/*
 Returns a String containing the current value of a chosen parameter
 @param index The index value of the parameter to be returned
 @return The String of a value contained within the chosen parameter
 */
const String ChannelStripNode::getParameterText(int index)
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

/*
 Returns a String containing the name of a chosen input channel
 @param channelIndex The input channel to return the name of
 @return The name of the input channel selected
 */
const String ChannelStripNode::getInputChannelName(int channelIndex) const
{
    return String(channelIndex + 1);
}

/*
 Returns a String containing the name of a chosen output channel
 @param channelIndex The output channel to return the name of
 @return The name of the output channel selected
 */
const String ChannelStripNode::getOutputChannelName(int channelIndex) const
{
    return String(channelIndex + 1);
}

/*
 Inherited from AudioProcessor
 */
bool ChannelStripNode::isInputChannelStereoPair(int) const
{
    return true;
}

/*
 Inherited from AudioProcessor
 */
bool ChannelStripNode::isOutputChannelStereoPair(int) const
{
    return true;
}

/*
 Inherited from AudioProcessor
 */
bool ChannelStripNode::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

/*
 Inherited from AudioProcessor
 */
bool ChannelStripNode::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

/*
 Inherited from AudioProcessor
 */
bool ChannelStripNode::silenceInProducesSilenceOut() const
{
    return false;
}

/*
 Congigures the channelStripProcess and preares it to play
 */
void ChannelStripNode::prepareToPlay(double, int)
{
    setPlayConfigDetails(2, 2, getSampleRate(), getBlockSize());
}

/*
 Inherited from AudioProcessor
 */
void ChannelStripNode::releaseResources()
{
    
}


/*
 This method is where user chosen values are applied to the audio buffer.
 If the track is not muted then gain is applied to both the left and right channels
 based on both the current gain value as well as taking into consideration the current
 panning value.
 @param &buffer the buffer to be processed
 */
void ChannelStripNode::processBlock(AudioSampleBuffer& buffer, MidiBuffer&)
{
    //Check to see if the track is un-muted
    if (!_muted)
    {
        //Apply the gain value to the left channel relative to the panning value
        //buffer.applyGain(0, 0, buffer.getNumSamples(), _gain*(0));
        float leftGain = _gain * (1.0f - _panning);
        float rightGain= _gain * _panning;
        buffer.applyGain(0, 0, buffer.getNumSamples(), leftGain);
        //Apply the gain value to the right channel relative to the panning value
        //buffer.applyGain(1, 0, buffer.getNumSamples(), 0);
        buffer.applyGain(1, 0, buffer.getNumSamples(), rightGain);
    }
    //Check to see if the track is muted
    if (_muted)
    {
        //Apply a gain value of 0 to the track
        buffer.applyGain(_muteGain);
    }
    
    for (int i = getMainBusNumInputChannels(); i < getMainBusNumOutputChannels(); ++i)
        buffer.clear(i, 0, buffer.getNumSamples());
}

/*
 Inherited from AudioProcessor
 */
void ChannelStripNode::getStateInformation(MemoryBlock&)
{
}

/*
 Inherited from AudioProcessor
 */
void ChannelStripNode::setStateInformation(const void*, int)
{
}

/*
 Inherited from AudioProcessor
 */
const void ChannelStripNode::setID(int ident)
{
    _id = ident;
}

/*
 Inherited from AudioProcessor
 */
const int ChannelStripNode::getID()
{
    return _id;
}

/*
 This function returns true as the editor was configured by us (see: ChannelStripComponent)
 */
bool ChannelStripNode::hasEditor() const
{
    return true;
}

AudioProcessorEditor* ChannelStripNode::createEditor()
{
    return nullptr;
}