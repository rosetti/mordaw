/*
  ==============================================================================

    Mixer.h
    Created: 2 Aug 2015 4:04:13pm
    Author:  Thomas

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
        
        void addPlugin(int trackNumber, const PluginDescription* desc, double x, double y);

    private:
        int _nextNodeID;
        bool _isPlaying;
        TimeSliceThread _thread;
        AudioProcessorGraph _processorGraph;
        AudioPluginFormatManager _pluginManager;
        KnownPluginList _knownPlugins;
        ScopedPointer<PluginDirectoryScanner> scanner;
        VSTPluginFormat* _vstFormat;
        #if defined(__APPLE__)
        AudioUnitPluginFormat* _auFormat;
        #endif
        std::map<Track *, TrackProcessor *> _tracks;
        std::map<TrackProcessor *, ChannelStripProcessor *> _strips;

        int _numInput, _numOutput, _bufferSize;
        double _sampleRate;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Mixer)
    };
}




#endif  // MIXER_H_INCLUDED
