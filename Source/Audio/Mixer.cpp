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
        _isPlaying(false),
        _knownPlugins(),
        _index(0)    
    {
        #if defined(__APPLE__)
        _vstFormat = new VSTPluginFormat();
        FileSearchPath path("/Library/Audio/Plug-Ins/VST");
        scanner = new PluginDirectoryScanner(_knownPlugins, *_vstFormat, path, false, File::nonexistent);
        _auFormat = new AudioUnitPluginFormat();
        _pluginManager.addDefaultFormats();
        FileSearchPath path2("/Library/Audio/Plug-Ins/Components");
        scanner = new PluginDirectoryScanner(_knownPlugins, *_auFormat, path2, true, File::nonexistent);
        #endif
        #ifdef WIN32
        _vstFormat = new VSTPluginFormat();
        FileSearchPath path("C:/Program Files/Steinberg/VstPlugins");
        scanner = new PluginDirectoryScanner(_knownPlugins, *_vstFormat, path, false, File::nonexistent);
        #elif WIN64
        _vstFormat = new VSTPluginFormat();
        FileSearchPath path("C:/Program Files (x86)/Common Files/Steinberg/VSTPlugins;C:/Program Files/Steinberg/VSTPlugins");
        scanner = new PluginDirectoryScanner(_knownPlugins, *_vstFormat, path, false, File::nonexistent);
        #endif
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
        _processorGraph.clear();
    }

    void Mixer::add(Track * track)
    {
        TrackProcessor *processor = new TrackProcessor(track, &_thread);
        processor->setPlayConfigDetails(_numInput, _numOutput, _sampleRate, _bufferSize);
        
        ChannelStripProcessor *strip = new ChannelStripProcessor();
        strip->setPlayConfigDetails(_numInput, _numOutput, _sampleRate, _bufferSize);

        track->prepareToPlay(_bufferSize, _sampleRate);
        _tracks.insert(std::pair<Track *, TrackProcessor *>(track, processor));
        
        strip->prepareToPlay(_bufferSize, (int)_sampleRate);
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
    
    void Mixer::addPlugin1(int trackNumber, const PluginDescription *desc, double x, double y)
    {
        if(desc != 0)
        {
            String errorMessage;
            AudioPluginInstance* instance = _pluginManager.createPluginInstance(*desc, _processorGraph.getSampleRate(), _processorGraph.getBlockSize(), errorMessage);
            
            AudioProcessorGraph::Node* node = 0;
            
            if(instance != 0)
                node = _processorGraph.addNode(instance, PLUGIN_BASE_NODE_ID + (100 * trackNumber));
            if(node != 0)
            {
                _processorGraph.removeConnection(TRACK_BASE_NODE_ID + (trackNumber - 1), 0, STRIP_BASE_NODE_ID + (trackNumber - 1), 0);
                _processorGraph.removeConnection(TRACK_BASE_NODE_ID + (trackNumber - 1), 1, STRIP_BASE_NODE_ID + (trackNumber - 1), 1);
                
                _processorGraph.addConnection(TRACK_BASE_NODE_ID + (trackNumber - 1), 0, node->nodeId, 0);
                _processorGraph.addConnection(TRACK_BASE_NODE_ID + (trackNumber - 1), 1, node->nodeId, 1);
                _processorGraph.addConnection(node->nodeId, 0, STRIP_BASE_NODE_ID + (trackNumber - 1), 0);
                _processorGraph.addConnection(node->nodeId, 1, STRIP_BASE_NODE_ID + (trackNumber - 1), 1);
                
                node->properties.set("x", x);
                node->properties.set("y", y);
            }
        }
    }
    
    void Mixer::addPlugin2(int trackNumber, const PluginDescription *desc, double x, double y)
    {
        if(desc != 0)
        {
            String errorMessage;
            AudioPluginInstance* instance = _pluginManager.createPluginInstance(*desc, _processorGraph.getSampleRate(), _processorGraph.getBlockSize(), errorMessage);
            
            AudioProcessorGraph::Node* node = 0;
            
            if(instance != 0)
                node = _processorGraph.addNode(instance, PLUGIN_BASE_NODE_ID + (100 * trackNumber) + 1);
            if(node != 0)
            {
                _processorGraph.removeConnection(PLUGIN_BASE_NODE_ID + (100 * trackNumber), 0, STRIP_BASE_NODE_ID + (trackNumber - 1), 0);
                _processorGraph.removeConnection(PLUGIN_BASE_NODE_ID + (100 * trackNumber), 1, STRIP_BASE_NODE_ID + (trackNumber - 1), 1);
                
                _processorGraph.addConnection(PLUGIN_BASE_NODE_ID + (100 * trackNumber), 0, node->nodeId, 0);
                _processorGraph.addConnection(PLUGIN_BASE_NODE_ID + (100 * trackNumber), 1, node->nodeId, 1);
                _processorGraph.addConnection(node->nodeId, 0, STRIP_BASE_NODE_ID + (trackNumber - 1), 0);
                _processorGraph.addConnection(node->nodeId, 1, STRIP_BASE_NODE_ID + (trackNumber - 1), 1);
                
                node->properties.set("x", x);
                node->properties.set("y", y);
            }
        }
    }
    
    void Mixer::addPlugin3(int trackNumber, const PluginDescription *desc, double x, double y)
    {
        if(desc != 0)
        {
            String errorMessage;
            AudioPluginInstance* instance = _pluginManager.createPluginInstance(*desc, _processorGraph.getSampleRate(), _processorGraph.getBlockSize(), errorMessage);
            
            AudioProcessorGraph::Node* node = 0;
            
            if(instance != 0)
                node = _processorGraph.addNode(instance, PLUGIN_BASE_NODE_ID + (100 * trackNumber) + 2);
            if(node != 0)
            {
                _processorGraph.removeConnection(PLUGIN_BASE_NODE_ID + (100 * trackNumber) + 1, 0, STRIP_BASE_NODE_ID + (trackNumber - 1), 0);
                _processorGraph.removeConnection(PLUGIN_BASE_NODE_ID + (100 * trackNumber) + 1, 1, STRIP_BASE_NODE_ID + (trackNumber - 1), 1);
                
                _processorGraph.addConnection(PLUGIN_BASE_NODE_ID + (100 * trackNumber) + 1, 0, node->nodeId, 0);
                _processorGraph.addConnection(PLUGIN_BASE_NODE_ID + (100 * trackNumber) + 1, 1, node->nodeId, 1);
                _processorGraph.addConnection(node->nodeId, 0, STRIP_BASE_NODE_ID + (trackNumber - 1), 0);
                _processorGraph.addConnection(node->nodeId, 1, STRIP_BASE_NODE_ID + (trackNumber - 1), 1);
                
                node->properties.set("x", x);
                node->properties.set("y", y);
            }
        }
    }
    
    void Mixer::addPlugin4(int trackNumber, const PluginDescription *desc, double x, double y)
    {
        if(desc != 0)
        {
            String errorMessage;
            AudioPluginInstance* instance = _pluginManager.createPluginInstance(*desc, _processorGraph.getSampleRate(), _processorGraph.getBlockSize(), errorMessage);
            
            AudioProcessorGraph::Node* node = 0;
            
            if(instance != 0)
                node = _processorGraph.addNode(instance, PLUGIN_BASE_NODE_ID + (100 * trackNumber) + 3);
            if(node != 0)
            {
                _processorGraph.removeConnection(PLUGIN_BASE_NODE_ID + (100 * trackNumber) + 2, 0, STRIP_BASE_NODE_ID + (trackNumber - 1), 0);
                _processorGraph.removeConnection(PLUGIN_BASE_NODE_ID + (100 * trackNumber) + 2, 1, STRIP_BASE_NODE_ID + (trackNumber - 1), 1);
                
                _processorGraph.addConnection(PLUGIN_BASE_NODE_ID + (100 * trackNumber) + 2, 0, node->nodeId, 0);
                _processorGraph.addConnection(PLUGIN_BASE_NODE_ID + (100 * trackNumber) + 2, 1, node->nodeId, 1);
                _processorGraph.addConnection(node->nodeId, 0, STRIP_BASE_NODE_ID + (trackNumber - 1), 0);
                _processorGraph.addConnection(node->nodeId, 1, STRIP_BASE_NODE_ID + (trackNumber - 1), 1);
                
                node->properties.set("x", x);
                node->properties.set("y", y);
            }
        }
    }
    
    void Mixer::addPreFaderPlugin(int trackNumber, const PluginDescription *desc, double x, double y)
    {
        if(desc != 0)
        {
            String errorMessage;
            AudioPluginInstance* instance = _pluginManager.createPluginInstance(*desc, _processorGraph.getSampleRate(), _processorGraph.getBlockSize(), errorMessage);
            
            AudioProcessorGraph::Node* node = 0;
            
            if(instance != 0)
                node = _processorGraph.addNode(instance, PLUGIN_BASE_NODE_ID + (100 * trackNumber) + _index++);
            if(node != 0)
            {
                _processorGraph.removeConnection(TRACK_BASE_NODE_ID + (trackNumber - 1), 0, OUTPUT_NODE_ID, 0);
                _processorGraph.removeConnection(TRACK_BASE_NODE_ID + (trackNumber - 1), 1, OUTPUT_NODE_ID, 1);
                
                _processorGraph.addConnection(TRACK_BASE_NODE_ID + (trackNumber - 1), 0, node->nodeId, 0);
                _processorGraph.addConnection(TRACK_BASE_NODE_ID + (trackNumber - 1), 1, node->nodeId, 1);
                _processorGraph.addConnection(node->nodeId, 0, STRIP_BASE_NODE_ID + (trackNumber - 1), 0);
                _processorGraph.addConnection(node->nodeId, 1, STRIP_BASE_NODE_ID + (trackNumber - 1), 1);
                
                node->properties.set("x", x);
                node->properties.set("y", y);
            }
        }
    }
    
    void Mixer::addPostFaderPlugin(int trackNumber, const PluginDescription *desc, double x, double y)
    {
        if(desc != 0)
        {
            String errorMessage;
            AudioPluginInstance* instance = _pluginManager.createPluginInstance(*desc, _processorGraph.getSampleRate(), _processorGraph.getBlockSize(), errorMessage);
            
            AudioProcessorGraph::Node* node = 0;
            
            if(instance != 0)
                node = _processorGraph.addNode(instance, PLUGIN_BASE_NODE_ID + (100 * trackNumber) + _index++);
            if(node != 0)
            {
                _processorGraph.removeConnection(STRIP_BASE_NODE_ID + (trackNumber - 1), 0, OUTPUT_NODE_ID, 0);
                _processorGraph.removeConnection(STRIP_BASE_NODE_ID + (trackNumber - 1), 1, OUTPUT_NODE_ID, 1);
            
                for(int i = 1; i < _index; ++i)
                {
                    _processorGraph.addConnection(STRIP_BASE_NODE_ID + (trackNumber - 1), 0, node->nodeId, 0);
                    _processorGraph.addConnection(STRIP_BASE_NODE_ID + (trackNumber - 1), 1, node->nodeId, 1);
                }

                _processorGraph.addConnection(node->nodeId, 0, OUTPUT_NODE_ID, 0);
                _processorGraph.addConnection(node->nodeId, 1, OUTPUT_NODE_ID, 1);
                
                node->properties.set("x", x);
                node->properties.set("y", y);
            }
            
        }
    }
    
    void Mixer::addParallelPlugin(int trackNumber, const PluginDescription *desc, double x, double y)
    {
        if(desc != 0)
        {
            String errorMessage;
            AudioPluginInstance* instance = _pluginManager.createPluginInstance(*desc, _processorGraph.getSampleRate(), _processorGraph.getBlockSize(), errorMessage);
        
            AudioProcessorGraph::Node* node = 0;
            
            if(instance != 0)
                node = _processorGraph.addNode(instance, PLUGIN_BASE_NODE_ID + (100 * trackNumber) + _index++);
            if(node != 0)
            {
                _processorGraph.removeConnection(TRACK_BASE_NODE_ID + (trackNumber - 1), 0, OUTPUT_NODE_ID, 0);
                _processorGraph.removeConnection(TRACK_BASE_NODE_ID + (trackNumber - 1), 1, OUTPUT_NODE_ID, 1);
                
                _processorGraph.addConnection(TRACK_BASE_NODE_ID + (trackNumber - 1), 0, node->nodeId, 0);
                _processorGraph.addConnection(TRACK_BASE_NODE_ID + (trackNumber - 1), 1, node->nodeId, 1);
                _processorGraph.addConnection(node->nodeId, 0, STRIP_BASE_NODE_ID + (trackNumber - 1), 0);
                _processorGraph.addConnection(node->nodeId, 1, STRIP_BASE_NODE_ID + (trackNumber - 1), 1);
                
                node->properties.set("x", x);
                node->properties.set("y", y);
                
                instance->createEditor();
                instance->hasEditor();
                instance->getParameter(1);
            }
        }
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
    
    int64 Mixer::getLongestTrack()
    {
        int64 longestTrackWidth_ = 0;
        for (auto currentTrack = _tracks.begin(), tracksEnd = _tracks.end(); currentTrack != tracksEnd; ++currentTrack)
        {
            int64 currentTrackLength_ = currentTrack->first->getTotalLength();
            if (currentTrackLength_ > longestTrackWidth_)
            {
                longestTrackWidth_ = currentTrackLength_;
            }
            return longestTrackWidth_;
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

    KnownPluginList& Mixer::getKnownPluginList()
    {
        return _knownPlugins;
    }
    
    AudioPluginFormatManager& Mixer::getFormatManager()
    {
        return _pluginManager;
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
            if ((tmp = (int)current->first->getTotalLength()) > maxLength) {
                maxLength = tmp;
            }
        }

        for (auto current = _tracks.begin(), end = _tracks.end(); current != end; ++current) {
            current->second->getSource().setNextReadPosition(maxLength);
        }
    }
}
