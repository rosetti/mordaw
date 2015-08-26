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

void TrackMixerComponent::mouseDrag(const MouseEvent &)
{
    //int x = e.x;
    //int y = e.y;
}

void TrackMixerComponent::buttonClicked(Button*)
{
    
}
void TrackMixerComponent::mouseDown(const MouseEvent &)
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
  _pixelsPerClip(pixelsPerClip),
  _numberOfClips(100)
{
    addAndMakeVisible(_trackMixer = new TrackMixerComponent(_trackID, _engine, _commands));
    _trackMixer->setAlwaysOnTop(true);
	_trackLength = getParentWidth() - (int)_mixerOffset;

}

TrackComponent::~TrackComponent()
{
	removeAllChildren();
    for (auto region : _regionComponents) {
        delete region;
    }
    _regionComponents.clear();
	
}

void TrackComponent::createRegionGUI(int64 posX, Audio::Region* region, AudioFormatManager& formatManager, File& audioFile)
{
    auto regionGUI = new RegionComponent(posX, _sampleRate, region, formatManager, audioFile, _pixelsPerClip);

    _posX.push_back(posX);
    _regionComponents.push_back(regionGUI);
    _sizeSamps.push_back(region->getLengthInSamples());
    String filePath = audioFile.getFullPathName();
    _regions.insert(std::pair<int64, String>(posX, filePath));
    addAndMakeVisible(regionGUI);
    resized();
}

void TrackComponent::paint (Graphics& g)
{
    g.setColour(Colours::grey);
    g.fillAll();
}

int64 TrackComponent::findTrackLength()
{
	int64 lastRegionPosition_ = 0;
	for (auto currentRegion : _regionComponents)
	{
		int64 currentRegionPosition_ = currentRegion->getPositionX();
			if (currentRegionPosition_ > lastRegionPosition_)
			{
				lastRegionPosition_ = currentRegionPosition_;
				_trackLength = currentRegionPosition_ + currentRegion->getRegionWidth();
			}
	}
	return _trackLength;
}

void TrackComponent::resized()
{	
    for(size_t current = 0; current < _regions.size(); ++current){
        auto r(getLocalBounds().reduced(4));

        r.setX((int)_posX.at(current));
        int64 lengthSeconds = samplesToSeconds(_sizeSamps.at(current), _sampleRate);
        r.setWidth((int)lengthSeconds * (int)_pixelsPerClip);
        r.removeFromBottom(6);
        _regionComponents.at(current)->setBounds(r.removeFromBottom(90));
    }
    _trackMixer->setBounds(0, 0, (int)_mixerOffset, getParentHeight());
    repaint();
}

void TrackComponent::setPixelsPerClip(int64 pixels)
{
    _pixelsPerClip = pixels;
    for(auto region : _regionComponents)
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

void TrackComponent::setNumberofClips(int64 clips)
{
    _numberOfClips = clips;
}

void TrackComponent::mouseDrag(const MouseEvent &e)
{
    if(_regionComponents.size() != 0)
    {
        for (std::size_t i = 0; i != _regionComponents.size(); ++i)
        {
            _regionComponents.at(i)->toFront(true);
            if(getComponentAt(e.x, e.y ) == _regionComponents.at(i))
            {
                MouseEvent ev = e.getEventRelativeTo(this);
                int distance = ev.getDistanceFromDragStartX();
                Rectangle<int> r = _regionComponents.at(i)->getBounds();
                if(ev.x > _mixerOffset)
                {
                    int newPos = r.getX() + distance;
                    int newEnd = r.getX() + distance + r.getWidth();
                    _posX.at(i) = newPos;
                    resized();
                    
                    Region* region = _regionComponents.at(i)->getRegion();
                    region->setNextReadPosition(0);
                    int64 samplesRange = secondsToSamples(_numberOfClips, _sampleRate);
                    int64 positionSamples = pixelsToSamples(newPos - _mixerOffset, _numberOfClips * _pixelsPerClip, samplesRange);
                    int64 widthInSamples = pixelsToSamples(newEnd - _mixerOffset, _numberOfClips * _pixelsPerClip, samplesRange);
                    _track->setTotalLength(widthInSamples);
                    _track->move(region, positionSamples);

                    if(_posX.at(i) < _mixerOffset)
                    {
                        _posX.at(i) = getX() + _mixerOffset ;
                        _track->move(region, 0);
                        resized();
                    }
                }
            }
        }
    }
}


void TrackComponent::filesDropped(const StringArray & files, int x, int)
{
    for (auto current = files.begin(), end = files.end(); current != end; ++current)
    {
        const String fileString = *current;
        String format;
        if(fileString.contains(".wav") || fileString.contains(".WAV"))
            format = "WAV";
        else if(fileString.contains(".aif") || fileString.contains(".aiff") || fileString.contains(".AIF") || fileString.contains(".AIFF"))
            format = "AIFF";
        else if(fileString.contains(".flac") || fileString.contains(".FLAC"))
            format = "FLAC";
            File file(fileString);
            AudioFormatManager formatManager;
            formatManager.registerBasicFormats();

            AudioFormatReader* reader = formatManager.createReaderFor(file);
            Region* region = new SampleRegion(reader, 1, &file);
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

int64 TrackComponent::getMixerOffset()
{
	return _mixerOffset;
}

std::map<int64, String>* TrackComponent::getRegionMap()
{
	return &_regions;
}

void TrackComponent::mouseDown(const MouseEvent &e) {

	ModifierKeys modifiers = ModifierKeys::getCurrentModifiersRealtime();

	// check the mod keys ..
	if (modifiers.isPopupMenu() || modifiers.isCtrlDown())
	{
		ScopedPointer<PopupMenu> trackMenu_ = new PopupMenu();
		trackMenu_->clear();
		trackMenu_->addItem(1, "Add Region", true);
        MouseEvent ev = e.getEventRelativeTo(this);
        for(auto region : _regionComponents)
        {
            Rectangle<int> bounds = region->getBounds();
            if(bounds.contains(ev.x + _mixerOffset, ev.y))
            {
                trackMenu_->addItem(2, "Remove Region", true);
            }
        }
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
				Audio::Region* region = new Audio::SampleRegion(reader, 1, &audioFile);
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
					getParentComponent()->resized();
				}
				else if (x < _mixerOffset)
				{
					_track->add(0, region);
					createRegionGUI(_mixerOffset, region, formatManager, audioFile);
					getParentComponent()->resized();
				}
			}
		}
        else if (trackMenu_->show() == 2)
        {
            for(auto i = 0; i < _regionComponents.size(); ++i)
            {
                MouseEvent ev = e.getEventRelativeTo(this);
                Rectangle<int> bounds = _regionComponents.at(i)->getBounds();
                if(bounds.contains(e.x + _mixerOffset, e.y))
                {
                    removeChildComponent(_regionComponents.at(i));
                    
                    Region* region = _regionComponents.at(i)->getRegion();
                    _track->remove(region, _posX.at(i));
                    std::vector<RegionComponent*>::iterator regit = _regionComponents.begin() + i;
                    RegionComponent* component = _regionComponents.at(i);
                    _regionComponents.erase(regit);
                    delete component;
                    _regions.erase(_posX.at(i));
                    std::vector<int64>::iterator posit = _posX.begin() + i;;
                    _posX.erase(posit);
                    _sizeSamps.erase(posit);
                }
            }
        }
	}
}
