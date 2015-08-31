/*
  ==============================================================================

    ProjectManager.h
    Created: 2 Aug 2015 3:14:20pm
    Author:  Matt

  ==============================================================================
*/

#ifndef PROJECTMANAGER_H_INCLUDED
#define PROJECTMANAGER_H_INCLUDED

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../Audio/Engine.h"
#include "../Audio/TrackProcessor.h"
#include "../Audio/Track.h"
#include "../Audio/ChannelStripProcessor.h"
#include "../GUI/MainWindow.h"

#include "../GUI/Arrangement.h"
#include "../GUI/RegionComponent.h"
#include "../GUI/TrackComponent.h"
#include "../GUI/MixerView.h"

using namespace Audio;

class ProjectManager : public Timer
{
public:
    explicit ProjectManager(ApplicationCommandManager &commands, Audio::Engine &engine, MainWindow &window);
    ~ProjectManager();

	void createBasicProjectFramework(const String& projectName);
	void saveCurrentProject(File savedFile);
	void saveCurrentProjectAs();
	void exportProjectAsWav();
	void isProjectExisting();
	void loadExistingProject();
    
    void timerCallback() override;

	void loadTracks();
	void addRegionGUIs(std::vector<Track*> audioTracks_);
	void loadChannelStripSettings();

    void getCommandInfo(CommandID commandID, ApplicationCommandInfo &result) const;
    void getAllCommands(Array<CommandID>& commands) const;
    bool perform(const ApplicationCommandTarget::InvocationInfo & info);

	enum Commands {
		newProject = 0x100,
		openProject = 0x101,
		saveProject = 0x102,
		saveProjectAs = 0x103,
		closeProject = 0x107,
		exportWav = 0x108,
		addTrack = 0x104,
		addRegion = 0x105,
		addRegionToTrack = 0x106,
    };

private:
    ApplicationCommandManager &_commands;
    Audio::Engine &_engine;
    MainWindow &_mainWindow;
	ScopedPointer<XmlElement>projectElements;

	FileChooser _saveChooser;
	FileChooser _loadChooser;
	FileChooser _exportWavChooser;
	File _projectFile;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProjectManager)
};

#endif  // PROJECTMANAGER_H_INCLUDED