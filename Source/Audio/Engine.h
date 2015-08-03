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

namespace Audio
{
    class Engine {
    public:
        Engine();
        ~Engine();

        void getCommandInfo(CommandID commandID, ApplicationCommandInfo &result) const;
        void getAllCommands(Array<CommandID>& commands) const;
        bool perform(const ApplicationCommandTarget::InvocationInfo & info);

        enum Commands {
            start = 0x200,
            stop = 0x201,
            pause = 0x202,
        };

    private:
        AudioDeviceManager _devices;
        AudioFormatManager _formats;
        ScopedPointer<Mixer> _mixer;
        AudioProcessorPlayer _player;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Engine)
    };
}

#endif  // ENGINE_H_INCLUDED
