/*
  ==============================================================================

    ProjectTab.cpp
    Created: 19 Aug 2015 5:01:13pm
    Author:  Matt

  ==============================================================================
*/

#include "ProjectTab.h"
#include "../Core/ProjectManager.h"

ProjectTab::ProjectTab(ApplicationCommandManager& commands) : _commands(commands)
{
	drawButtons();
}

ProjectTab::~ProjectTab()
{
}

void ProjectTab::drawButtons()
{
	_newProjectButton = new TextButton("New Project", "Create A New Project...");
	_newProjectButton->setCommandToTrigger(&_commands, ProjectManager::newProject, true);

	_saveProjectButton = new TextButton("Save Project", "Save The Current Project...");
	_saveProjectButton->setCommandToTrigger(&_commands, ProjectManager::saveProject, true);

	_saveProjectAsButton = new TextButton("Save Project As", "Specify Where To Save The Project...");
	_saveProjectAsButton->setCommandToTrigger(&_commands, ProjectManager::saveProjectAs, true);

	_openProjectButton = new TextButton("Open Project", "Open A Project...");
	_openProjectButton->setCommandToTrigger(&_commands, ProjectManager::openProject, true);

	addAndMakeVisible(_newProjectButton);
	addAndMakeVisible(_saveProjectButton);
	addAndMakeVisible(_saveProjectAsButton);
	addAndMakeVisible(_openProjectButton);

	_addTrackButton = new TextButton("Add Track", "Add A New track To The Current Project...");
	_addTrackButton->setCommandToTrigger(&_commands, ProjectManager::addTrack, true);

	addAndMakeVisible(_addTrackButton);
}

void ProjectTab::buttonClicked(Button *)
{
}

void ProjectTab::paint(Graphics &g)
{
	//g.setColour(Colours::steelblue);
	//g.drawRect(getLocalBounds(), 2);   // draw an outline around the component

	//g.setColour(Colours::black);
	//g.drawRect(getLocalBounds(), 1);   // draw an outline around the component
}

void ProjectTab::resized()
{
	_newProjectButton->setBounds(10, 10, getWidth() - 20, 20);
	_saveProjectButton->setBounds(10, 31, getWidth() - 20, 20);
	_saveProjectAsButton->setBounds(10, 52, getWidth() - 20, 20);
	_openProjectButton->setBounds(10, 73, getWidth() - 20, 20);

	_addTrackButton->setBounds(10, 115, getWidth() - 20, 20);
}

