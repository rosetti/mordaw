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
#include "ProjectTab.h"

//==============================================================================
/*
*/
class LeftSide    : public Component
{
public:
    LeftSide(const ApplicationCommandManager &manager);
	~LeftSide();

	void addTabs();

    void paint (Graphics&);
    void resized();

private:
    const ApplicationCommandManager &_commands;
		
	TimeSliceThread _tsThread;
	DirectoryContentsList _directoryList;
	
	TabbedComponent _tabbedComponent;
	ScopedPointer<FileTreeComponent> _fileTree;
	ScopedPointer<ProjectTab> _projectTab;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LeftSide)
};


#endif  // LEFTSIDE_H_INCLUDED
