/*
==============================================================================

Engine.h
Created: 1 Aug 2015 4:20:04pm
Author:  Thomas

==============================================================================
*/

#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

namespace Audio
{
    class Engine {
    public:
        Engine();
        ~Engine();

    private:
        AudioDeviceManager _devices;
        AudioFormatManager _formats;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Engine)
    };
}

#endif  // ENGINE_H_INCLUDED
