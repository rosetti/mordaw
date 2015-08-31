/*
  ==============================================================================

    Mixer.h
    Created: 2 Aug 2015 4:04:13pm
	Author: Matt & Dan

  ==============================================================================
*/

#ifndef MIXER_H_INCLUDED
#define MIXER_H_INCLUDED

#include "ExportProcessor.h"
#include "TrackProcessor.h"
#include "ChannelStripProcessor.h"

namespace Audio
{
    class Mixer {
    public:
        Mixer(int numInputChannels, int numOutputChannels, double sampleRate, int bufferSize);
        ~Mixer();

        // add and remove tracks
        void add(Track *track);
		void remove();

        // transport methods
        void startPlayingAt(int64 position = -1);
        void pause();
        void stop();
        
        enum StripParameter
        {
            GAIN = 1,
            PAN = 2,
            MUTE = 3
        };
        
        // channel strip controls
        void muteTrack(int trackID);
        void soloTrack(int trackID);
        void changeGain(int trackID, float gain);
        void changePan(int trackID, float pan);
        
        void startExporting(const File& file);
        void stopExporting();
        bool isExporting();
        
        // return the list of recoginised plugins
        KnownPluginList& getKnownPluginList();
        
        // return reference to format manager
        AudioPluginFormatManager& getFormatManager();

        // return the processor graph
        AudioProcessorGraph *getProcessorGraph();
        
        // return the processor maps
		std::map<Track *, TrackProcessor *> *getTrackMap();
		std::map<TrackProcessor *, ChannelStripProcessor *> *getStripMap();

        bool isPlaying() const;
        void goToTheEnd();
        
        // get the current number of channel strips
        int getNumberOfStrips();
        // get the current number of tracks
		int getNumberOfTracks();
        
        // get the longest track length
        int64 getLongestTrack();

        enum NodeIDs
        {
            INPUT_NODE_ID = 0x100,
            OUTPUT_NODE_ID = 0x200,
            EXPORT_NODE_ID = 0x300,
            MASTER_STRIP_NODE_ID = 0x1000,
            TRACK_BASE_NODE_ID = 0x10000,
            STRIP_BASE_NODE_ID = 0x11000,
            PLUGIN_BASE_NODE_ID = 0x20000
        };
        
        /* Mixer plugin functions */
        // add mixer plugins
		void addPlugin(int trackNumber, int pluginNumber, bool preFade, const PluginDescription *desc, double x, double y);
        
        /* Other plugin functions */
        // adds a single plugin post fader
        void addPostFaderPlugin(int trackNumber, const PluginDescription* desc, double x, double y);
        // adds limitless amounts of plugins
        void addParallelPlugin(int trackNumber, const PluginDescription* desc, double x, double y);

    private:
        int _index;
        int _nextNodeID;
        bool _isPlaying;
        TimeSliceThread _thread;
        AudioProcessorGraph _processorGraph;
        AudioPluginFormatManager _pluginManager;
        // holds a list of plugins
        KnownPluginList _knownPlugins;
        // a scanner for plugins
        ScopedPointer<PluginDirectoryScanner> scanner;
        /* Define plugin types */
        // vst plugin format on all platforms
        ScopedPointer<VSTPluginFormat> _vstFormat;
        // audio unit on mac devices
        #if defined(__APPLE__)
        ScopedPointer<AudioUnitPluginFormat> _auFormat;
        #endif
        ExportProcessor* _exportProcessor;
       ChannelStripProcessor* _masterStrip;
        // processor maps
        std::map<Track *, TrackProcessor *> _tracks;
        std::map<TrackProcessor *, ChannelStripProcessor *> _strips;

        int _numInput, _numOutput, _bufferSize;
        double _sampleRate;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Mixer)
    };
}




#endif  // MIXER_H_INCLUDED
