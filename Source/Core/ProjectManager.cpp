/*
==============================================================================

ProjectManager.cpp
Created: 2 Aug 2015 3:14:20pm
Author:  Thomas

==============================================================================
*/

#include "ProjectManager.h"

ProjectManager::ProjectManager(ApplicationCommandManager &commands, Audio::Engine &engine, MainWindow &window) : 
    _engine(engine), 
    _commands(commands),
    _mainWindow(window)
{
}

ProjectManager::~ProjectManager()
{
}

void ProjectManager::getAllCommands(Array<CommandID>& commands) const
{
    const CommandID ids[] = {
        newProject,
        openProject,
        saveProject,
        saveProjectAs,
        StandardApplicationCommandIDs::cut,
        StandardApplicationCommandIDs::copy,
        StandardApplicationCommandIDs::paste,
        addTrack,
    };

    commands.addArray(ids, numElementsInArray(ids));
}

void ProjectManager::getCommandInfo(CommandID commandID, ApplicationCommandInfo &result) const {
    const String projectManagement("Project Management");
    const String selection("Selection");
 
    switch (commandID) {
    case newProject:
        result.setInfo("New Project...", "Create a new project.", projectManagement, 0);
        result.addDefaultKeypress('N', ModifierKeys::commandModifier);
        break;

    case openProject:
        result.setInfo("Open Project...", "Open an existing project.", projectManagement, 0);
        result.addDefaultKeypress('O', ModifierKeys::commandModifier);
        break;

    case saveProject:
        result.setInfo("Save Project", "Save the current project.", projectManagement, 0);
        result.addDefaultKeypress('S', ModifierKeys::commandModifier);
        break;

    case saveProjectAs:
        result.setInfo("Save Project As...", "Save the current project as a new project.", projectManagement, 0);
        result.addDefaultKeypress('S', ModifierKeys::ctrlAltCommandModifiers);
        break;

    case StandardApplicationCommandIDs::cut:
        // Need to check whether isDisabled should be passed or not, regarding the selection
        result.setInfo("Cut", "Cut the selection.", selection, ApplicationCommandInfo::isDisabled);
        result.addDefaultKeypress('X', ModifierKeys::commandModifier);
        break;

    case StandardApplicationCommandIDs::copy:
        // Need to check whether isDisabled should be passed or not, regarding the selection
        result.setInfo("Copy", "Copy the selection.", selection, ApplicationCommandInfo::isDisabled);
        result.addDefaultKeypress('C', ModifierKeys::commandModifier);
        break;

    case StandardApplicationCommandIDs::paste:
        // Need to check whether isDisabled should be passed or not, regarding the selection
        result.setInfo("Paste", "Paste the previously copied selection.", selection, ApplicationCommandInfo::isDisabled);
        result.addDefaultKeypress('V', ModifierKeys::commandModifier);
        break;

    case addTrack:
        result.setInfo("Add track", "Add a track to the project.", projectManagement, 0);
        result.addDefaultKeypress('+', 0);
        break;

    default:
        break;
    }
}
bool ProjectManager::perform(const ApplicationCommandTarget::InvocationInfo & info)
{
    Audio::Track *track = nullptr;

    switch (info.commandID) {
    case newProject:
        // Close current project
        // Create new project
        return true;

    case openProject:
        // Ask for save if project has modifications
        // Close current project
        // Open existing project
        return true;

    case saveProject:
        // Choose destination if project is new
        // Save existing project
        return true;

    case saveProjectAs:
        // Choose destination
        // Save existing project to the destination
        return true;

    case StandardApplicationCommandIDs::cut:
        // Call copy()
        // Delete the selection
        return true;

    case StandardApplicationCommandIDs::copy:
        // Verify that something is selected
        // Put a copy of the selection in the clipboard
        return true;

    case StandardApplicationCommandIDs::paste:
        // Verify that something is in the clipboard
        // Verify that that thing can be pasted where the user wants to
        // Paste it, or do nothing
        return true;

    case addTrack:
        track = new Audio::Track();
        _engine.getMixer()->add(track);
        _mainWindow.Content.addTrack(track);
        return true;

    default:
        return false;
    }
}
