/*
  ==============================================================================

    RegionComponent.cpp
    Created: 13 Aug 2015 12:21:11pm
	Author: Matt & Dan
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
    _filename = file.getFileName();
	_lengthSeconds = static_cast<int64>(samplesToSeconds(_region->getLengthInSamples(), _sampleRate));
}

RegionComponent::~RegionComponent()
{
    removeAllChildren();
	
	_thumbnail->setSource(nullptr);
	_thumbnail->clear();
	_thumbnailCache->clear();
    delete _thumbnail;
    delete _thumbnailCache;
}

void RegionComponent::paint (Graphics& g)
{
    g.fillAll(Colours::steelblue);
    auto lengthSeconds = static_cast<int64>(samplesToSeconds(_region->getLengthInSamples(), _sampleRate));
    _bounds.setHeight(getParentHeight()-20);
    _bounds.setWidth(static_cast<int>(lengthSeconds) * static_cast<int>(_pixelsPerClip));
    g.reduceClipRegion(_bounds);
    g.fillAll(Colours::grey);
    g.setColour(Colours::black);
    g.fillRect(_bounds);
    g.setColour(Colours::green);
    _thumbnail->drawChannels(g, _bounds, 0.0f, static_cast<int>(_lengthSeconds), 0.5f);
    
    g.setColour(Colours::white);
    g.setFont(8.0f);
    g.drawText(_filename, _bounds, Justification::topLeft);
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