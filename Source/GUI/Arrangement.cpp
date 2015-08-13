/*
  ==============================================================================

    Arrangement.cpp
    Created: 10 Aug 2015 10:58:25pm
    Author:  dtl

  ==============================================================================
*/

#include "Arrangement.h"

//==============================================================================
Arrangement::Arrangement(const ApplicationCommandManager &commands)
{
    _cursor = new TimelineCursor(56000.0);
    addAndMakeVisible(_cursor);
}

Arrangement::~Arrangement()
{
}

void Arrangement::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (Colours::white);   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::lightblue);
    g.setFont (14.0f);
    g.drawText ("Arrangement", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void Arrangement::resized()
{
    _cursor->setBounds(0, 0, getParentWidth(), getParentHeight());

}
