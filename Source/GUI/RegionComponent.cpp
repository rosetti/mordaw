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
    : _sampleRate(sampleRate),
    _pixelsPerClip(pixelsPerClip),
    _posX(x),
    _region(region),
    _thumbnailCache(new AudioThumbnailCache(100))

{
	_inputSource = new FileInputSource(file),
	_thumbnail = new AudioThumbnail(1024, formatManager, *_thumbnailCache);
    _thumbnail->setSource(_inputSource);
    setOpaque(true);
    setAlwaysOnTop(true);
    _filename = file.getFileName();
	_lengthSeconds = static_cast<int64>(samplesToSeconds(_region->getLengthInSamples(), _sampleRate));
}

RegionComponent::~RegionComponent()
{
    removeAllChildren();
}

void RegionComponent::paint (Graphics& g)
{
    g.fillAll(Colours::steelblue);
    Rectangle<int> bounds_;
    //bounds_.setHeight(getParentHeight());
    //bounds_.setWidth((int)_lengthSeconds * (int)_pixelsPerClip);
    auto lengthSeconds = static_cast<int64>(samplesToSeconds(_region->getLengthInSamples(), _sampleRate));
    bounds_.setHeight(getParentHeight()-20);
    bounds_.setWidth(static_cast<int>(lengthSeconds) * static_cast<int>(_pixelsPerClip));
    g.reduceClipRegion(bounds_);
    g.fillAll(Colours::grey);
    //int64 posSamples = pixelsToSamples(_posX, getWidth(), _region->getLengthInSamples());
    //int64 posSeconds = samplesToSeconds(posSamples, 44100.f);
    g.setColour(Colours::black);
    g.fillRect(bounds_);
    g.setColour(Colours::green);
    _thumbnail->drawChannels(g, bounds_, 0.0f, static_cast<int>(_lengthSeconds), 0.5f);
    g.setColour(Colours::white);
    g.setFont(8.0f);
    g.drawText(_filename, bounds_, Justification::topLeft);
    resized();
}


void RegionComponent::setPixelsPerClip(int64 pixels)
{
    _pixelsPerClip = pixels;
    resized();
}

int64 RegionComponent::getPositionX()
{
	return _posX;
}

int64 RegionComponent::getRegionWidth()
{
	return _lengthSeconds * _pixelsPerClip;
}

Audio::Region* RegionComponent::getRegion()
{
    return _region;
}


void RegionComponent::resized()
{
    repaint();
}