/*
  ==============================================================================

    TrackComponent.cpp
    Created: 12 Aug 2015 10:17:59pm
    Author:  Thomas

  ==============================================================================
*/

#include "TrackComponent.h"
#include "../Utility/Conversion.h"
#include "../Audio/SampleRegion.h"
#include "../Core/ProjectManager.h"

TrackMixerComponent::TrackMixerComponent(int trackID, const Audio::Engine& engine, ApplicationCommandManager& commands)
: _trackID(trackID),
_commands(commands),
_engine(engine)
{
    addAndMakeVisible(_trackLabel = new Label("Track " + String (trackID)));
    
    addAndMakeVisible(_muteButton = new ToggleButton("Mute"));
    _muteButton->setColour(TextButton::buttonColourId, Colours::blue);
    _muteButton->addListener(this);
    
    addAndMakeVisible(_soloButton = new ToggleButton("Solo"));
    _soloButton->setColour(TextButton::buttonColourId, Colours::yellow);
    _soloButton->addListener(this);
}

TrackMixerComponent::~TrackMixerComponent()
{
}

void TrackMixerComponent::mouseDrag(const MouseEvent &e)
{
    int x = e.x;
    int y = e.y;
}

void TrackMixerComponent::buttonClicked(Button* button)
{
    
}
void TrackMixerComponent::mouseDown(const MouseEvent &e)
{
    
}

void TrackMixerComponent::buttonStateChanged(Button* button)
{
    if(button == _muteButton)
        _engine.getMixer();
    else if(button == _soloButton)
        _engine.getMixer()->soloTrack(_trackID);
	ModifierKeys modifiers = ModifierKeys::getCurrentModifiersRealtime();

	// check the mod keys ..
	if (modifiers.isPopupMenu() || modifiers.isCtrlDown())
	{
		ScopedPointer<PopupMenu> arrangeMenu_ = new PopupMenu();
		arrangeMenu_->clear();
		arrangeMenu_->addCommandItem(&_commands, ProjectManager::addTrack);
		arrangeMenu_->show();
	}
}
    
void TrackMixerComponent::paint(Graphics &g)
{
    g.setColour(Colours::darkgrey);
    g.fillAll();
}

void TrackMixerComponent::resized()
{
    int buttonSize = 16;
    _muteButton->setBounds(0, buttonSize * 3, getWidth()/2, buttonSize);
    _soloButton->setBounds(0, buttonSize * 2, getWidth()/2, buttonSize);
    _trackLabel->setText("Track " + String (_trackID), NotificationType::dontSendNotification);
    _trackLabel->setBounds(getWidth()/2+20, getParentHeight()/2, getWidth()/2, buttonSize);
}

int TrackMixerComponent::getTrackID()
{
	return _trackID;
}

void TrackMixerComponent::setTrackID(int trackID)
{
	_trackID = trackID;
}

//==============================================================================
TrackComponent::TrackComponent(ApplicationCommandManager& commands, Audio::Track *track, int trackID, const Audio::Engine& engine, int64 pixelsPerClip)
: _track(track),
  _commands(commands),
  _trackID(trackID),
  _engine(engine),
  _sampleRate(_engine.getCurrentSamplerate()),
  _mixerOffset(200),
  _pixelsPerClip(pixelsPerClip)
{
    addAndMakeVisible(_trackMixer = new TrackMixerComponent(_trackID, _engine, _commands));
    _trackMixer->setAlwaysOnTop(true);

}

TrackComponent::~TrackComponent()
{
	removeAllChildren();
}

void TrackComponent::createRegionGUI(int64 posX, Audio::Region* region, AudioFormatManager& formatManager, File& audioFile)
{
    auto regionGUI = new RegionComponent(posX, _sampleRate, region, formatManager, audioFile, _pixelsPerClip);

    _regions.push_back(regionGUI);
    _posX.push_back(posX);
    _sizeSamps.push_back(region->getLengthInSamples());
    addAndMakeVisible(regionGUI);
    resized();
    //setOpaque(true);
}

void TrackComponent::paint (Graphics& g)
{
    g.setColour(Colours::grey);
    g.fillAll();
}

void TrackComponent::resized()
{
    for(size_t current = 0; current < _regions.size(); ++current){
        auto r(getLocalBounds().reduced(4));

        r.setX((int)_posX.at(current));
        int64 lengthSeconds = (int64)samplesToSeconds(_sizeSamps.at(current), (int64)_sampleRate);
        r.setWidth((int)lengthSeconds * _pixelsPerClip);
        r.removeFromBottom(6);
        _regions.at(current)->setBounds(r.removeFromBottom(90));
    }
    _trackMixer->setBounds(0, 0, _mixerOffset, getParentHeight());
    repaint();
}

void TrackComponent::setPixelsPerClip(int64 pixels)
{
    _pixelsPerClip = pixels;
    for(auto region : _regions)
    {
        region->setPixelsPerClip(_pixelsPerClip);
    }
    repaint();
    resized();
}

bool TrackComponent::isInterestedInFileDrag(const StringArray & files)
{
    bool accepted;
    Array<String> extensions;
    extensions.add(".wav");
    extensions.add(".aif");
    extensions.add(".aiff");
    extensions.add(".flac");

    for (auto currentFile = files.begin(), end = files.end(); currentFile != end; ++currentFile) {
        accepted = false;
        for (auto extension = extensions.begin(), endExtensions = extensions.end(); extension != endExtensions; ++extension) {
            if (currentFile->endsWith(*extension)) {
                accepted = true;
                break;
            }
        }
        if (!accepted) {
            return false;
        }
    }
    return true;
}

void TrackComponent::mouseDrag(const MouseEvent &e)
{
    int x = e.x;
    int y = e.y;
}

void TrackComponent::filesDropped(const StringArray & files, int x, int y)
{
    for (auto current = files.begin(), end = files.end(); current != end; ++current)
    {
        const String fileString = *current;
        String format;
        if(fileString.contains(".wav"))
            format = "WAV";
        else if(fileString.contains(".aif") || fileString.contains(".aiff"))
            format = "AIFF";
        else if(fileString.contains(".flac"))
            format = "FLAC";
            File file(fileString);
            AudioFormatManager formatManager;
            formatManager.registerBasicFormats();

            AudioFormatReader* reader = formatManager.createReaderFor(file);
            Audio::Region* region = new Audio::SampleRegion(reader, 1);
        if(x > _mixerOffset)
        {
            // 100 represents the number of seconds
            int64 samplesRange = secondsToSamples(100, _sampleRate);
            // 20 represents the size of a second in pixels - this all needs replacing with dynamically
            // generated values.
            int64 positionSamples = pixelsToSamples(x - _mixerOffset, 100 * _pixelsPerClip, samplesRange);
            
            _track->add(positionSamples, region);
            createRegionGUI(x, region, formatManager, file);
        }
        else if(x < _mixerOffset)
        {
            _track->add(0, region);
            createRegionGUI(_mixerOffset, region, formatManager, file);
        }
    }
}

int TrackComponent::getTrackID()
{
	return _trackID;
}

void TrackComponent::setTrackID(int trackID)
{
	_trackID = trackID;
}

void TrackComponent::mouseDown(const MouseEvent &e) {

	ModifierKeys modifiers = ModifierKeys::getCurrentModifiersRealtime();

	// check the mod keys ..
	if (modifiers.isPopupMenu() || modifiers.isCtrlDown())
	{
		ScopedPointer<PopupMenu> trackMenu_ = new PopupMenu();
		trackMenu_->clear();
		trackMenu_->addItem(1, "Add Region", true);

		if (trackMenu_->show() == 1)
		{
			FileChooser chooser("Select an audio file to add...",
				File::nonexistent,
				"*.wav; *aif; *.flac");
			if (chooser.browseForFileToOpen()) {
				File audioFile(chooser.getResult());
				const String fileString = audioFile.getFullPathName();
				String format;
				if (fileString.contains(".wav"))
					format = "WAV";
				else if (fileString.contains(".aif") || fileString.contains(".aiff"))
					format = "AIFF";
				else if (fileString.contains(".flac"))
					format = "FLAC";
				AudioFormatManager formatManager;
				formatManager.registerBasicFormats();

				AudioFormatReader* reader = formatManager.createReaderFor(audioFile);
				Audio::Region* region = new Audio::SampleRegion(reader, 1);
				Point<int> position = e.getPosition();
				int x = position.getX();
				
				if (x > _mixerOffset)
				{
					// 100 represents the number of seconds
					int64 samplesRange = secondsToSamples(100, _sampleRate);
					// 20 represents the size of a second in pixels - this all needs replacing with dynamically
					// generated values.
					int64 positionSamples = pixelsToSamples(x - _mixerOffset, 100 * _pixelsPerClip, samplesRange);

					_track->add(positionSamples, region);
					createRegionGUI(x, region, formatManager, audioFile);
				}
				else if (x < _mixerOffset)
				{
					_track->add(0, region);
					createRegionGUI(_mixerOffset, region, formatManager, audioFile);
				}
			}
		}
	}
	else
	{
		dragger.startDraggingComponent(getComponentAt(e.x, e.y), e);
	}

}
