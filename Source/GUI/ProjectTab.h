/*
  ==============================================================================

    ProjectTab.h
    Created: 19 Aug 2015 5:01:13pm
    Author:  Matt

  ==============================================================================
*/

#ifndef PROJECTTAB_H_INCLUDED
#define PROJECTTAB_H_INCLUDED

#include "../../JuceLibraryCode/JuceHeader.h"
//#include "../Core/ProjectManager.h"

class ProjectTab :	public Component,
					public Button::Listener
{
public:
	ProjectTab(const ApplicationCommandManager& commands);
	~ProjectTab();

	void drawButtons();

	virtual void buttonClicked(Button* button);

private:
	ScopedPointer<Button> _newProjectButton, _saveProjectButton, loadProjectButton, closeProjectButton;
	ScopedPointer<Button> _addTrackButton, _removeTrackButton;

	//ApplicationCommandManager &_commands;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProjectTab)
};




#endif  // PROJECTTAB_H_INCLUDED
