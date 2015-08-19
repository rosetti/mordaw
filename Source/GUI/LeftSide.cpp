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

LeftSide::LeftSide(ApplicationCommandManager &commands) : _commands(commands),
	_tabbedComponent(TabbedButtonBar::Orientation::TabsAtLeft),
	_tsThread("File Tree"),
	_directoryList(nullptr, _tsThread)
{
	//Set Up the File Tree and Directories
	_projectTab = new ProjectTab(_commands);
	_fileTree = new FileTreeComponent(_directoryList);
	_directoryList.setDirectory(File::getSpecialLocation(File::userHomeDirectory), true, true);
	_tsThread.startThread(3);

	//Add Tabs to the component
	addTabs();

	addAndMakeVisible(_tabbedComponent);
}

LeftSide::~LeftSide()
{
}

void LeftSide::addTabs()
{
	_tabbedComponent.addTab("Project", Colours::darkgrey, _projectTab, false);
	_tabbedComponent.addTab("Files", Colours::darkgrey, _fileTree, false);
	_tabbedComponent.addTab("Plugins", Colours::darkgrey, nullptr, false);
}

void LeftSide::paint (Graphics& g)
{

}

void LeftSide::resized()
{
	_tabbedComponent.setBounds(0, 0, getWidth(), getHeight());
}