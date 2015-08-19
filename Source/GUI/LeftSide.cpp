/*
  ==============================================================================

    LeftSide.cpp
    Created: 4 Aug 2015 7:58:42pm
    Author:  Thomas

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "LeftSide.h"

//==============================================================================
LeftSide::LeftSide(const ApplicationCommandManager &commands) : _commands(commands)
{
	tabbedComponent = new TabbedComponent(TabbedButtonBar::Orientation::TabsAtLeft);
	addAndMakeVisible(tabbedComponent);
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
}

LeftSide::~LeftSide()
{
}

void LeftSide::paint (Graphics& g)
{

}

void LeftSide::resized()
{
	tabbedComponent->setBounds(0, 0, 300, getHeight());
}
