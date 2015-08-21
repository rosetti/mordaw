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

        AudioProcessorGraph *getProcessorGraph();

        bool isPlaying() const;
        void goToTheEnd();
        
        int getNumberOfStrips();

        enum NodeIDs
        {
            INPUT_NODE_ID = 0x100,
            OUTPUT_NODE_ID = 0x200,
            
            STRIP_BASE_NODE_ID = 0x11000,
        };

    private:
        int _nextNodeID;
        bool _isPlaying;
        TimeSliceThread _thread;
        AudioProcessorGraph _processorGraph;
        std::map<Track *, TrackProcessor *> _tracks;
        std::map<TrackProcessor *, ChannelStripProcessor *> _strips;

        int _numInput, _numOutput, _bufferSize;
        double _sampleRate;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Mixer)
    };
}




#endif  // MIXER_H_INCLUDED
