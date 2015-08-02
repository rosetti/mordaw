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
        _bufferSize(bufferSize)
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

        track->prepareToPlay(_bufferSize, _sampleRate);
        _tracks.insert(std::pair<Track *, TrackProcessor *>(track, processor));

        auto node = _processorGraph.addNode(processor, _nextNodeID);
        jassert(_processorGraph.addConnection(node->nodeId, 0, OUTPUT_NODE_ID, 0));
        _processorGraph.addConnection(node->nodeId, 1, OUTPUT_NODE_ID, 1);

        _nextNodeID += 1;
    }

    void Mixer::startPlayingAt(int64 position)
    {
        for (auto current = _tracks.begin(), end = _tracks.end(); current != end; ++current) {
            current->second->getSource().prepareToPlay(_processorGraph.getBlockSize(), _processorGraph.getSampleRate());
            current->second->getSource().setNextReadPosition(position);
            current->second->getSource().start();
        }
    }
    
    void Mixer::stop()
    {
        for (auto current = _tracks.begin(), end = _tracks.end(); current != end; ++current) {
            current->second->getSource().stop();
            current->second->getSource().releaseResources();
        }
    }

    AudioProcessorGraph *Mixer::getProcessorGraph()
    {
        return &_processorGraph;
    }
}
