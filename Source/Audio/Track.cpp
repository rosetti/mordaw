/*
  ==============================================================================

    Track.cpp
    Created: 1 Aug 2015 4:27:45pm
    Author:  Thomas

  ==============================================================================
*/

#include "Track.h"

namespace Audio {
    Track::Track() : _totalLength(0)
    {
    }

    Track::~Track()
    {
        for (auto region = _regions.begin(), end = _regions.end(); region != end; ++region) {
            delete region->second;
        }
    }

    void Track::add(int64 position, Region *region)
    {
        _regions.insert(std::pair<int64, Region *>(position, region));
        
        if (position >= _totalLength) {
            _totalLength = position + region->getTotalLength();
        }
    }

    Region * Track::findCurrentRegion() const
    {
        return findRegionAt(_currentPosition);
    }

    Region * Track::findRegionAt(int64 position) const
    {
        for (auto region = _regions.begin(), end = _regions.end(); region != end; ++region) {
            if (position >= region->first && position - region->first <= region->second->getTotalLength()) {
                return region->second;
            }
        }

        return nullptr;
    }

    void Track::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
    {
        _samples = samplesPerBlockExpected;
        _rate = sampleRate;

        for (auto region = _regions.begin(), end = _regions.end(); region != end; ++region) {
            region->second->prepareToPlay(samplesPerBlockExpected, sampleRate);
        }
    }

    void Track::releaseResources()
    {
    }

    void Track::getNextAudioBlock(const AudioSourceChannelInfo & bufferToFill)
    {
        Region *region = findCurrentRegion();

        if (region) {
            region->getNextAudioBlock(bufferToFill);
            _currentPosition += bufferToFill.numSamples;
        }
    }

    void Track::setNextReadPosition(int64 newPosition)
    {
        _currentPosition = newPosition;
    }

    int64 Track::getNextReadPosition() const
    {
        return _currentPosition;
    }

    int64 Track::getTotalLength() const
    {
        return _totalLength;
    }

    bool Track::isLooping() const
    {
        return false;
    }
}
