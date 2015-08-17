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
RegionComponent::RegionComponent(int64 x, double sampleRate, Audio::Region* region, AudioFormatManager& formatManager, const File& file, int64 pixelsPerClip)
    : _region(region),
    _inputSource(file),
    _thumbnail(1024, formatManager, _thumbnailCache),
    _thumbnailCache(100),
    _posX(x),
    _sampleRate(sampleRate),
    _pixelsPerClip(pixelsPerClip)
{
    _thumbnail.setSource(&_inputSource);
    setOpaque(true);
    setAlwaysOnTop(true);
    _filename = file.getFileName();
}

RegionComponent::~RegionComponent()
{
    //_thumbnailCache.clear();
    _thumbnail.clear();
}

void RegionComponent::paint (Graphics& g)
{
    Rectangle<int> bounds;
    int64 lengthSeconds = samplesToSeconds(_region->getLengthInSamples(), _sampleRate);
    bounds.setHeight(getParentHeight());
    bounds.setWidth(lengthSeconds * _pixelsPerClip);
    g.reduceClipRegion(bounds);
    g.fillAll(Colours::grey);
    //int64 posSamples = pixelsToSamples(_posX, getWidth(), _region->getLengthInSamples());
    //int64 posSeconds = samplesToSeconds(posSamples, 44100.f);
    g.setColour(Colours::black);
    g.fillRect(bounds);
    g.setColour(Colours::green);
    _thumbnail.drawChannels(g, bounds, 0.0f, lengthSeconds, 0.5f);
    g.setColour(Colours::white);
    g.setFont(8.0f);
    g.drawText(_filename, bounds, Justification::topLeft);
    repaint();
}

void RegionComponent::setPixelsPerClip(int64 pixels)
{
    _pixelsPerClip = pixels;
    repaint();
}

void RegionComponent::resized()
{
    repaint();
}