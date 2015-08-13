/*
  ==============================================================================

    TrackComponent.cpp
    Created: 12 Aug 2015 10:17:59pm
    Author:  Thomas

  ==============================================================================
*/

#include "TrackComponent.h"

//==============================================================================
TrackComponent::TrackComponent(Audio::Track *track) : _track(track)
{
}

TrackComponent::~TrackComponent()
{
}

void TrackComponent::createRegionGUI(Audio::Region* region, AudioFormatManager& formatManager, File& audioFile)
{
    auto regionGUI = new RegionComponent(region, formatManager, audioFile);

    _regions.push_back(regionGUI);
    addAndMakeVisible(regionGUI);
    resized();
    setOpaque(true);
}

void TrackComponent::paint (Graphics& g)
{
    g.setColour(Colours::grey);
    g.fillAll();
}

void TrackComponent::resized()
{
    for (auto current = _regions.begin(), end = _regions.end(); current != end; ++current) {
        auto r(getLocalBounds().reduced(4));

        r.removeFromBottom(6);
        (*current)->setBounds(r.removeFromBottom(140));
    }
}
