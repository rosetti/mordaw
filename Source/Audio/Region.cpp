/*
==============================================================================

Region.cpp
Created: 1 Aug 2015 11:27:32pm
Author:  Thomas

==============================================================================
*/

#include "Region.h"

namespace Audio {
    /*
     Constructs an region object with a total length of 0.
     @param length The length of a region in samples
     */
	Region::Region(int64 length) : _currentPosition(0), _totalLength(length)
	{
	}

	Region::~Region()
	{
	}

    /*
     Get the total length of the region insamples
     */
	int64 Region::getLengthInSamples()
	{
		return _totalLength;
	}

    /*
     Check to see if the region overlaps.
     */
	bool Region::overlaps(int64 position, int64 startPosition) {
		return position >= startPosition && position < getTotalLength() + startPosition;
	}

    /*
     Prepare the region to play (inherited from AudioSource).
     */
	void Region::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
	{
		_samples = samplesPerBlockExpected;
		_rate = sampleRate;
	}

	void Region::releaseResources()
	{
	}

    /*
     Set the current position to the new position
     */
	void Region::setNextReadPosition(int64 newPosition)
	{
		_currentPosition = newPosition;
	}

    /*
     Get the current read position.
     */
	int64 Region::getNextReadPosition() const
	{
		return _currentPosition;
	}

    /*
     Get the total length of the region.
     */
	int64 Region::getTotalLength() const
	{
		return _totalLength;
	}

    /*
     Check whether the region is looping, currently always false.
     */
	bool Region::isLooping() const
	{
		return false;
	}
}