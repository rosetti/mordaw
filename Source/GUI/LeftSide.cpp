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
LeftSide::LeftSide(const ApplicationCommandManager &commands) : _commands(commands),
	tabbedComponent(TabbedButtonBar::Orientation::TabsAtLeft)
	//tsThread("File Tree"),
	//directoryList(nullptr, tsThread),
	//fileTree(directoryList)
{
	/*
	tabbedComponent.addTab("Files",
		Colours::darkgrey,
		&fileTree,
		false);
	*/

	addAndMakeVisible(tabbedComponent);
}

LeftSide::~LeftSide()
{
}

void LeftSide::paint (Graphics& g)
{

}

void LeftSide::resized()
{
	tabbedComponent.setBounds(0, 0, 300, getHeight());
}
