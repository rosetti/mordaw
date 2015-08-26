/*
  ==============================================================================

    Mixer.h
    Created: 2 Aug 2015 4:04:13pm

  ==============================================================================
*/

#ifndef MIXER_H_INCLUDED
#define MIXER_H_INCLUDED

#include "TrackProcessor.h"
#include "ChannelStripProcessor.h"

namespace Audio
{
    class Mixer {
    public:
        Mixer(int numInputChannels, int numOutputChannels, double sampleRate, int bufferSize);
        ~Mixer();

        void add(Track *track);
		void remove();

        void startPlayingAt(int64 position = -1);
        void pause();
        void stop();
        
        enum StripParameter
        {
            GAIN = 1,
            PAN = 2,
            MUTE = 3
        };
        
        void muteTrack(int trackID);
        void soloTrack(int trackID);
        void changeGain(int trackID, float gain);
        void changePan(int trackID, float pan);
        
        KnownPluginList& getKnownPluginList();
        AudioPluginFormatManager& getFormatManager();

        AudioProcessorGraph *getProcessorGraph();
		std::map<Track *, TrackProcessor *> *getTrackMap();
		std::map<TrackProcessor *, ChannelStripProcessor *> *getStripMap();

        bool isPlaying() const;
        void goToTheEnd();
        
        int getNumberOfStrips();
		int getNumberOfTracks();

        enum NodeIDs
        {
            INPUT_NODE_ID = 0x100,
            OUTPUT_NODE_ID = 0x200,
            TRACK_BASE_NODE_ID = 0x10000,
            STRIP_BASE_NODE_ID = 0x11000,
            PLUGIN_BASE_NODE_ID = 0x20000
        };
        
        /* Mixer plugin functions */
        // add mixer plugins
        void addPlugin1(int trackNumbwe, const PluginDescription* desc, double x, double y);
        void addPlugin2(int trackNumbwe, const PluginDescription* desc, double x, double y);
        void addPlugin3(int trackNumbwe, const PluginDescription* desc, double x, double y);
        void addPlugin4(int trackNumbwe, const PluginDescription* desc, double x, double y);
        
        /* Other plugin functions */
        // adds a single plugin post fader
        void addPostFaderPlugin(int trackNumber, const PluginDescription* desc, double x, double y);
        // adds a single plugin post fader
        void addPreFaderPlugin(int trackNumber, const PluginDescription* desc, double x, double y);
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
        // processor maps
        std::map<Track *, TrackProcessor *> _tracks;
        std::map<TrackProcessor *, ChannelStripProcessor *> _strips;

        int _numInput, _numOutput, _bufferSize;
        double _sampleRate;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Mixer)
    };
}




#endif  // MIXER_H_INCLUDED
