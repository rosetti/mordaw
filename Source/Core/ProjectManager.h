/*
  ==============================================================================

    ProjectManager.h
    Created: 2 Aug 2015 3:14:20pm
    Author:  Thomas

  ==============================================================================
*/

#ifndef PROJECTMANAGER_H_INCLUDED
#define PROJECTMANAGER_H_INCLUDED

#include "../../JuceLibraryCode/JuceHeader.h"

class ProjectManager
{
public:
    ProjectManager();
    ~ProjectManager();

    bool perform(const ApplicationCommandTarget::InvocationInfo & info);

    enum Commands {
        newProject = 0x100,
        openProject = 0x101,
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProjectManager)
};



#endif  // PROJECTMANAGER_H_INCLUDED
