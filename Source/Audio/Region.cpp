/*
==============================================================================

Region.cpp
Created: 1 Aug 2015 11:27:32pm
Author:  Thomas

==============================================================================
*/

#include "Region.h"

namespace Audio {
    Region::Region(int64 length) : _currentPosition(0), _totalLength(length)
    {
    }

    Region::~Region()
    {
    }

    bool Region::overlaps(int64 position, int64 startPosition) {
        return position >= startPosition && position < getTotalLength() + startPosition;
    }

    void Region::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
    {
        _samples = samplesPerBlockExpected;
        _rate = sampleRate;
    }

    void Region::releaseResources()
    {
    }

    void Region::setNextReadPosition(int64 newPosition)
    {
        _currentPosition = newPosition;
    }

    int64 Region::getNextReadPosition() const
    {
        return _currentPosition;
    }

    int64 Region::getTotalLength() const
    {
        return _totalLength;
    }

    bool Region::isLooping() const
    {
        return false;
    }
}