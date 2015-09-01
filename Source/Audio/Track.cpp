/*
  ==============================================================================

    Track.cpp
    Created: 1 Aug 2015 4:27:45pm
    Author:  Thomas

  ==============================================================================
*/

#include <stdexcept>
#include "Track.h"
#include "../Utility/Conversion.h"

namespace Audio
{
    Track::Track() : _totalLength(0) {
    }

    Track::~Track() {
        for (auto region = _regions.begin(), end = _regions.end(); region != end; ++region) {
            delete _resampled.at(region->second);
            delete region->second;
        }
    }

    void Track::add(int64 position, Region* region) {
        auto resampled = new ResamplingAudioSource(region, false, 2);

        _regions.insert(std::pair<int64, Region *>(position, region));
        _resampled.insert(std::pair<Region *, ResamplingAudioSource *>(region, resampled));

        if (position >= _totalLength) {
            _totalLength = position + region->getTotalLength();
        }
    }

    Region* Track::findCurrentRegion() const {
        return findRegionAt(_currentPosition);
    }

    Region* Track::findRegionAt(int64 position) const {
        for (auto region = _regions.begin(), end = _regions.end(); region != end; ++region) {
            if (region->second->overlaps(position, region->first)) {
                return region->second;
            }
        }

        return nullptr;
    }

    const std::map<int64, Region *> &Track::getRegions() const {
        return _regions;
    }

    bool Track::move(Region *region, int64 position) {
        if (position + region->getTotalLength() >= _totalLength) {
            _totalLength = position + region->getTotalLength();
        }

        for (auto current = _regions.begin(), end = _regions.end(); current != end; ++region) {
            if (current->second == region) {
                auto regionAtPosition = findRegionAt(position);

                if (regionAtPosition != region && regionAtPosition != nullptr) {
                    return false;
                } else {
                    _regions.insert(std::pair<int64, Region *>(position, current->second));
                    _regions.erase(current->first);
                    return true;
                }
            }
        }

        throw std::range_error("The region has not been found in the track.");
    }
    
    bool Track::remove(Region *region, int64 position)
    {
        for (auto current = _regions.begin(), end = _regions.end(); current != end; ++region) {
            if (current->second == region) {
                auto regionAtPosition = findRegionAt(position);
                
                if (regionAtPosition != region && regionAtPosition != nullptr) {
                    return false;
                }
				else {
                    _regions.erase(current->first);
                    delete region;
                    return true;
                }
            }
        }
		return false;
    }

    void Track::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
        _samples = samplesPerBlockExpected;
        _rate = sampleRate;

        for (auto region = _regions.begin(), end = _regions.end(); region != end; ++region) {
            ResamplingAudioSource* resampled = _resampled.at(region->second);

            region->second->prepareToPlay(samplesPerBlockExpected, sampleRate);
            resampled->prepareToPlay(samplesPerBlockExpected, sampleRate);
            resampled->setResamplingRatio(region->second->getBaseSampleRate() / sampleRate);
        }
    }

    void Track::releaseResources() {
    }

    void Track::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) {
        Region* region = findCurrentRegion();

        if (region) {
            _resampled.at(region)->getNextAudioBlock(bufferToFill);
        } else {
            bufferToFill.buffer->clear();
        }

        _currentPosition += bufferToFill.numSamples;
    }

    void Track::setNextReadPosition(int64 newPosition) {
        bool found = false;

        for (auto region = _regions.begin(), end = _regions.end(); region != end; ++region) {
            if (found) {
                region->second->setNextReadPosition(0);
            } else {
                if (newPosition <= region->second->getTotalLength() + region->first) {
                    region->second->setNextReadPosition(newPosition < region->first ? 0 : newPosition - region->first);
                }
            }
        }

        _currentPosition = newPosition;
    }

    int64 Track::getNextReadPosition() const {
        return _currentPosition;
    }
    
    void Track::setTotalLength(int64 length)
    {
        _totalLength = length;
    }

    int64 Track::getTotalLength() const {
        return _totalLength;
    }

    bool Track::isLooping() const {
        return false;
    }
}

#if JUCE_UNIT_TESTS

class TrackTests  : public UnitTest
{
public:
    TrackTests() : UnitTest ("Random") {}
    
    void runTest()
    {
        beginTest ("Random");
        
        Region* testRegion = new Audio::SampleRegion()
    }
};

static RandomTests randomTests;

#endif
