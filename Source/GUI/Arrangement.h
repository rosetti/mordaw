/*
  ==============================================================================

    Arrangement.h
    Created: 10 Aug 2015 10:58:25pm
    Author:  dtl

  ==============================================================================
*/

#ifndef ARRANGEMENT_H_INCLUDED
#define ARRANGEMENT_H_INCLUDED

#include "../../JuceLibraryCode/JuceHeader.h"
#include "TimelineCursor.h"

//==============================================================================
/*
*/
class Arrangement    : public Component
{
public:
    Arrangement(const ApplicationCommandManager &commands);
    ~Arrangement();

    void paint (Graphics&);
    void resized();

private:
    ScopedPointer<TimelineCursor> _cursor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Arrangement)
};


#endif  // ARRANGEMENT_H_INCLUDED
