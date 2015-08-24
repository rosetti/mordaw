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
    _zoomInButton= new TextButton("+");
    _zoomInButton->addListener(this);
    //addAndMakeVisible(_cursor);
    addAndMakeVisible(_zoomInButton);
    addAndMakeVisible(_timeline);
    setPixelsPerClip(30);
	_trackNumber = 1;
}

Arrangement::~Arrangement()
{
	/*
    for (auto track : _tracks) {
        delete track;
    }
	*/
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

	ModifierKeys modifiers = ModifierKeys::getCurrentModifiersRealtime();

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
	setSize((100 * (int)_pixelsPerClip) + (int)_mixerOffset, 100 + (_tracks.size() * 100));
    _timeline->setBounds(0,0, (100 * (int)_pixelsPerClip)+ (int)_mixerOffset, 20);
    _cursor->setBounds((int)_mixerOffset, 0, getParentWidth(), getParentHeight());
    _zoomInButton->setBounds(getWidth()/2, getHeight()/2, 20, 20);

    auto i = 0;
	for (auto currentTrack = _tracks.begin(), end = _tracks.end(); currentTrack != end; ++currentTrack) {
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