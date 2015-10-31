/*
  ==============================================================================

    TrackProcessor.cpp
    Created: 2 Aug 2015 4:19:11pm
    Author:  Thomas

  ==============================================================================
*/

#include "TrackNode.h"

namespace Audio {
    /*
     Constructs a mixer object
     @param track Pointer to a track AudioSource
     @param thread Thread for the running the track processor
     */
    TrackNode::TrackNode(Track *track, TimeSliceThread *thread) : _track(track)
    {
        _source.setSource(track, 0, thread, 0);
    }

    TrackNode::~TrackNode()
    {
    }

    /*
     Get the source.
     */
    AudioTransportSource& TrackNode::getSource()
    {
        return _source;
    }

    /*
     Get name of processor.
     */
    const String TrackNode::getName() const
    {
        return "Track Processor";
    }

    /*
     Prepare the source to play.
     */
    void TrackNode::prepareToPlay(double newSampleRate, int estimatedSamplesPerBlock)
    {
        _source.prepareToPlay(estimatedSamplesPerBlock, newSampleRate);
    }

    /*
     Release the resources
     */
    void TrackNode::releaseResources()
    {
        _source.releaseResources();
    }

    /*
     Convert the audio source into to a processor.
     */
    void TrackNode::processBlock(AudioSampleBuffer & buffer, MidiBuffer &)
    {
        AudioSourceChannelInfo channelInfo;
        
        channelInfo.buffer = &buffer;
        channelInfo.startSample = 0;
        channelInfo.numSamples = buffer.getNumSamples();
        _source.getNextAudioBlock(channelInfo);
    }

    /*
     Get the current channel name
     */
    const String TrackNode::getInputChannelName(int channelIndex) const
    {
        return String(channelIndex + 1);
    }

    /*
     Get the current channel name
     */
    const String TrackNode::getOutputChannelName(int channelIndex) const
    {
        return String(channelIndex + 1);
    }

    /*
     The processor is not a stereo pair
     */
    bool TrackNode::isInputChannelStereoPair(int ) const
    {
        return false;
    }

    /*
     The processor is not a stereo pair
     */
    bool TrackNode::isOutputChannelStereoPair(int) const
    {
        return false;
    }

    /*
     The processor can produce audio output without input.
     */
    bool TrackNode::silenceInProducesSilenceOut() const
    {
        return false;
    }

    /*
     The processor has no tail.
     */
    double TrackNode::getTailLengthSeconds() const
    {
        return 0.0f;
    }

    /*
     Check if device can accepts midi
     */
    bool TrackNode::acceptsMidi() const
    {
        return false;
    }

    /*
     Check if device can produce midi
     */
    bool TrackNode::producesMidi() const
    {
        return false;
    }
    
    /*
     Inherited from AudioProcessor
     */
    AudioProcessorEditor * TrackNode::createEditor()
    {
        return nullptr;
    }

    /*
     Inherited from AudioProcessor
     */
    bool TrackNode::hasEditor() const
    {
        return false;
    }

    /*
     Inherited from AudioProcessor
     */
    int TrackNode::getNumPrograms()
    {
        return 0;
    }

    /*
     Inherited from AudioProcessor
     */
    int TrackNode::getCurrentProgram()
    {
        return 0;
    }

    /*
     Inherited from AudioProcessor
     */
    void TrackNode::setCurrentProgram(int)
    {
    }

    /*
     Inherited from AudioProcessor
     */
    const String TrackNode::getProgramName(int)
    {
        return String::empty;
    }

    /*
     Inherited from AudioProcessor
     */
    void TrackNode::changeProgramName(int, const String &)
    {
    }

    /*
     Inherited from AudioProcessor
     */
    void TrackNode::getStateInformation(juce::MemoryBlock &)
    {
    }

    /*
     Inherited from AudioProcessor
     */
    void TrackNode::setStateInformation(const void *, int)
    {
    }

}