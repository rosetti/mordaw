/*
  ==============================================================================

    TrackProcessor.cpp
    Created: 2 Aug 2015 4:19:11pm
    Author:  Thomas

  ==============================================================================
*/

#include "TrackProcessor.h"

namespace Audio {
    /*
     Constructs a mixer object
     @param track Pointer to a track AudioSource
     @param thread Thread for the running the track processor
     */
    TrackProcessor::TrackProcessor(Track *track, TimeSliceThread *thread) : _track(track)
    {
        _source.setSource(track, 0, thread, 0);
    }

    TrackProcessor::~TrackProcessor()
    {
    }

    /*
     Get the source.
     */
    AudioTransportSource &TrackProcessor::getSource()
    {
        return _source;
    }

    /*
     Get name of processor.
     */
    const String TrackProcessor::getName() const
    {
        return "Track Processor";
    }

    /*
     Prepare the source to play.
     */
    void TrackProcessor::prepareToPlay(double newSampleRate, int estimatedSamplesPerBlock)
    {
        _source.prepareToPlay(estimatedSamplesPerBlock, newSampleRate);
    }

    /*
     Release the resources
     */
    void TrackProcessor::releaseResources()
    {
        _source.releaseResources();
    }

    /*
     Convert the audio source into to a processor.
     */
    void TrackProcessor::processBlock(AudioSampleBuffer & buffer, MidiBuffer &)
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
    const String TrackProcessor::getInputChannelName(int channelIndex) const
    {
        return String(channelIndex + 1);
    }

    /*
     Get the current channel name
     */
    const String TrackProcessor::getOutputChannelName(int channelIndex) const
    {
        return String(channelIndex + 1);
    }

    /*
     The processor is not a stereo pair
     */
    bool TrackProcessor::isInputChannelStereoPair(int ) const
    {
        return false;
    }

    /*
     The processor is not a stereo pair
     */
    bool TrackProcessor::isOutputChannelStereoPair(int) const
    {
        return false;
    }

    /*
     The processor can produce audio output without input.
     */
    bool TrackProcessor::silenceInProducesSilenceOut() const
    {
        return false;
    }

    /*
     The processor has no tail.
     */
    double TrackProcessor::getTailLengthSeconds() const
    {
        return 0.0f;
    }

    /*
     Check if device can accepts midi
     */
    bool TrackProcessor::acceptsMidi() const
    {
        return false;
    }

    /*
     Check if device can produce midi
     */
    bool TrackProcessor::producesMidi() const
    {
        return false;
    }
    
    /*
     Inherited from AudioProcessor
     */
    AudioProcessorEditor * TrackProcessor::createEditor()
    {
        return nullptr;
    }

    /*
     Inherited from AudioProcessor
     */
    bool TrackProcessor::hasEditor() const
    {
        return false;
    }

    /*
     Inherited from AudioProcessor
     */
    int TrackProcessor::getNumPrograms()
    {
        return 0;
    }

    /*
     Inherited from AudioProcessor
     */
    int TrackProcessor::getCurrentProgram()
    {
        return 0;
    }

    /*
     Inherited from AudioProcessor
     */
    void TrackProcessor::setCurrentProgram(int	)
    {
    }

    /*
     Inherited from AudioProcessor
     */
    const String TrackProcessor::getProgramName(int)
    {
        return String::empty;
    }

    /*
     Inherited from AudioProcessor
     */
    void TrackProcessor::changeProgramName(int, const String &)
    {
    }

    /*
     Inherited from AudioProcessor
     */
    void TrackProcessor::getStateInformation(juce::MemoryBlock &)
    {
    }

    /*
     Inherited from AudioProcessor
     */
    void TrackProcessor::setStateInformation(const void *, int)
    {
    }

}