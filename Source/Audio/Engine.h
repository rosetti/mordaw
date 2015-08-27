/*
==============================================================================

Engine.h
Created: 1 Aug 2015 4:20:04pm
Author:  Thomas

==============================================================================
*/

#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#include "Mixer.h"
#include "Track.h"
#include "Recorder.h"


namespace Audio
{
    class Engine {
    public:
        Engine(ApplicationCommandManager *commands);
        ~Engine();

        void getCommandInfo(CommandID commandID, ApplicationCommandInfo &result) const;
        void getAllCommands(Array<CommandID>& commands) const;
        bool perform(const ApplicationCommandTarget::InvocationInfo & info);
        double getCurrentSamplerate() const;
        // just temporary until we have a proper way of adding tracks.
        int64 getTotalLength() const;
        Mixer *getMixer() const;
        StringArray getRecordedFileNames() const;

        enum Commands {
            start = 0x200,
            stop = 0x201,
            pause = 0x202,
            rewind = 0x203,
            forward = 0x204,
            mute = 0x205,
            solo = 0x206,
            record = 0x207
        };

    private:
        AudioDeviceManager _devices;
        AudioFormatManager _formats;
        ScopedPointer<Mixer> _mixer;
        AudioProcessorPlayer _player;
        ApplicationCommandManager *_commands;
        AudioRecorder _recorder;
        
        OwnedArray<Track> tracks;
        
        int64 _totalLength;
        int64 _recordedFiles;
        StringArray _recordedFileNames;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Engine)
    };
}

#endif  // ENGINE_H_INCLUDED
