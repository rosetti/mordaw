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

void ProjectTab::paint(Graphics &)
{
}

void ProjectTab::resized()
{
	_newProjectButton->setBounds(0, 0, getWidth(), 20);
	_saveProjectButton->setBounds(0, 20, getWidth(), 20);
	_saveProjectAsButton->setBounds(0, 40, getWidth(), 20);
	_openProjectButton->setBounds(0, 60, getWidth(), 20);

	_addTrackButton->setBounds(0, 100, getWidth(), 20);
}

