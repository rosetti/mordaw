/*
==============================================================================

ProjectManager.cpp
Created: 2 Aug 2015 3:14:20pm
Author:  Thomas

==============================================================================
*/

#include "ProjectManager.h"
#include "../Audio/TrackProcessor.h"
#include "../Audio/Track.h"
#include "../Audio/SampleRegion.h"

ProjectManager::ProjectManager(ApplicationCommandManager &commands, Audio::Engine &engine, MainWindow &window) :
	_engine(engine),
	_commands(commands),
	_mainWindow(window),
	_saveChooser("Save your project...", File::getSpecialLocation
		(File::userHomeDirectory), "*.mor"),
	_loadChooser("Load a project...", File::getSpecialLocation
		(File::userHomeDirectory), "*.mor"),
	_projectFile()
{
	projectElements = new XmlElement("Project_Elements");
}

ProjectManager::~ProjectManager()
{
}

void ProjectManager::createBasicProjectFramework(const String& projectName)
{
	//Clear the current XML project element
	projectElements->deleteAllChildElements();
	//Create the framework for saving project settings/details and add it to projectElements
	XmlElement* projectSettings = new XmlElement("Settings");
	projectSettings->setAttribute("Project_Name", projectName);
	projectSettings->setAttribute("Project_File_Path", "Unsaved");
	projectElements->addChildElement(projectSettings);

	//Create the framework for saving project tracks
	XmlElement* projectTracks = new XmlElement("Tracks");
	projectElements->addChildElement(projectTracks);
	//Create the framework for storing track channel strips
	XmlElement* projectStrips = new XmlElement("Strips");
	projectElements->addChildElement(projectStrips);
	//STRIPS AND TRACKS SHOULD BE LINKED
}

void ProjectManager::saveCurrentProject(File savedFile)
{
	_projectFile = savedFile;
	//Retrieve a map of all the tracks
	std::map<TrackComponent*, int*> *tracks_ = _mainWindow.Content.getArrangement()->getTrackMap();
	//Create XML elements for each track
	int trackNumber_ = 1;
	for (auto currentTrack = tracks_->begin(), end = tracks_->end(); currentTrack != end; ++currentTrack)
	{
		//Create an entry for the track and add it to projectTracks
		String trackName_ = "Track_" + (String)trackNumber_;
		XmlElement* track_ = new XmlElement(trackName_);
		projectElements
			->getChildByName("Tracks")->addChildElement(track_);

		//Retrieve a map of all the tracks regions
		std::map<int64*, String *>* regions_ = currentTrack->first->getRegionMap();
		//Create XML elements for each region
		int regionNumber_ = 1;
		for (auto currentRegion = regions_->begin(), end = regions_->end(); currentRegion != end; ++currentRegion)
		{
			String regionName_ = "Region_" + (String)regionNumber_;
			//Create an entry for the region and add it to the respective track entry
			XmlElement* region_ = new XmlElement(regionName_);
			projectElements
				->getChildByName("Tracks")
				->getChildByName(trackName_)->addChildElement(region_);
			//Add the regions file path to the respective region entry
			String *regionPath_ = currentRegion->second;
			projectElements->getChildByName("Tracks")
				->getChildByName(trackName_)
					->getChildByName(regionName_)->setAttribute("File_Path", *regionPath_);
			//Add the regions position to the respective region entry
			int64 regionPosition_ = currentRegion->first;
			projectElements->getChildByName("Tracks")
				->getChildByName(trackName_)
				->getChildByName(regionName_)->setAttribute("Region_Position", regionPosition_);
			regionNumber_++;
		}
		trackNumber_++;
	}
	//Reset the track number for the channel strips
	trackNumber_ = 1;
	//Retrieve a map of all the strips
	std::vector<ChannelStripComponent*> *strips_ = _mainWindow.Content.getMixer()->getChannelStrips();
	//Create XML elements for each strip
	for (auto currentStrip : *strips_) {
		String stripName_ = "TrackStrip_" + (String)trackNumber_;
		XmlElement* strip_ = new XmlElement(stripName_);
		projectElements
			->getChildByName("Strips")->addChildElement(strip_);
		//Add the current mute state
		bool mute_ = currentStrip->getButtonState("mute");
		projectElements
			->getChildByName("Strips")
				->getChildByName(stripName_)->setAttribute("Mute", (int)mute_);
		//Add the current solo state
		bool solo_ = currentStrip->getButtonState("solo");
		projectElements
			->getChildByName("Strips")
				->getChildByName(stripName_)->setAttribute("Solo", (int)solo_);
		//Add the current volume state
		float volume_ = currentStrip->getSliderValue("gain");
		projectElements
			->getChildByName("Strips")
				->getChildByName(stripName_)->setAttribute("Gain", (double)volume_);
		//Add the current panning state
		float panning_ = currentStrip->getSliderValue("panning");
		projectElements
			->getChildByName("Strips")
				->getChildByName(stripName_)->setAttribute("Panning", (double)panning_);
		trackNumber_++;
	}

	//reset the file
	_projectFile.deleteFile();
	_projectFile.create();
	// Save existing project to the destination
	//Create XML
	String xmlDoc = projectElements->createDocument(String::empty, false);
	//Append to savefile
	_projectFile.appendText(xmlDoc);
}

void ProjectManager::projectExisting()
{
	String _currentName = projectElements->getChildByName("Settings")->getStringAttribute("Project_Name");
	if (!_currentName.compare("Untitled_Project")) {
		saveCurrentProjectAs();
	}
	else {
		saveCurrentProject(_projectFile);
	}
}

void ProjectManager::saveCurrentProjectAs()
{
	// Choose destination
	if (_saveChooser.browseForFileToSave(false))
	{
		_projectFile = _saveChooser.getResult();
		//Get the File path where the project will be saved
		String stringFile = _projectFile.getFullPathName();
		projectElements->getChildByName("Settings")->setAttribute("Project_File_Path", stringFile);
		//Get the File name to be used as the project name
		String projectName = _projectFile.getFileNameWithoutExtension();
		projectElements->getChildByName("Settings")->setAttribute("Project_Name", projectName);

		bool overwrite = true;
		if (_projectFile.existsAsFile())
		{
			overwrite = AlertWindow::showOkCancelBox(
				AlertWindow::WarningIcon, "A project by this name already exists!", "Would you like to overwrite this project file?");
		}

		if (overwrite == true)
		{
			saveCurrentProject(_projectFile);
		}
	}
}

void ProjectManager::getAllCommands(Array<CommandID>& commands) const
{
    const CommandID ids[] = {
        newProject,
        openProject,
        saveProject,
        saveProjectAs,
		closeProject,
		exportAudio,
        StandardApplicationCommandIDs::cut,
        StandardApplicationCommandIDs::copy,
        StandardApplicationCommandIDs::paste,
        addTrack,
        addRegion,
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

	case closeProject:
		result.setInfo("Close Project", "Close the Current Project.", projectManagement, 0);
		result.addDefaultKeypress('C', ModifierKeys::ctrlAltCommandModifiers);
		break;

	case exportAudio:
		result.setInfo("Export Audio", "Export the Project as an Audio File.", projectManagement, 0);
		result.addDefaultKeypress('E', ModifierKeys::ctrlAltCommandModifiers);

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

    case addRegion:
        result.setInfo("Add region", "Add a region to the selected track.", projectManagement, 0);
		break;

	case addRegionToTrack:
		result.setInfo("Add region to track", "Add region to a specific track", projectManagement, 0);
		break;

    default:
        break;
    }
}
bool ProjectManager::perform(const ApplicationCommandTarget::InvocationInfo & info)
{
    Audio::Track *track = nullptr;
    TrackComponent *trackComponent = nullptr;

    switch (info.commandID) {
    case newProject:
		createBasicProjectFramework("Untitled_Project");
        // Close current project
        // Create new project
        return true;

    case openProject:
        // Ask for save if project has modifications
        // Close current project
        // Open existing project
        return true;

    case saveProject:
		projectExisting();
        // Choose destination if project is new
		// Save existing project
        return true;

    case saveProjectAs:
		saveCurrentProjectAs();
        return true;

	case closeProject:
		//Ask the User if They Wish To Save
		//Close the Current Project
		return true;

	case exportAudio:
		//Ask user where they want to save the file
		//Export the buffer as an audio file


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

    case addRegion:
        trackComponent = dynamic_cast<TrackComponent *>(info.originatingComponent);
        return true;

    default:
        return false;
    }
}
