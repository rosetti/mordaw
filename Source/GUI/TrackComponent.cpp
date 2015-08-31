/*
  ==============================================================================

    TrackComponent.cpp
    Created: 12 Aug 2015 10:17:59pm
    Author:	TrackMixerComponent	: Dan 
			TrackComponent		: Matt

  ==============================================================================
*/

#include "TrackComponent.h"
#include "../Utility/Conversion.h"
#include "../Audio/SampleRegion.h"
#include "../Core/ProjectManager.h"

/* TrackMixerComponent is a sub component TrackComponent which holds mute and solo controls and the track label */
// construct a track mixer component
TrackMixerComponent::TrackMixerComponent(int trackID, const Audio::Engine& engine, ApplicationCommandManager& commands)
: _trackID(trackID), _commands(commands), _engine(engine)
{
    // create and add track label
    addAndMakeVisible(_trackLabel = new Label("Track " + String (trackID)));
    
    // create and add mute button
    addAndMakeVisible(_muteButton = new ToggleButton("Mute"));
    _muteButton->setColour(TextButton::buttonColourId, Colours::blue);
    _muteButton->addListener(this);
    
    // create and add solo button
    addAndMakeVisible(_soloButton = new ToggleButton("Solo"));
    _soloButton->setColour(TextButton::buttonColourId, Colours::yellow);
    _soloButton->addListener(this);
}

TrackMixerComponent::~TrackMixerComponent()
{
}

// inherited from ButtonListener; not required
void TrackMixerComponent::buttonClicked(Button*)
{
}

void TrackMixerComponent::buttonStateChanged(Button* button)
{
    // if button is clicked mute or solo track
    if(button == _muteButton)
        _engine.getMixer();
    else if(button == _soloButton)
        _engine.getMixer()->soloTrack(_trackID);
	ModifierKeys modifiers = ModifierKeys::getCurrentModifiersRealtime();

	// menu for adding tracks
	if (modifiers.isPopupMenu() || modifiers.isCtrlDown())
	{
		ScopedPointer<PopupMenu> arrangeMenu_ = new PopupMenu();
		arrangeMenu_->clear();
		arrangeMenu_->addCommandItem(&_commands, ProjectManager::addTrack);
		arrangeMenu_->show();
	}
}

// paint the background grey
void TrackMixerComponent::paint(Graphics &g)
{
    g.setColour(Colours::darkgrey);
    g.fillAll();

	//g.setColour(Colours::steelblue);
	//g.drawRect(getLocalBounds(), 2);   // draw an outline around the component

	//g.setColour(Colours::black);
	//g.drawRect(getLocalBounds(), 1);   // draw an outline around the component
}

// set TrackMixerComponent bounds and set track label
void TrackMixerComponent::resized()
{
    int buttonSize = 16;
    _muteButton->setBounds(0, buttonSize * 3, getWidth()/2, buttonSize);
    _soloButton->setBounds(0, buttonSize * 2, getWidth()/2, buttonSize);
    _trackLabel->setText("Track " + String (_trackID), NotificationType::dontSendNotification);
    _trackLabel->setBounds(getWidth()/2+20, getParentHeight()/2, getWidth()/2, buttonSize);
}

// return the track id
int TrackMixerComponent::getTrackID()
{
	return _trackID;
}

// set the track id
void TrackMixerComponent::setTrackID(int trackID)
{
	_trackID = trackID;
}

/* The track component can contain a number of region components, regions can be added using drag and drop or with a menu */
TrackComponent::TrackComponent(ApplicationCommandManager& commands, Audio::Track *track, int trackID, const Audio::Engine& engine, int64 pixelsPerClip)
: _track(track), _engine(engine), _trackID(trackID), _commands(commands), _sampleRate(engine.getCurrentSamplerate()), _mixerOffset(200), _pixelsPerClip(pixelsPerClip), _numberOfClips(100)
{
    // add and make visible the TrackMixerComponent
    addAndMakeVisible(_trackMixer = new TrackMixerComponent(_trackID, _engine, _commands));
    _trackMixer->setAlwaysOnTop(true);
    // set the track length to the arrange width - the mixer offset in px
	_trackLength = getParentWidth() - static_cast<int>(_mixerOffset);

}

TrackComponent::~TrackComponent()
{
    // remove all children, delete region and clear the region vector
	removeAllChildren();
    for (auto region : _regionComponents) {
        delete region;
    }
    _regionComponents.clear();
	
}

// create a region from a file at a specified position in pixels
void TrackComponent::createRegionGUI(int64 posX, Audio::Region* region, AudioFormatManager& formatManager, File& audioFile)
{
    // create a RegionComponent
    auto regionGUI = new RegionComponent(posX, _sampleRate, region, formatManager, audioFile, _pixelsPerClip);

    // add the region parameter to its container, the same index can be used to access each of these
    _posX.push_back(posX);
    _regionComponents.push_back(regionGUI);
    _sizeSamps.push_back(region->getLengthInSamples());
    String filePath = audioFile.getFullPathName();
    _regions.insert(std::pair<int64, String>(posX, filePath));
    // add the region to the UI
    addAndMakeVisible(regionGUI);
    resized();
}

void TrackComponent::paint (Graphics& g)
{
    // set the background to grey
    g.setColour(Colours::grey);
    g.fillAll();

	g.setColour(Colours::steelblue);
	g.drawRect(getLocalBounds(), 2);   // draw an outline around the component

	g.setColour(Colours::black);
	g.drawRect(getLocalBounds(), 1);   // draw an outline around the component
}

// get the current track length
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
        // update the bounds of the regions to the values in the _posX container
        for(size_t current = 0; current < _regions.size(); ++current)
        {
            auto r(getLocalBounds().reduced(4));
            r.setX((int)_posX.at(current));
            int64 lengthSeconds = samplesToSeconds(_sizeSamps.at(current), _sampleRate);
            r.setWidth((int)lengthSeconds * (int)_pixelsPerClip);
            r.removeFromBottom(6);
            _regionComponents.at(current)->setBounds(r.removeFromBottom(90));
        }
    // set the track mixer bounds
    _trackMixer->setBounds(0, 0, (int)_mixerOffset, getParentHeight());
    repaint();
}

// set the pixels per clip for each region
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

// implements FileDragAndDropTarget
bool TrackComponent::isInterestedInFileDrag(const StringArray & files)
{
    bool accepted;
    // file types accepted by target
    Array<String> extensions;
    extensions.add(".wav");
    extensions.add(".aif");
    extensions.add(".aiff");
    extensions.add(".flac");

    // checks the string array for the file extensions, return true if accepted and false if not
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
            int64 samplesRange = secondsToSamples((double)_numberOfClips, _sampleRate);
            int64 positionSamples = pixelsToSamples(x - _mixerOffset, _numberOfClips * _pixelsPerClip, samplesRange);
            
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

void TrackComponent::setNumberofClips(int64 clips)
{
    _numberOfClips = clips;
}

void TrackComponent::mouseDrag(const MouseEvent &e)
{
    int posX;
    if(_regionComponents.size() != 0)
    {
        MouseEvent ev = e.getEventRelativeTo(this);
        posX = ev.x;
        for (std::size_t i = 0; i != _regionComponents.size(); ++i)
        {
            _regionComponents.at(i)->toFront(true);
            
            int distance = ev.getDistanceFromDragStartX();
            Rectangle<int> r = _regionComponents.at(i)->getBounds();
            if(_regionComponents.at(i)->getPositionX() < posX && posX < (_regionComponents.at(i)->getPositionX() + _regionComponents.at(i)->getRegionWidth()))
            {
                int newPos = r.getX() + distance;
                int newEnd = r.getX() + distance + r.getWidth();
                _posX.at(i) = newPos;
                _regionComponents.at(i)->setBounds(newPos, 0, newEnd - newPos, getHeight());
                resized();
                
                int64 samplesRange = secondsToSamples(static_cast<double>(_numberOfClips), _sampleRate);
                int64 positionSamples = pixelsToSamples(newPos - _mixerOffset, _numberOfClips * _pixelsPerClip, samplesRange);
                int64 widthInSamples = pixelsToSamples(newEnd - _mixerOffset, _numberOfClips * _pixelsPerClip, samplesRange);
                _track->setTotalLength(widthInSamples);
                _track->move(_regionComponents.at(i)->getRegion(), positionSamples);
                
                if(_posX.at(i) < _mixerOffset)
                {
                    _posX.at(i) = getX() + _mixerOffset ;
                    _track->move(_regionComponents.at(i)->getRegion(), 0);
                    resized();
                }
            }
            else if((getComponentAt(e.x, e.y ) == _regionComponents.at(i)))
            {
                if(ev.x > _mixerOffset)
                {
                    int newPos = r.getX() + distance;
                    int newEnd = r.getX() + distance + r.getWidth();
                    _posX.at(i) = newPos;
                    resized();
                    Region* region = _regionComponents.at(i)->getRegion();
                    region->setNextReadPosition(0);
                    int64 samplesRange = secondsToSamples((double)_numberOfClips, _sampleRate);
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

    int posX;
	// check the mod keys ..
	if (modifiers.isPopupMenu() || modifiers.isCtrlDown())
	{
		ScopedPointer<PopupMenu> trackMenu_ = new PopupMenu();
		trackMenu_->clear();
		trackMenu_->addCommandItem(&_commands, MainWindow::showMixer);
		trackMenu_->addItem(1, "Add Region", true);
        MouseEvent ev = e.getEventRelativeTo(this);
        for(auto region : _regionComponents)
        {
            posX = ev.x;
            region->setBroughtToFrontOnMouseClick(true);
            if(region->getPositionX() < posX && posX < (region->getPositionX() + region->getRegionWidth()))
            {
                trackMenu_->addItem(2, "Remove Region", true);
            }
        }
        switch (trackMenu_->show())
        {
            case 1:
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
                        int64 samplesRange = secondsToSamples(100, _sampleRate);
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
                break;
            case 2:
            {
                CriticalSection critical;
                critical.enter();
                for(size_t i = 0; i < _regionComponents.size(); ++i)
                {
                    
                    Rectangle<int> bounds_ = _regionComponents.at(i)->getBounds();
					posX = ev.x;
                    if((int)_regionComponents.at(i)->getPositionX() < posX && posX < ((int)_regionComponents.at(i)->getPositionX() + (int)_regionComponents.at(i)->getRegionWidth()))
                    {
                        _track->remove(_regionComponents.at(i)->getRegion(), _posX.at(i));
                        std::vector<RegionComponent*>::iterator regit = _regionComponents.begin() + i;
                        RegionComponent* component = _regionComponents.at(i);
                        removeChildComponent(_regionComponents.at(i));
                        _regionComponents.erase(regit);
                        delete component;
                        _regions.erase(_posX.at(i));
                        std::vector<int64>::iterator posit = _posX.begin() + i;;
                        _posX.erase(posit);
                        std::vector<int64>::iterator sampsit = _sizeSamps.begin() + i;;
                        _sizeSamps.erase(sampsit);
                    }
                }
                critical.exit();
            }
            default:
                break;
        }
    }
}
		