/*
  ==============================================================================

    TrackComponent.h
    Created: 12 Aug 2015 10:17:59pm
    Author:  Thomas

  ==============================================================================
*/

#ifndef TRACKCOMPONENT_H_INCLUDED
#define TRACKCOMPONENT_H_INCLUDED

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../Audio/Track.h"

//==============================================================================
/*
*/
class TrackComponent    : public Component
{
public:
    TrackComponent(Audio::Track *track);
    ~TrackComponent();

    void paint (Graphics&);
    void resized();

private:
    Audio::Track *_track;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TrackComponent)
};


#endif  // TRACKCOMPONENT_H_INCLUDED
