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

class TimelineClip  : public Component
{
public:
    TimelineClip(int64 clipNumber)
    : _clipNumber(clipNumber)
    {
        setName((String) clipNumber);
    }
    
    ~TimelineClip()
    {}
    
    void paint (Graphics& g)
    {
        g.fillAll (Colours::white);   // clear the background
        
        g.setColour (Colours::grey);
           // draw an outline around the component
        g.fillRect(getLocalBounds());
        g.setColour(Colours::black);
        g.drawRect(getLocalBounds());

        g.setColour (Colours::whitesmoke);
        g.setFont (14.0f);
        g.drawText ((String) _clipNumber, getLocalBounds(),
                    Justification::centred, true);   // draw some placeholder text
    }
    
    void resized()
    {}
    
private:
    int64 _clipNumber;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TimelineClip)
};

#endif  // TIMELINECLIP_H_INCLUDED
