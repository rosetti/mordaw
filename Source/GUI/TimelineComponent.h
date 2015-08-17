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

//==============================================================================
/*
*/
class TimelineComponent    : public Component
{
public:
    TimelineComponent(int64 numberOfClips, int64 mixerOffset);
    TimelineComponent(int64 numberOfClips, int64 pixelsPerClip, int64 mixerOffset);
    ~TimelineComponent();

    void paint (Graphics&);
    void resized();

private:
    std::vector<TimelineClip*> _clips;
    int64 _numberOfClips;
    int64 _pixelsPerClip;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TimelineComponent)
};


#endif  // TIMELINECOMPONENT_H_INCLUDED
