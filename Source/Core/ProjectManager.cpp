/*
==============================================================================

ProjectManager.cpp
Created: 2 Aug 2015 3:14:20pm
Author:  Matt

==============================================================================
*/

#include "ProjectManager.h"
#include "../Audio/TrackProcessor.h"
#include "../Audio/Track.h"
#include "../Audio/SampleRegion.h"
#include "../Utility/Conversion.h"

/*
The Project Manager class does what it says on the tin. It manages the project. This includes and may not be limited to
creating new projects, saving, loading, exporting.
*/
ProjectManager::ProjectManager(ApplicationCommandManager &commands, Audio::Engine &engine, MainWindow &window) :
	_engine(engine),
	_commands(commands),
	_mainWindow(window),
	_saveChooser("Save your .mor project...", File::getSpecialLocation
		(File::userHomeDirectory), "*.mor"),
	_loadChooser("Load a .mor project...", File::getSpecialLocation
		(File::userHomeDirectory), "*.mor"),
	_exportWavChooser("Choose a place to save the exported file...", File::getSpecialLocation
		(File::userHomeDirectory), "*.wav"),
	_projectFile()
{
	//Create an XML element to hold details about the project
	projectElements = new XmlElement("Project_Elements");
}

ProjectManager::~ProjectManager()
{
}

/*
This function creates a basic project framework to allow the saving of a project in the form of an XML document
@param projectName The name of the current project. By default this is "Unsaved"
*/
void ProjectManager::createBasicProjectFramework(const String& projectName)
{
	//Clear the current XML project element
	projectElements->deleteAllChildElements();
	//Create the framework for saving project settings/details and add it to projectElements
	XmlElement* projectSettings = new XmlElement("Settings");
	//Set the project name
	projectSettings->setAttribute("Project_Name", projectName);
	//Set the user projects filepath (currently unsaved)
	projectSettings->setAttribute("Project_File_Path", "Unsaved");
	//Add MasterStrip to Project Settings
	//Add the project settings to the main projectElements XML element
	projectElements->addChildElement(projectSettings);
	XmlElement* masterStrip = new XmlElement("Master_Strip");
	projectElements
		->getChildByName("Settings")
		->addChildElement(masterStrip);

	

	//Create the framework for saving project tracks
	XmlElement* projectTracks = new XmlElement("Tracks");
	projectElements->addChildElement(projectTracks);
	//Create the framework for storing track channel strips
	XmlElement* projectStrips = new XmlElement("Strips");
	projectElements->addChildElement(projectStrips);
}

/*
Saves the current project
@param savedFile The file to append the current projects XML Document to
*/
void ProjectManager::saveCurrentProject(File savedFile)
{
	savedFile.deleteFile();
	_projectFile = savedFile;
	createBasicProjectFramework(savedFile.getFileName());
	//Get the File path where the project will be saved
	String stringFile = _projectFile.getFullPathName();
	//Set the project file path
	projectElements
		->getChildByName("Settings")
		->setAttribute("Project_File_Path", stringFile);
	//Get the File name to be used as the project name
	String projectName = _projectFile.getFileNameWithoutExtension();
	//Set the project name
	projectElements
		->getChildByName("Settings")
		->setAttribute("Project_Name", projectName);

	//Retrieve Master Strip
	ChannelStripProcessor* masterStrip_ = _engine.getMixer()->getMasterStrip();
	
	//Retrieve MasterStrip Volume Setting
	float masterVolume_ = masterStrip_->getParameter(1);
	//Store Volume Setting
	projectElements
		->getChildByName("Settings")
		->getChildByName("Master_Strip")
		->setAttribute("Gain", (double)masterVolume_);
	//Retrieve MasterStrip Panning Setting
	float masterPanning_ = masterStrip_->getParameter(2);
	//Store Panning Setting
	projectElements
		->getChildByName("Settings")
		->getChildByName("Master_Strip")
		->setAttribute("Panning", (double)masterPanning_);
	//Retrieve Mute Setting
	bool masterMute_ = masterStrip_->getMuteParameter();
	//Store the Mute Setting
	projectElements
		->getChildByName("Settings")
		->getChildByName("Master_Strip")
		->setAttribute("Mute", masterMute_);

	//Retrieve a map of all the tracks
	std::vector<TrackComponent*>* tracks_ = _mainWindow.Content.getArrangement()->getTrackMap();
	//Create XML elements for each track
	int trackNumber_ = 1;
	for (auto currentTrack : *tracks_)
	{
		//Create an entry for the track and add it to projectTracks
		String trackName_ = "Track_" + (String)trackNumber_;
		XmlElement* track_ = new XmlElement(trackName_);
		projectElements
			->getChildByName("Tracks")->addChildElement(track_);

		//Retrieve a map of all the tracks regions
		std::map<int64, String>* regions_ = currentTrack->getRegionMap();
		//Create XML elements for each region
		int regionNumber_ = 1;
		for (auto currentRegion = regions_->begin(), regionsEnd = regions_->end(); currentRegion != regionsEnd; ++currentRegion)
		{
			String regionName_ = "Region_" + (String)regionNumber_;
			//Create an entry for the region and add it to the respective track entry
			XmlElement* region_ = new XmlElement(regionName_);
			projectElements
				->getChildByName("Tracks")
				->getChildByName(trackName_)
				->addChildElement(region_);
			//Add the regions file path to the respective region entry
			String regionPath_ = currentRegion->second;
			projectElements->getChildByName("Tracks")
				->getChildByName(trackName_)
				->getChildByName(regionName_)
				->setAttribute("File_Path", regionPath_);
			//Add the regions position to the respective region entry
			int regionPosition_ = (int) currentRegion->first;
			projectElements->getChildByName("Tracks")
				->getChildByName(trackName_)
				->getChildByName(regionName_)
				->setAttribute("Region_Position", regionPosition_);
			regionNumber_++;
		}
		//Increase the track number
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
			->getChildByName("Strips")
			->addChildElement(strip_);
		//Add the current mute state
		bool mute_ = currentStrip->getButtonState("Mute");
		projectElements
			->getChildByName("Strips")
			->getChildByName(stripName_)
			->setAttribute("Mute", mute_);
		//Add the current solo state
		bool solo_ = currentStrip->getButtonState("Solo");
		projectElements
			->getChildByName("Strips")
			->getChildByName(stripName_)
			->setAttribute("Solo", (int)solo_);
		//Add the current volume state
		float volume_ = currentStrip->getSliderValue("Gain");
		projectElements
			->getChildByName("Strips")
			->getChildByName(stripName_)
			//->addTextElement((String)volume_);
			->setAttribute("Gain", (double)volume_);
		//Add the current panning state
		float panning_ = currentStrip->getSliderValue("Panning");
		projectElements
			->getChildByName("Strips")
			->getChildByName(stripName_)
			->setAttribute("Panning", (double)panning_);
		//Increase the track number
		trackNumber_++;
	}
	//reset the file
	_projectFile.deleteFile();
	_projectFile.create();
	// Save existing project to the destination
	//Create XML document
	String xmlDoc = projectElements->createDocument(String::empty, false);
	//Append to savefile
	_projectFile.appendText(xmlDoc);
}

/*
Allows the user to choose a save location then runs saveCurrentProject
*/
void ProjectManager::saveCurrentProjectAs()
{
	// Choose destination
	if (_saveChooser.browseForFileToSave(false))
	{
		//Get the file and location that the user chose
		_projectFile = _saveChooser.getResult();

		bool overwrite = true;
		//If the file exists; ask the user if they wish to overwrite it
		if (_projectFile.existsAsFile())
		{
			overwrite = AlertWindow::showOkCancelBox(
				AlertWindow::WarningIcon, "A project by this name already exists!", "Would you like to overwrite this project file?");
		}

		//If the user wishes to overwrite the file; Save
		if (overwrite == true)
		{
			saveCurrentProject(_projectFile);
		}
	}
}

/*
Loads an existing project
*/
void ProjectManager::loadExistingProject()
{
	//Let the user choose a .mor file then begin processing
	if (_loadChooser.browseForFileToOpen()) {
		//Update the project File
		_projectFile = _loadChooser.getResult();
		//Parse the project file into projectElements
		projectElements = XmlDocument::parse(_projectFile);
		loadTracks();
	}
}

/*
Used by loadExistingProject; this function loads the chosen projects tracks from an XML document
*/
void ProjectManager::loadTracks()
{
	//Find out the number of tracks in the file to be loaded
	int numberOfTracks_ = projectElements
		->getChildByName("Tracks")
		->getNumChildElements();
	int currentTrack_ = 1;
	std::vector<Track*> audioTracks_;
	//Loop through and add the tracks
	while (currentTrack_ <= numberOfTracks_)
	{
		//Add the tracks to the mixer
		Audio::Track* track_ = new Audio::Track();
		audioTracks_.push_back(track_);
		_engine.getMixer()->add(track_);
		_mainWindow.Content.addTrack(track_);
		
		currentTrack_++;
	}
	//Add each tracks regions to the UI
	addRegionGUIs(audioTracks_);
	//Load each tracks Channel Strip settings
	loadChannelStripSettings();
}

/*
Used by loadTracks to add each tracks associated regions to the UI
*/
void ProjectManager::addRegionGUIs(std::vector<Track*> audioTracks_)
{
	std::vector<TrackComponent*>* trackComponents_ = _mainWindow.Content.getArrangement()->getTrackMap();
	
	int numberOfTracks_ = projectElements
		->getChildByName("Tracks")
		->getNumChildElements();
	if (numberOfTracks_ > 0)
	{
		//Create XML elements for each track
		int trackNumber_ = 1;
		while (trackNumber_ <= numberOfTracks_)
		{
			//Find out the number of regions the loaded track has
			String trackName_ = "Track_" + (String)trackNumber_;
			int numberOfRegions_ = projectElements
				->getChildByName("Tracks")
				->getChildByName(trackName_)
				->getNumChildElements();
			if (numberOfRegions_ > 0)
			{
				int currentRegion_ = 1;
				//Loop through and add the regions
				while (currentRegion_ <= numberOfRegions_)
				{
					String regionName_ = "Region_" + (String)currentRegion_;
					//Retrieve the file path associated with the region
					String fileString_ = projectElements
						->getChildByName("Tracks")
						->getChildByName(trackName_)
						->getChildByName(regionName_)
						->getStringAttribute("File_Path");
					//Check that the file is in the correct format
					String format_;
					if (fileString_.contains(".wav") || fileString_.contains(".WAV"))
						format_ = "WAV";
					else if (fileString_.contains(".aif") || fileString_.contains(".aiff") || fileString_.contains(".AIF") || fileString_.contains(".AIFF"))
						format_ = "AIFF";
					else if (fileString_.contains(".flac") || fileString_.contains(".FLAC"))
						format_ = "FLAC";
					File currentRegionFile_(fileString_);

					//Create a format manager and register basic formats (WAV, AIF(F), Flac)
					AudioFormatManager formatManager_;
					formatManager_.registerBasicFormats();

					//Create a reader for the Region file
					AudioFormatReader* reader_ = formatManager_.createReaderFor(currentRegionFile_);
					//Create the necessary region
					Audio::Region* region = new Audio::SampleRegion(reader_, 1, &currentRegionFile_);

					//Retrieve the regions position
					int regionPosition_ = projectElements
						->getChildByName("Tracks")
						->getChildByName(trackName_)
						->getChildByName(regionName_)
						->getIntAttribute("Region_Position");
					int64 mixerOffset_ = _mainWindow.Content.getArrangement()->getMixerOffset();
					int64 pixelsPerClip_ = _mainWindow.Content.getArrangement()->getPixelsPerClip();
					double sampleRate_ = _engine.getCurrentSamplerate();

					if (regionPosition_ >= mixerOffset_)
					{
						int64 samplesRange_ = secondsToSamples(100, sampleRate_);
						int64 positionSamples_ = pixelsToSamples(regionPosition_ - mixerOffset_, 100 * pixelsPerClip_, samplesRange_);

						audioTracks_.at(trackNumber_ - 1)->add(positionSamples_, region);
						trackComponents_->at(trackNumber_-1)->createRegionGUI(regionPosition_, region, formatManager_, currentRegionFile_);
					}
					else if (regionPosition_ < mixerOffset_)
					{
						audioTracks_.at(trackNumber_)->add(0, region);
						trackComponents_->at(trackNumber_-1)->createRegionGUI(mixerOffset_, region, formatManager_, currentRegionFile_);
					}
					currentRegion_++;
				}
			}
			trackNumber_++;
		}
	}
}

/*
Used by loadTracks to add each tracks associated
*/
void ProjectManager::loadChannelStripSettings()
{
	//Retrieve the vector of channel strips
	std::vector<ChannelStripComponent*>* strips_ = _mainWindow.Content.getMixer()->getChannelStrips();

	//reset the track number to 1
	int trackNumber_ = 1;
	//Iterate through and apply the settings
	for (auto strip : *strips_)
	{
		//Set the current strip name/number
		String stripName_ = "TrackStrip_" + (String)trackNumber_;

		//Retrieve and set the mute state
		bool mute = projectElements
			->getChildByName("Strips")
			->getChildByName(stripName_)
			->getBoolAttribute("Mute");
		strip->setButtonState("Mute", mute);

		//Retrieve and set the current solo state
		bool solo = projectElements
			->getChildByName("Strips")
			->getChildByName(stripName_)
			->getBoolAttribute("Solo");
		strip->setButtonState("Solo", solo);
		
		//Retrieve and set the current gains state
		double gain = projectElements
			->getChildByName("Strips")
			->getChildByName(stripName_)
			->getDoubleAttribute("Gain");
		strip->setSliderValue("Gain", gain);

		//Retrieve and set the current panning state
		double panning = projectElements
			->getChildByName("Strips")
			->getChildByName(stripName_)
			->getDoubleAttribute("Panning");
		strip->setSliderValue("Panning", panning);
		
		//Increment the track number
		trackNumber_++;
	}
}

/*
A quick checker to see if the project already exists as a file
*/
void ProjectManager::isProjectExisting()
{
	//Retrieve the current projects name
	String _currentName = projectElements->getChildByName("Settings")->getStringAttribute("Project_Name");
	
	//If the project is named "Untitled_Project" we can assume it hasn't been saved
	if (!_currentName.compare("Untitled_Project")) {
		//Proceed to save the project as a new file
		saveCurrentProjectAs();
	}
	//If the project does exist...
	else {
		//Save the current project in the same place
		saveCurrentProject(_projectFile);
	}
}

/*
Exports the current project to a file in a WAV format
*/
void ProjectManager::exportProjectAsWav()
{
	//Allow the user to choose a place to save the project
	if (_exportWavChooser.browseForFileToSave(false))
	{
		//Retrieve the user specified location
		const File exportFile_ = _exportWavChooser.getResult();
		bool overwrite = true;
		//Check if the file still exists
		if (_projectFile.existsAsFile())
		{
			//Ask the user if they wish to overwrite the file
			overwrite = AlertWindow::showOkCancelBox(
				AlertWindow::WarningIcon, "An audio file by this name already exists!", "Would you like to overwrite this file?");
		}

		//If the user confirms they want to overwrite the file (or it doesn't exist)
		if (overwrite == true)
		{

            if(!_engine.getMixer()->isExporting())
            {
				//Start the export
                _engine.getMixer()->startExporting(exportFile_);
                startTimer(1000);
				//Play what is being exported
                _engine.getMixer()->startPlayingAt(0);
            }
		}
	}
}

/*
A function which tells the exporter to stop exporting if export playback has stopped
*/
void ProjectManager::timerCallback()
{

    if(!_engine.getMixer()->isPlaying())
    {
        _engine.getMixer()->stopExporting();
        stopTimer();
    }
}

/*
Retrieves all commands related to the project manager
@param commands An array of the commands
*/
void ProjectManager::getAllCommands(Array<CommandID>& commands) const
{
    const CommandID ids[] = {
        newProject,
        openProject,
        saveProject,
        saveProjectAs,
		closeProject,
		exportWav,
        StandardApplicationCommandIDs::cut,
        StandardApplicationCommandIDs::copy,
        StandardApplicationCommandIDs::paste,
        addTrack,
        addRegion,
    };

    commands.addArray(ids, numElementsInArray(ids));
}

/*
Retrieves information about the commands
@param commandID The ID of the command to be retrieved
@param result Holds information describing a command
*/
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

	case exportWav:
		result.setInfo("Export As Wav", "Export the Project as an Audio File.", projectManagement, 0);
		result.addDefaultKeypress('E', ModifierKeys::ctrlAltCommandModifiers);
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

/*
Carries out a command
@param info Contains contextual information about the invocation of a command
*/
bool ProjectManager::perform(const ApplicationCommandTarget::InvocationInfo & info)
{
	//Set track and trackComponent pointers to nullptr
    Audio::Track *track = nullptr;
    TrackComponent *trackComponent = nullptr;

    switch (info.commandID) {
    case newProject:
		// Close current project
		//TODO CLOSE THE CURRENT PROJECT
		// Create new project
		createBasicProjectFramework("Untitled_Project");
        return true;

    case openProject:
        // Ask for save if project has modifications
        // Close current project
        // Open existing project
		loadExistingProject();
        return true;

    case saveProject:
		isProjectExisting();
		// Save existing project
        return true;

    case saveProjectAs:
		// Choose destination if project is new
		saveCurrentProjectAs();
        return true;

	case closeProject:
		//Ask the User if They Wish To Save
		//Close the Current Project
		return true;

	case exportWav:
		//Ask user where they want to save the file
		exportProjectAsWav();
		//Export the buffer as an audio file
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
		//Update track to contain a new Track
        track = new Audio::Track();
		//Add the track to the mixer
        _engine.getMixer()->add(track);
		//Add the track to the UI
        _mainWindow.Content.addTrack(track);
        return true;

    case addRegion:
		//Unused as of yet
        trackComponent = dynamic_cast<TrackComponent *>(info.originatingComponent);
        return true;

    default:
        return false;
    }
}
