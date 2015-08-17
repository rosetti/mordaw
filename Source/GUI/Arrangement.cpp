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
: _engine(engine), _commands(commands), _mixerOffset(200), _timelineOffset(20)
{
    _timeline = new TimelineComponent(100, _mixerOffset);
    _cursor = new TimelineCursor(_engine);
    _addTrackButton = new TextButton("Add a track");
    _addTrackButton->setCommandToTrigger(&commands, ProjectManager::addTrack, true);
    //addAndMakeVisible(_cursor);
    addAndMakeVisible(_addTrackButton);
    addAndMakeVisible(_timeline);
}

Arrangement::~Arrangement()
{
    for (auto track : _tracks) {
        delete track;
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
    _timeline->setBounds(0,0, getWidth(), _timelineOffset);
    _cursor->setBounds(_mixerOffset, 0, getParentWidth(), getParentHeight());
    _addTrackButton->setBounds(30, _tracks.size() * 100 + 35, 100, 30);

    auto i = 0;
    for (auto track : _tracks) {
        track->setBounds(0, (100 * i++) + _timelineOffset, getWidth(), 100);
    }
}

void Arrangement::addTrack(Audio::Track* track) {
    auto trackComponent = new TrackComponent(_commands, track, _tracks.size()+1);
    _tracks.add(trackComponent);
    addAndMakeVisible(trackComponent);
    resized();
}