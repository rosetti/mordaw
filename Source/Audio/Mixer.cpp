/*
==============================================================================

Mixer.cpp
Created: 2 Aug 2015 4:04:13pm
Author: Matt & Dan

==============================================================================
*/

#include "Mixer.h"

/*
Mixer is a class that takes all track objects and mixes together their audio streams. Tracks are connected to an
AudioProcessorGraph object which in turn allows audio to flow through to the output device
*/
namespace Audio
{
	/*
	Constructs a mixer object
	@param numInputChannels The number of input channels required
	@param numOutputChannels The number of output channels required
	@param sampleRate the curren sample rate
	@param bufferSize The size of the current buffer
	*/
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
		//If the operating system being used is an Apple system; set the default paths for VST and AudioUnit plugins
        #if defined(__APPLE__)
		
		//Create a VST format object
        _vstFormat = new VSTPluginFormat();
		//Set the VST path
        FileSearchPath path("/Library/Audio/Plug-Ins/VST");
		//Create a scanner object to detect VST plugins at the set location
        scanner = new PluginDirectoryScanner(_knownPlugins, *_vstFormat, path, false, File::nonexistent);
		
		//Create an Audio Unit object
        _auFormat = new AudioUnitPluginFormat();
        //Set the Audio Unit Path
        FileSearchPath path2("/Library/Audio/Plug-Ins/Components");
		//Create a scanner object to detect Audio Units at the set location
        scanner = new PluginDirectoryScanner(_knownPlugins, *_auFormat, path2, true, File::nonexistent);

		//Add default formats (eg VST) to the plugin manager
		_pluginManager.addDefaultFormats();
        #endif

		//If the operating system is a 32 bit Windows system; set the default path for VST plugins
        #ifdef WIN32

		//Create a VST format object
        _vstFormat = new VSTPluginFormat();
		//Set the VST path
        FileSearchPath path("C:/Program Files/Steinberg/VstPlugins");
		//Create a scanner object to detect VST plugins at the set location
        scanner = new PluginDirectoryScanner(_knownPlugins, *_vstFormat, path, false, File::nonexistent);

		//If the operating system is a 64 bit Windows system; set the default path for VST plugins
        #elif WIN64

		//Create a VST format object
        _vstFormat = new VSTPluginFormat();
		//Set the VST path
        FileSearchPath path("C:/Program Files (x86)/Common Files/Steinberg/VSTPlugins;C:/Program Files/Steinberg/VSTPlugins");
		//Create a scanner object to detect VST plugins at the set location
        scanner = new PluginDirectoryScanner(_knownPlugins, *_vstFormat, path, false, File::nonexistent);
        #endif
		
		//Create a processor object which handles exporting projects to various formats (currently just WAV) and configure it.
        _exportProcessor = new ExportProcessor();
		_exportProcessor->setPlayConfigDetails(_numInput, _numOutput, _sampleRate, _bufferSize);

		//Create a Channel Strip that has controls for the overall output and configure it.
        _masterStrip = new ChannelStripProcessor();
		_masterStrip->setPlayConfigDetails(_numInput, _numOutput, _sampleRate, _bufferSize);

		//Create an input node that allows audio from input devices (,icrophones, instruments etc.) to flow through the graph
        auto input = new AudioProcessorGraph::AudioGraphIOProcessor(AudioProcessorGraph::AudioGraphIOProcessor::audioInputNode);
		//Create an output node that allows audio to flow out of the graph to the systems pre-selected (or default) output device
        auto output = new AudioProcessorGraph::AudioGraphIOProcessor(AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode);

		//Set the graphs configuration
        _processorGraph.setPlayConfigDetails(numInputChannels, numOutputChannels, sampleRate, bufferSize);

		//Add the input and output nodes to the graph
        _processorGraph.addNode(input, INPUT_NODE_ID);
        _processorGraph.addNode(output, OUTPUT_NODE_ID);

		//Add the export node to the graph
        _processorGraph.addNode(_exportProcessor, EXPORT_NODE_ID);
		//Add the master strip to the graph
        _processorGraph.addNode(_masterStrip, MASTER_STRIP_NODE_ID);

		//Connect the master strip to the export node
		_processorGraph.addConnection(MASTER_STRIP_NODE_ID, 0, EXPORT_NODE_ID, 0);
		_processorGraph.addConnection(MASTER_STRIP_NODE_ID, 1, EXPORT_NODE_ID, 1);

		//Connect the  master strip to the output node
		_processorGraph.addConnection(MASTER_STRIP_NODE_ID, 0, OUTPUT_NODE_ID, 0);
		_processorGraph.addConnection(MASTER_STRIP_NODE_ID, 1, OUTPUT_NODE_ID, 1);
	}

    Mixer::~Mixer()
    {
        _thread.stopThread(0);
        stop();
    }

	/*
	Adds a Track object to the mixer.
	*/
    void Mixer::add(Track * track)
    {
		//Create a new track processor and configure it. This handles pushing the audio stream through the graph
        TrackProcessor *processor = new TrackProcessor(track, &_thread);
        processor->setPlayConfigDetails(_numInput, _numOutput, _sampleRate, _bufferSize);
        
		//Create a channel strip processor for the track and configure it
        ChannelStripProcessor *strip = new ChannelStripProcessor();
        strip->setPlayConfigDetails(_numInput, _numOutput, _sampleRate, _bufferSize);

		//Prepare the track to be played
        track->prepareToPlay(_bufferSize, _sampleRate);
		//Add the Track and its associated Track Processor to a map
        _tracks.insert(std::pair<Track *, TrackProcessor *>(track, processor));
        
		//Prepare the channel strip to be played
        strip->prepareToPlay(_bufferSize, (int)_sampleRate);
		//Add the TrackProcessor and its associated Channel Strip to a map
        _strips.insert(std::pair<TrackProcessor *, ChannelStripProcessor *>(processor, strip));
        strip->setID(_nextNodeID + 0x1000);

		//Create and add nodes to the graph for the track and the channel strip
        auto tNode = _processorGraph.addNode(processor, _nextNodeID);
        auto cNode = _processorGraph.addNode(strip, _nextNodeID + 0x1000);
		//Connect the track node to the channel strip node to allow audio to flow through them
        _processorGraph.addConnection(tNode->nodeId, 0, cNode->nodeId, 0);
        _processorGraph.addConnection(tNode->nodeId, 1, cNode->nodeId, 1);
		//Connect the channel strip node to the master strip to allow audio to flow through them and onwards ot the output node
        _processorGraph.addConnection(cNode->nodeId, 0, MASTER_STRIP_NODE_ID, 0);
        _processorGraph.addConnection(cNode->nodeId, 1, MASTER_STRIP_NODE_ID, 1);

		//Increment the next nodes ID for future additions
        _nextNodeID += 1;
    }
    
	void Mixer::addPlugin(int trackNumber, int pluginNumber, const PluginDescription *desc, double x, double y)
	{
		if (desc != 0)
		{
			String errorMessage;
			AudioPluginInstance* instance = _pluginManager.createPluginInstance(*desc, _processorGraph.getSampleRate(),
				_processorGraph.getBlockSize(), errorMessage);

			AudioProcessorGraph::Node* node = 0;

			if (instance != 0) {
				node = _processorGraph.addNode(instance, PLUGIN_BASE_NODE_ID + (100 * trackNumber) + pluginNumber);
			}
			if (node != 0)
			{
				if (trackNumber == 0) {
					if (pluginNumber == 4) {
						_processorGraph.removeConnection(MASTER_STRIP_NODE_ID, 0, EXPORT_NODE_ID, 0);
						_processorGraph.removeConnection(MASTER_STRIP_NODE_ID, 1, EXPORT_NODE_ID, 0);
						_processorGraph.removeConnection(MASTER_STRIP_NODE_ID, 0, OUTPUT_NODE_ID, 0);
						_processorGraph.removeConnection(MASTER_STRIP_NODE_ID, 1, OUTPUT_NODE_ID, 1);

						_processorGraph.addConnection(MASTER_STRIP_NODE_ID, 0, node->nodeId, 0);
						_processorGraph.addConnection(MASTER_STRIP_NODE_ID, 1, node->nodeId, 1);
						_processorGraph.addConnection(node->nodeId, 0, EXPORT_NODE_ID, 0);
						_processorGraph.addConnection(node->nodeId, 1, EXPORT_NODE_ID, 1);
						_processorGraph.addConnection(node->nodeId, 0, OUTPUT_NODE_ID, 0);
						_processorGraph.addConnection(node->nodeId, 1, OUTPUT_NODE_ID, 1);
					}
					else if (pluginNumber > 4 && pluginNumber < 8)
					{
						_processorGraph.removeConnection(PLUGIN_BASE_NODE_ID + (100 * trackNumber) + (pluginNumber - 1), 0, EXPORT_NODE_ID, 0);
						_processorGraph.removeConnection(PLUGIN_BASE_NODE_ID + (100 * trackNumber) + (pluginNumber - 1), 1, EXPORT_NODE_ID, 1);
						_processorGraph.removeConnection(PLUGIN_BASE_NODE_ID + (100 * trackNumber) + (pluginNumber - 1), 0, OUTPUT_NODE_ID, 0);
						_processorGraph.removeConnection(PLUGIN_BASE_NODE_ID + (100 * trackNumber) + (pluginNumber - 1), 1, OUTPUT_NODE_ID, 1);

						_processorGraph.addConnection(PLUGIN_BASE_NODE_ID + (100 * trackNumber) + (pluginNumber - 1), 0, node->nodeId, 0);
						_processorGraph.addConnection(PLUGIN_BASE_NODE_ID + (100 * trackNumber) + (pluginNumber - 1), 1, node->nodeId, 1);
						_processorGraph.addConnection(node->nodeId, 0, EXPORT_NODE_ID, 0);
						_processorGraph.addConnection(node->nodeId, 1, EXPORT_NODE_ID, 1);
						_processorGraph.addConnection(node->nodeId, 0, OUTPUT_NODE_ID, 0);
						_processorGraph.addConnection(node->nodeId, 1, OUTPUT_NODE_ID, 1);
					}
				}
				else {
					//Prefade plugins are 0, 1, 2 & 3
					if (pluginNumber == 0) {
						_processorGraph.removeConnection(TRACK_BASE_NODE_ID + (trackNumber - 1), 0, STRIP_BASE_NODE_ID + (trackNumber - 1), 0);
						_processorGraph.removeConnection(TRACK_BASE_NODE_ID + (trackNumber - 1), 1, STRIP_BASE_NODE_ID + (trackNumber - 1), 1);

						//_processorGraph.addConnection(TRACK_BASE_NODE_ID + (trackNumber - 1) + (pluginNumber), 0, node->nodeId, 0);
						//_processorGraph.addConnection(TRACK_BASE_NODE_ID + (trackNumber - 1) + (pluginNumber), 1, node->nodeId, 1);
						_processorGraph.addConnection(TRACK_BASE_NODE_ID + (trackNumber - 1), 0, node->nodeId, 0);
						_processorGraph.addConnection(TRACK_BASE_NODE_ID + (trackNumber - 1), 1, node->nodeId, 1);

						_processorGraph.addConnection(node->nodeId, 0, STRIP_BASE_NODE_ID + (trackNumber - 1), 0);
						_processorGraph.addConnection(node->nodeId, 1, STRIP_BASE_NODE_ID + (trackNumber - 1), 1);
					}
					else if (pluginNumber > 0 && pluginNumber < 4)
					{
						_processorGraph.removeConnection(PLUGIN_BASE_NODE_ID + (100 * trackNumber) + (pluginNumber - 1), 0, STRIP_BASE_NODE_ID + (trackNumber - 1), 0);
						_processorGraph.removeConnection(PLUGIN_BASE_NODE_ID + (100 * trackNumber) + (pluginNumber - 1), 1, STRIP_BASE_NODE_ID + (trackNumber - 1), 1);

						_processorGraph.addConnection(PLUGIN_BASE_NODE_ID + (100 * trackNumber) + (pluginNumber - 1), 0, node->nodeId, 0);
						_processorGraph.addConnection(PLUGIN_BASE_NODE_ID + (100 * trackNumber) + (pluginNumber - 1), 1, node->nodeId, 1);

						_processorGraph.addConnection(node->nodeId, 0, STRIP_BASE_NODE_ID + (trackNumber - 1), 0);
						_processorGraph.addConnection(node->nodeId, 1, STRIP_BASE_NODE_ID + (trackNumber - 1), 1);
					}
					//Postfade plugins are 4, 5, 6 & 7
					else if (pluginNumber == 4)
					{
						_processorGraph.removeConnection(STRIP_BASE_NODE_ID + (trackNumber - 1), 0, OUTPUT_NODE_ID, 0);
						_processorGraph.removeConnection(STRIP_BASE_NODE_ID + (trackNumber - 1), 1, OUTPUT_NODE_ID, 1);

						_processorGraph.addConnection(STRIP_BASE_NODE_ID + (trackNumber - 1), 0, node->nodeId, 0);
						_processorGraph.addConnection(STRIP_BASE_NODE_ID + (trackNumber - 1), 1, node->nodeId, 1);
						_processorGraph.addConnection(node->nodeId, 0, OUTPUT_NODE_ID, 0);
						_processorGraph.addConnection(node->nodeId, 1, OUTPUT_NODE_ID, 1);
					}
					else if (pluginNumber > 4 && pluginNumber < 8)
					{
						_processorGraph.removeConnection(PLUGIN_BASE_NODE_ID + (100 * trackNumber) + (pluginNumber - 1), 0, OUTPUT_NODE_ID, 0);
						_processorGraph.removeConnection(PLUGIN_BASE_NODE_ID + (100 * trackNumber) + (pluginNumber - 1), 1, OUTPUT_NODE_ID + (trackNumber - 1), 1);

						_processorGraph.addConnection(PLUGIN_BASE_NODE_ID + (100 * trackNumber) + (pluginNumber - 1), 0, node->nodeId, 0);
						_processorGraph.addConnection(PLUGIN_BASE_NODE_ID + (100 * trackNumber) + (pluginNumber - 1), 1, node->nodeId, 1);

						_processorGraph.addConnection(node->nodeId, 0, OUTPUT_NODE_ID, 0);
						_processorGraph.addConnection(node->nodeId, 1, OUTPUT_NODE_ID, 1);
					}
				}
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
		if (trackID == 0)
		{
			_masterStrip->setMuteParameter();
		}
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
		if (trackID == 0)
		{
			_masterStrip->setParameter(StripParameter::GAIN, gain);
		}
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
		if (trackID == 0)
		{
			_masterStrip->setParameter(StripParameter::PAN, pan);
		}
        for(auto current = _strips.begin(), end = _strips.end(); current != end; ++current)
        {
            if(current->second->getID() == NodeIDs::STRIP_BASE_NODE_ID + (trackID - 1))
            {
                current->second->setParameter(StripParameter::PAN, pan);
            }
        }
    }
    
    void Mixer::startExporting(const File &file)
    {
        _exportProcessor->startExporting(file);
    }
    
    void Mixer::stopExporting()
    {
        _exportProcessor->stopExporting();
    }
    
    bool Mixer::isExporting()
    {
        return _exportProcessor->isExporting();
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
        }
        return longestTrackWidth_;
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

	ChannelStripProcessor* Mixer::getMasterStrip()
	{
		return _masterStrip;
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
