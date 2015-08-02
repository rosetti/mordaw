/*
==============================================================================

ProjectManager.cpp
Created: 2 Aug 2015 3:14:20pm
Author:  Thomas

==============================================================================
*/

#include "ProjectManager.h"

ProjectManager::ProjectManager()
{
}

ProjectManager::~ProjectManager()
{
}

void ProjectManager::getAllCommands(Array<CommandID>& commands) const
{
    const CommandID ids[] = {
        ProjectManager::newProject,
        ProjectManager::openProject,
    };

    commands.addArray(ids, numElementsInArray(ids));
}

void ProjectManager::getCommandInfo(CommandID commandID, ApplicationCommandInfo &result) const {
    const String projectManagement("Project Management");
 
    switch (commandID) {
    case ProjectManager::newProject:
        result.setInfo("New Project...", "Create a new project.", projectManagement, 0);
        result.addDefaultKeypress('N', ModifierKeys::commandModifier);
        break;

    case ProjectManager::openProject:
        result.setInfo("Open Project...", "Open an existing project.", projectManagement, 0);
        result.addDefaultKeypress('O', ModifierKeys::commandModifier);
        break;

    default:
        break;
    }
}
bool ProjectManager::perform(const ApplicationCommandTarget::InvocationInfo & info)
{
    switch (info.commandID) {
    case ProjectManager::newProject:
        // Close current project
        // Create new project
        return true;
    case ProjectManager::openProject:
        // Close current project
        // Open existing project
        return true;
    default:
        return false;
    }
}
