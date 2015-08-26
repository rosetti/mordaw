/*
  ==============================================================================

    Arrangement.cpp
    Created: 10 Aug 2015 10:58:25pm
    Author:  dtl

  ==============================================================================
*/

#include "Arrangement.h"
#include "../Core/ProjectManager.h"

//==============================================================================
Arrangement::Arrangement(ApplicationCommandManager &commands, const Audio::Engine &engine)
: _engine(engine), _commands(commands), _mixerOffset(200), _pixelsPerClip(30)
{
    _timeline = new TimelineComponent(100, _mixerOffset);
    _cursor = new TimelineCursor(_engine);
    //_zoomInButton= new TextButton("+");
    //_zoomInButton->addListener(this);
    addAndMakeVisible(_cursor);
    addAndMakeVisible(_zoomInButton);
    //addAndMakeVisible(_timeline);
    setPixelsPerClip(30);
	_trackNumber = 1;
}

Arrangement::~Arrangement()
{
    removeAllChildren();
    for (auto track : _tracks) {
        delete track.first;
    }
}

void Arrangement::buttonClicked(Button* button)
{
    if(button == _zoomInButton)
    {
        setPixelsPerClip(_pixelsPerClip += 10);
        repaint();
    }
}

void Arrangement::mouseDown(const MouseEvent &) {

    auto modifiers = ModifierKeys::getCurrentModifiersRealtime();

	// check the mod keys ..
	if (modifiers.isPopupMenu() || modifiers.isCtrlDown())
	{
		ScopedPointer<PopupMenu> arrangeMenu_ = new PopupMenu();
		arrangeMenu_->clear();
		arrangeMenu_->addCommandItem(&_commands, ProjectManager::addTrack);
		arrangeMenu_->addCommandItem(&_commands, MainWindow::showMixer);
		arrangeMenu_->show();
	}
}

void Arrangement::mouseDoubleClick(const MouseEvent &)
{
	//ProjectManager::addTrack;
}

void Arrangement::paint (Graphics& g)
{
	g.setGradientFill(ColourGradient(Colours::darkgrey, 1, 1, Colours::darkorange, 10, 10, true));
	//g.setColour(Colours::darkorange);
	//g.drawRect(0, 0, getWidth(), getParentHeight());
}

void Arrangement::resized()
{
	// go through regions
	//find last region position
	//posX + lengthinseconds
	//total + 30
	
	int64 longestTrackWidth_ = getLongestTrackLength();
	setSize(((100 * (int)_pixelsPerClip) + (int)_mixerOffset) + (int)longestTrackWidth_, 100 + (_tracks.size() * 100));
    _timeline->setBounds(0,0, ((100 * (int)_pixelsPerClip)+ (int)_mixerOffset) + longestTrackWidth_, 20);
    if(_timeline->getNumberOfClips() != (100 * (int)_pixelsPerClip + longestTrackWidth_))
    {
        _timeline->setNumberOfClips(100 * (int)_pixelsPerClip + longestTrackWidth_);
    }

    auto i = 0;
	for (auto currentTrack = _tracks.begin(), end = _tracks.end(); currentTrack != end; ++currentTrack) {
        currentTrack->first->setNumberofClips(100 * _pixelsPerClip + longestTrackWidth_);
        currentTrack->first->setBounds(0, (100 * i++) + 20, getWidth(), 100);
    }
}

void Arrangement::setPixelsPerClip(int64 pixels)
{
    _pixelsPerClip = pixels;
    _timeline = new TimelineComponent(100, _pixelsPerClip, _mixerOffset);
    addAndMakeVisible(_timeline);
	for (auto currentTrack = _tracks.begin(), end = _tracks.end(); currentTrack != end; ++currentTrack) {
        currentTrack->first->setPixelsPerClip(_pixelsPerClip);
    }
    repaint();
    resized();
}

int64 Arrangement::getLongestTrackLength()
{
	int64 longestTrackWidth_ = 0;
	for (auto currentTrack = _tracks.begin(), tracksEnd = _tracks.end(); currentTrack != tracksEnd; ++currentTrack)
	{
		int64 currentTrackLength_ = currentTrack->first->findTrackLength();
		if (currentTrackLength_ > (100 * _pixelsPerClip) + _mixerOffset)
		{
			if (currentTrackLength_ > longestTrackWidth_)
			{
				longestTrackWidth_ = currentTrackLength_;
			}
		}
	}
	return longestTrackWidth_;
}

int64 Arrangement::getPixelsPerClip()
{
	return _pixelsPerClip;
}

int64 Arrangement::getMixerOffset()
{
	return _mixerOffset;
}

std::map<TrackComponent*, int*>* Arrangement::getTrackMap()
{
	return &_tracks;
}

void Arrangement::addTrack(Audio::Track* track) {
    auto trackComponent = new TrackComponent(_commands, track, _tracks.size()+1, _engine, _pixelsPerClip);
	_tracks.insert(std::pair<TrackComponent *, int*>(trackComponent, &_trackNumber));
	_trackNumber++;
    addAndMakeVisible(trackComponent);
    resized();
}

void Arrangement::removeTrack(int trackNumber) {
	for (auto currentTrack = _tracks.begin(), end = _tracks.end(); currentTrack != end; ++currentTrack) {
		if (currentTrack->first->getTrackID() == trackNumber) {

		}
	}
}

void Arrangement::addRegionToTrack(int) {

}