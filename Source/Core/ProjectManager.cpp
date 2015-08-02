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
