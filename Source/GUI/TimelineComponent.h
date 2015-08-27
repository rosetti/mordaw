/*
  ==============================================================================

    TimelineComponent.h
    Created: 16 Aug 2015 8:26:16pm
    Author:  dtl

  ==============================================================================
*/

#ifndef TIMELINECOMPONENT_H_INCLUDED
#define TIMELINECOMPONENT_H_INCLUDED

#include "../../JuceLibraryCode/JuceHeader.h"
#include "TimelineClip.h"
#include "TimelineCursor.h"
#include "../Audio/Engine.h"

//==============================================================================
/*
*/
class TimelineComponent    : public Component
{
public:
    TimelineComponent(const Audio::Engine &engine, int64 numberOfClips, int64 mixerOffset);
    TimelineComponent(const Audio::Engine &engine, int64 numberOfClips, int64 pixelsPerClip, int64 mixerOffset);
    ~TimelineComponent();

    void paint (Graphics&);
    void resized();
    
    int getNumberOfClips();
    void setNumberOfClips(int64 numberOfClips);

private:
    void addClips(int64 numberOfClips);
    
    std::vector<TimelineClip*> _clips;
    ScopedPointer<TimelineCursor> _cursor;
    const Audio::Engine& _engine;
    int64 _mixerOffset;
    int64 _numberOfClips;
    int64 _pixelsPerClip;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TimelineComponent)
};


#endif  // TIMELINECOMPONENT_H_INCLUDED
