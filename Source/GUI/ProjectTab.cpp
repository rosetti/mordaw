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

	_exportWavButton = new TextButton("Export as Wav", "Export the project as a wav...");
	_exportWavButton->setCommandToTrigger(&_commands, ProjectManager::exportWav, true);

	addAndMakeVisible(_exportWavButton);

	_addTrackButton = new TextButton("Add Track", "Add A New track To The Current Project...");
	_addTrackButton->setCommandToTrigger(&_commands, ProjectManager::addTrack, true);

	addAndMakeVisible(_addTrackButton);
}

void ProjectTab::buttonClicked(Button *)
{
}

void ProjectTab::paint(Graphics &g)
{
	g.setColour(Colours::black);
	g.drawRect(5.0f, 100.0f, (float)getWidth() - 10.0f, 4.0f, 2.0f);

	g.setColour(Colours::steelblue);
	g.drawRect(7.0f, 101.0f, (float)getWidth() - 14.0f, 2.0f, 0.5f);

	g.setColour(Colours::black);
	g.drawRect(5.0f, 145.0f, (float)getWidth() - 10.0f, 4.0f, 2.0f);

	g.setColour(Colours::steelblue);
	g.drawRect(7.0f, 146.0f, (float)getWidth() - 14.0f, 2.0f, 0.5f);

	g.setColour(Colours::steelblue);
	g.drawRect(getLocalBounds(), 2);   // draw an outline around the component

	g.setColour(Colours::black);
	g.drawRect(getLocalBounds(), 1);   // draw an outline around the component
}

void ProjectTab::resized()
{
	_newProjectButton->setBounds(10, 10, getWidth() - 20, 20);
	_saveProjectButton->setBounds(10, 30, getWidth() - 20, 20);
	_saveProjectAsButton->setBounds(10, 50, getWidth() - 20, 20);
	_openProjectButton->setBounds(10, 70, getWidth() - 20, 20);

	_exportWavButton->setBounds(10, 115, getWidth() - 20, 20);

	_addTrackButton->setBounds(10, 160, getWidth() - 20, 20);
}

