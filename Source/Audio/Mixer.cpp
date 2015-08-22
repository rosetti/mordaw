/*
==============================================================================

Mixer.cpp
Created: 2 Aug 2015 4:04:13pm
Author:  Thomas

==============================================================================
*/

#include "Mixer.h"

namespace Audio
{
    Mixer::Mixer(int numInputChannels, int numOutputChannels, double sampleRate, int bufferSize) : 
        _nextNodeID(0x10000),
        _thread("Audio"),
        _numInput(numInputChannels),
        _numOutput(numOutputChannels), 
        _sampleRate(sampleRate), 
        _bufferSize(bufferSize),
        _isPlaying(false)
    {
        auto input = new AudioProcessorGraph::AudioGraphIOProcessor(AudioProcessorGraph::AudioGraphIOProcessor::audioInputNode);
        auto output = new AudioProcessorGraph::AudioGraphIOProcessor(AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode);


        _processorGraph.setPlayConfigDetails(numInputChannels, numOutputChannels, sampleRate, bufferSize);
        _processorGraph.addNode(input, INPUT_NODE_ID);
        _processorGraph.addNode(output, OUTPUT_NODE_ID);
    }

    Mixer::~Mixer()
    {
        _thread.stopThread(0);
        stop();
    }

    void Mixer::add(Track * track)
    {
        TrackProcessor *processor = new TrackProcessor(track, &_thread);
        processor->setPlayConfigDetails(_numInput, _numOutput, _sampleRate, _bufferSize);
        
        ChannelStripProcessor *strip = new ChannelStripProcessor();
        strip->setPlayConfigDetails(_numInput, _numOutput, _sampleRate, _bufferSize);

        track->prepareToPlay(_bufferSize, _sampleRate);
        _tracks.insert(std::pair<Track *, TrackProcessor *>(track, processor));
        
        strip->prepareToPlay(_bufferSize, _sampleRate);
        _strips.insert(std::pair<TrackProcessor *, ChannelStripProcessor *>(processor, strip));
        strip->setID(_nextNodeID + 0x1000);

        auto tNode = _processorGraph.addNode(processor, _nextNodeID);
        auto cNode = _processorGraph.addNode(strip, _nextNodeID + 0x1000);
        _processorGraph.addConnection(tNode->nodeId, 0, cNode->nodeId, 0);
        _processorGraph.addConnection(tNode->nodeId, 1, cNode->nodeId, 1);
        _processorGraph.addConnection(cNode->nodeId, 0, OUTPUT_NODE_ID, 0);
        _processorGraph.addConnection(cNode->nodeId, 1, OUTPUT_NODE_ID, 1);

        _nextNodeID += 1;
        
    }

	void Mixer::remove()
	{

	}

    void Mixer::startPlayingAt(int64 position)
    {
        if (!_isPlaying) {
            for (auto current = _tracks.begin(), end = _tracks.end(); current != end; ++current) {
                current->second->getSource().prepareToPlay(_processorGraph.getBlockSize(), _processorGraph.getSampleRate());
                current->second->getSource().setNextReadPosition(position);
                current->second->getSource().start();
            }

            _isPlaying = true;
        }
    }

    void Mixer::pause()
    {
        if (_isPlaying) {
            for (auto current = _tracks.begin(), end = _tracks.end(); current != end; ++current) {
                current->second->getSource().stop();
            }

            _isPlaying = false;
        }
    }
    
    void Mixer::stop()
    {
        if (_isPlaying) {
            for (auto current = _tracks.begin(), end = _tracks.end(); current != end; ++current) {
                current->second->getSource().stop();
                current->second->getSource().releaseResources();
            }

            _isPlaying = false;
        }
    }
    
    void Mixer::muteTrack(int trackID)
    {
        for(auto current = _strips.begin(), end = _strips.end(); current != end; ++current)
        {
            if(current->second->getID() == NodeIDs::STRIP_BASE_NODE_ID + (trackID - 1))
            {
                current->second->setMuteParameter();
            }
        }
    }
    
    
    void Mixer::soloTrack(int trackID)
    {
        for(auto current = _strips.begin(), end = _strips.end(); current != end; ++current)
        {
            if(current->second->getID() != NodeIDs::STRIP_BASE_NODE_ID + (trackID - 1))
            {
                current->second->setMuteParameter();
            }
        }
    }
    
    void Mixer::changeGain(int trackID, float gain)
    {
        for(auto current = _strips.begin(), end = _strips.end(); current != end; ++current)
        {
            if(current->second->getID() == NodeIDs::STRIP_BASE_NODE_ID + (trackID - 1))
            {
                current->second->setParameter(StripParameter::GAIN, gain);
            }
        }
    }
    
    void Mixer::changePan(int trackID, float pan)
    {
        for(auto current = _strips.begin(), end = _strips.end(); current != end; ++current)
        {
            if(current->second->getID() == NodeIDs::STRIP_BASE_NODE_ID + (trackID - 1))
            {
                current->second->setParameter(StripParameter::PAN, pan);
            }
        }
    }
    
    int Mixer::getNumberOfStrips()
    {
        return _strips.size();
    }

	int Mixer::getNumberOfTracks()
	{
		return _tracks.size();
	}


    AudioProcessorGraph *Mixer::getProcessorGraph()
    {
        return &_processorGraph;
    }

	std::map<Track*, TrackProcessor*>* Mixer::getTrackMap()
	{
		return &_tracks;
	}

	std::map<TrackProcessor*, ChannelStripProcessor*>* Mixer::getStripMap()
	{
		return &_strips;
	}

    bool Mixer::isPlaying() const {
        return _isPlaying;
    }

    void Mixer::goToTheEnd() {
        auto maxLength = 0, tmp = 0;

        for (auto current = _tracks.begin(), end = _tracks.end(); current != end; ++current) {
            if ((tmp = current->first->getTotalLength()) > maxLength) {
                maxLength = tmp;
            }
        }

        for (auto current = _tracks.begin(), end = _tracks.end(); current != end; ++current) {
            current->second->getSource().setNextReadPosition(maxLength);
        }
    }
}
