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

namespace Audio
{
    class Mixer {
    public:
        Mixer(int numInputChannels, int numOutputChannels, double sampleRate, int bufferSize);
        ~Mixer();

        void add(Track *track);

        void startPlayingAt(int64 position = 0);
        void stop();

        AudioProcessorGraph *getProcessorGraph();

        enum NodeIDs
        {
            INPUT_NODE_ID = 0x100,
            OUTPUT_NODE_ID = 0x200,
        };

    private:
        int _nextNodeID;
        TimeSliceThread _thread;
        AudioProcessorGraph _processorGraph;
        std::map<Track *, TrackProcessor *> _tracks;
    };
}




#endif  // MIXER_H_INCLUDED
