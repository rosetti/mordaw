/*
  ==============================================================================

    TimelineClip.h
    Created: 14 Aug 2015 2:20:22pm
    Author:  dtl

  ==============================================================================
*/

#ifndef TIMELINECLIP_H_INCLUDED
#define TIMELINECLIP_H_INCLUDED

#include "../../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/

/* TimelineClip is a subcomponent of TimelineComponent, contains an individual clip */
class TimelineClip  : public Component
{
public:
    // construct a timeline clip with the specified clip number
    TimelineClip(int64 clipNumber)
    : _clipNumber(clipNumber)
    {
        // set the name of the clip
        setName(static_cast<String>(clipNumber));
    }
    
    ~TimelineClip()
    {}
    
    void paint (Graphics& g)
    {
        // clear the background
        g.fillAll (Colours::white);
        g.setColour (Colours::grey);
        // draw an outline around the component
        g.fillRect(getLocalBounds());
        g.setColour(Colours::black);
        g.drawRect(getLocalBounds());

        g.setColour (Colours::whitesmoke);
        g.setFont (14.0f);
        // draw clip number
        g.drawText (static_cast<String>(_clipNumber), getLocalBounds(),
                    Justification::left, true);
        int quarterWidth = getWidth()/4;
        int halfWidth = quarterWidth + quarterWidth;
        int threeQuartWidth = halfWidth + quarterWidth;
        // draw lines at appropriate intervals
        g.drawLine(quarterWidth, getHeight()-1, quarterWidth, getHeight()-3);
        g.drawLine(halfWidth, getHeight()-1, halfWidth, getHeight()-6);
        g.drawLine(threeQuartWidth, getHeight()-1, threeQuartWidth, getHeight()-3);
    }
    
    void resized()
    {}
    
private:
    int64 _clipNumber;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TimelineClip)
};

#endif  // TIMELINECLIP_H_INCLUDED
