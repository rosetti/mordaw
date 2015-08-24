/*
  ==============================================================================

    TrackProcessor.cpp
    Created: 2 Aug 2015 4:19:11pm
    Author:  Thomas

  ==============================================================================
*/

#include "TrackProcessor.h"

namespace Audio {
    TrackProcessor::TrackProcessor(Track *track, TimeSliceThread *thread) : _track(track)
    {
        _source.setSource(track, 0, thread, 0);
    }

    TrackProcessor::~TrackProcessor()
    {
    }

    AudioTransportSource &TrackProcessor::getSource()
    {
        return _source;
    }

    const String TrackProcessor::getName() const
    {
        return "Track Processor";
    }

    void TrackProcessor::prepareToPlay(double newSampleRate, int estimatedSamplesPerBlock)
    {
        _source.prepareToPlay(estimatedSamplesPerBlock, newSampleRate);
    }

    void TrackProcessor::releaseResources()
    {
        _source.releaseResources();
    }

    void TrackProcessor::processBlock(AudioSampleBuffer & buffer, MidiBuffer &)
    {
        AudioSourceChannelInfo channelInfo;
        
        channelInfo.buffer = &buffer;
        channelInfo.startSample = 0;
        channelInfo.numSamples = buffer.getNumSamples();
        _source.getNextAudioBlock(channelInfo);
    }

    const String TrackProcessor::getInputChannelName(int channelIndex) const
    {
        return String(channelIndex + 1);
    }

    const String TrackProcessor::getOutputChannelName(int channelIndex) const
    {
        return String(channelIndex + 1);
    }

    bool TrackProcessor::isInputChannelStereoPair(int ) const
    {
        return false;
    }

    bool TrackProcessor::isOutputChannelStereoPair(int) const
    {
        return false;
    }

    bool TrackProcessor::silenceInProducesSilenceOut() const
    {
        return false;
    }

    double TrackProcessor::getTailLengthSeconds() const
    {
        return 0.0f;
    }

    bool TrackProcessor::acceptsMidi() const
    {
        return false;
    }

    bool TrackProcessor::producesMidi() const
    {
        return false;
    }

    AudioProcessorEditor * TrackProcessor::createEditor()
    {
        return nullptr;
    }

    bool TrackProcessor::hasEditor() const
    {
        return false;
    }

    int TrackProcessor::getNumPrograms()
    {
        return 0;
    }

    int TrackProcessor::getCurrentProgram()
    {
        return 0;
    }

    void TrackProcessor::setCurrentProgram(int	)
    {
    }

    const String TrackProcessor::getProgramName(int)
    {
        return String::empty;
    }

    void TrackProcessor::changeProgramName(int, const String &)
    {
    }

    void TrackProcessor::getStateInformation(juce::MemoryBlock &)
    {
    }

    void TrackProcessor::setStateInformation(const void *, int)
    {
    }

}