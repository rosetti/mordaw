/*
  ==============================================================================

    Arrangement.h
    Created: 10 Aug 2015 10:58:25pm
    Author:  dtl

  ==============================================================================
*/

#ifndef ARRANGEMENT_H_INCLUDED
#define ARRANGEMENT_H_INCLUDED

#include "TrackComponent.h"
#include "../../JuceLibraryCode/JuceHeader.h"
#include "TimelineCursor.h"
#include "TimelineComponent.h"
#include "../Audio/Engine.h"

//==============================================================================
/*
*/
class Arrangement : public Component {
public:
    Arrangement(ApplicationCommandManager &commands, const Audio::Engine &engine);
    ~Arrangement();

    void paint(Graphics&);
    void resized();
    void addTrack(Audio::Track* track);

private:
    int64 _mixerOffset, _timelineOffset;
    Array<TrackComponent *> _tracks;
    ScopedPointer<TimelineComponent> _timeline;
    ScopedPointer<TimelineCursor> _cursor;
    ScopedPointer<TextButton> _addTrackButton;
    ApplicationCommandManager &_commands;
    const Audio::Engine &_engine;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Arrangement)
};


#endif // ARRANGEMENT_H_INCLUDED

