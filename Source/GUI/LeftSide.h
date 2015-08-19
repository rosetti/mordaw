/*
  ==============================================================================

    LeftSide.h
    Created: 4 Aug 2015 7:58:42pm
    Author:  Thomas

  ==============================================================================
*/

#ifndef LEFTSIDE_H_INCLUDED
#define LEFTSIDE_H_INCLUDED

#include "TransportControls.h"

//==============================================================================
/*
*/
class LeftSide    : public Component
{
public:
    LeftSide(const ApplicationCommandManager &manager);
    ~LeftSide();

    void paint (Graphics&);
    void resized();

private:
    const ApplicationCommandManager &_commands;
	ScopedPointer<TabbedComponent> tabbedComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LeftSide)
};


#endif  // LEFTSIDE_H_INCLUDED
