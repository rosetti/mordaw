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
    _posX(x),
    _sampleRate(sampleRate),
    _pixelsPerClip(pixelsPerClip)
{
	_inputSource = new FileInputSource(file),
	_thumbnailCache = new AudioThumbnailCache(100);
	_thumbnail = new AudioThumbnail(1024, formatManager, *_thumbnailCache);
    _thumbnail->setSource(_inputSource);
    setOpaque(true);
    setAlwaysOnTop(true);
    _filename = file.getFileName();
	_lengthSeconds = (int64)samplesToSeconds(_region->getLengthInSamples(), _sampleRate);
}

RegionComponent::~RegionComponent()
{
	//_thumbnail->setSource(nullptr);
    //_thumbnailCache->clear();
}

void RegionComponent::paint (Graphics& g)
{
    g.fillAll(Colours::steelblue);
    Rectangle<int> bounds_;
<<<<<<< HEAD
    bounds_.setHeight(getParentHeight());
    bounds_.setWidth((int)_lengthSeconds * (int)_pixelsPerClip);
=======
    int64 lengthSeconds = (int64)samplesToSeconds(_region->getLengthInSamples(), _sampleRate);
    bounds_.setHeight(getParentHeight()-20);
    bounds_.setWidth((int)lengthSeconds * (int)_pixelsPerClip);
>>>>>>> origin/master
    g.reduceClipRegion(bounds_);
    g.fillAll(Colours::grey);
    //int64 posSamples = pixelsToSamples(_posX, getWidth(), _region->getLengthInSamples());
    //int64 posSeconds = samplesToSeconds(posSamples, 44100.f);
    g.setColour(Colours::black);
    g.fillRect(bounds_);
    g.setColour(Colours::green);
    _thumbnail->drawChannels(g, bounds_, 0.0f, (int)_lengthSeconds, 0.5f);
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
	int64 regionWidth = _lengthSeconds * _pixelsPerClip;
	return regionWidth;
}

Audio::Region* RegionComponent::getRegion()
{
    return _region;
}


void RegionComponent::resized()
{
    repaint();
}