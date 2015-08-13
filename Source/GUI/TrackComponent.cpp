/*
  ==============================================================================

    TrackComponent.cpp
    Created: 12 Aug 2015 10:17:59pm
    Author:  Thomas

  ==============================================================================
*/

#include "TrackComponent.h"

//==============================================================================
TrackComponent::TrackComponent(ApplicationCommandManager &commands, Audio::Track *track) : _track(track), _commands(commands)
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

bool TrackComponent::isInterestedInFileDrag(const StringArray & files)
{
    bool accepted;
    Array<String> extensions;
    extensions.add(".wav");
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

void TrackComponent::filesDropped(const StringArray & files, int x, int y)
{
    for (auto current = files.begin(), end = files.end(); current != end; ++current) {

    }
}