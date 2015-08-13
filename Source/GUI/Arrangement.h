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
#include "../Audio/Engine.h"

//==============================================================================
/*
*/
class Arrangement : public Component {
public:
    Arrangement(const ApplicationCommandManager &commands, const Audio::Engine &engine);
    ~Arrangement();

    void paint(Graphics&);
    void resized();

private:
    Array<TrackComponent *> tracks;

    ScopedPointer<TimelineCursor> _cursor;
    const Audio::Engine &_engine;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Arrangement)
};


#endif // ARRANGEMENT_H_INCLUDED

