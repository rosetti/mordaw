/*
  ==============================================================================

    RegionComponent.cpp
    Created: 13 Aug 2015 12:21:11pm
    Author:  Thomas

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "RegionComponent.h"

//==============================================================================
RegionComponent::RegionComponent(Audio::Region* region, AudioFormatManager& formatManager, const File& file)
    : _region(region),
    _inputSource(file),
    _thumbnail(1024, formatManager, _thumbnailCache),
    _thumbnailCache(5)
{
    _thumbnail.setSource(&_inputSource);
    setOpaque(true);
    setAlwaysOnTop(true);
}

RegionComponent::~RegionComponent()
{
}

void RegionComponent::paint (Graphics& g)
{
    auto bounds = getLocalBounds();
    bounds.setWidth(static_cast<int>(_thumbnail.getTotalLength()));

    g.drawRect(bounds);
    g.setColour(Colours::black);
    g.fillRect(bounds);
    g.setColour(Colours::green);
    _thumbnail.drawChannels(g, bounds.reduced(2), 0.0f, _thumbnail.getTotalLength(), 1.0f);
}

void RegionComponent::resized()
{
}
