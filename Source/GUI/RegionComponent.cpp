/*
  ==============================================================================

    RegionComponent.cpp
    Created: 13 Aug 2015 12:21:11pm
    Author:  Thomas

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../Utility/Conversion.h"
#include "RegionComponent.h"

//==============================================================================
RegionComponent::RegionComponent(int64 x, Audio::Region* region, AudioFormatManager& formatManager, const File& file)
    : _region(region),
    _inputSource(file),
    _thumbnail(1024, formatManager, _thumbnailCache),
    _thumbnailCache(100),
    _posX(x)
{
    _thumbnail.setSource(&_inputSource);
    setOpaque(true);
    setAlwaysOnTop(true);
    filename = file.getFileName();
}

RegionComponent::~RegionComponent()
{
    //_thumbnailCache.clear();
    _thumbnail.clear();
}

void RegionComponent::paint (Graphics& g)
{
    g.fillAll(Colours::grey);
    int64 posSamples = pixelsToSamples(_posX, getWidth(), _region->getLengthInSamples());
    int64 posSeconds = samplesToSeconds(posSamples, 44100.f);
    int64 lengthSeconds = samplesToSeconds(_region->getLengthInSamples(), 44100.f);
    
    Rectangle<int> bounds;
    bounds.setHeight(getParentHeight());
    bounds.setWidth(lengthSeconds * 20);
    g.setColour(Colours::black);
    g.fillRect(bounds);
    g.setColour(Colours::green);
    _thumbnail.drawChannels(g, bounds, 0.0f, lengthSeconds, 0.5f);
    g.setColour(Colours::white);
    g.setFont(8.0f);
    g.drawText(filename, bounds, Justification::topLeft);
    repaint();
}

void RegionComponent::resized()
{
    repaint();
}
