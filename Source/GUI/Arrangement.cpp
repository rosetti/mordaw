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
    _addTrackButton = new TextButton("Add a track");
    _addTrackButton->setCommandToTrigger(&commands, ProjectManager::addTrack, true);
    _zoomInButton= new TextButton("+");
    _zoomInButton->addListener(this);
    //addAndMakeVisible(_cursor);
    addAndMakeVisible(_zoomInButton);
    addAndMakeVisible(_addTrackButton);
    addAndMakeVisible(_timeline);
    setPixelsPerClip(30);
}

Arrangement::~Arrangement()
{
    for (auto track : _tracks) {
        delete track;
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

void Arrangement::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (Colours::white);   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::lightblue);
    g.setFont (15.0f);
    g.drawText ("Arrangement", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void Arrangement::resized()
{
    _timeline->setBounds(0,0, getWidth(), 20);
    _cursor->setBounds(_mixerOffset, 0, getParentWidth(), getParentHeight());
    _addTrackButton->setBounds(30, _tracks.size() * 100 + 35, 100, 30);
    _zoomInButton->setBounds(getWidth()/2, getHeight()/2, 20, 20);

    auto i = 0;
    for (auto track : _tracks) {
        track->setBounds(0, (100 * i++) + 20, getWidth(), 100);
    }
}

void Arrangement::setPixelsPerClip(int64 pixels)
{
    _pixelsPerClip = pixels;
    _timeline = new TimelineComponent(100, _pixelsPerClip, _mixerOffset);
    addAndMakeVisible(_timeline);
    for(auto track : _tracks)
    {
        track->setPixelsPerClip(_pixelsPerClip);
    }
    repaint();
    resized();
}

void Arrangement::addTrack(Audio::Track* track) {
    auto trackComponent = new TrackComponent(_commands, track, _tracks.size()+1, _engine.getCurrentSamplerate(), _pixelsPerClip);
    _tracks.add(trackComponent);
    addAndMakeVisible(trackComponent);
    resized();
}