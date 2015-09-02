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
    /*
     Constructs an track object with a total length of 0.
     */
    Track::Track() : _totalLength(0) {
    }

    /*
     Delete all regions
     */
    Track::~Track() {
        for (auto region = _regions.begin(), end = _regions.end(); region != end; ++region) {
            delete _resampled.at(region->second);
            delete region->second;
        }
    }

    /*
     Add a region to the track.
     */
    void Track::add(int64 position, Region* region) {
        auto resampled = new ResamplingAudioSource(region, false, 2);

        _regions.insert(std::pair<int64, Region *>(position, region));
        _resampled.insert(std::pair<Region *, ResamplingAudioSource *>(region, resampled));

        if (position >= _totalLength) {
            _totalLength = position + region->getTotalLength();
        }
    }

    /*
     Find the region at the current position in samples.
     */
    Region* Track::findCurrentRegion() const {
        return findRegionAt(_currentPosition);
    }

    /*
     Find the region at a particular position.
     */
    Region* Track::findRegionAt(int64 position) const {
        for (auto region = _regions.begin(), end = _regions.end(); region != end; ++region) {
            if (region->second->overlaps(position, region->first)) {
                return region->second;
            }
        }

        return nullptr;
    }

    /*
     Get the regions map.
     */
    const std::map<int64, Region *> &Track::getRegions() const {
        return _regions;
    }

    /*
     Move a region to a particular position in samples.
     */
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
    
    /*
     Remove a region which contains a particular position in samples.
     */
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

    /*
     Prepare the track to play (inherited from AudioSource)
     */
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

    /*
     Inherite from AudioSource
     */
    void Track::releaseResources() {
    }

    /*
     Process the next audio block (inherited from AudioSource)
     */
    void Track::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) {
        Region* region = findCurrentRegion();

        if (region) {
            _resampled.at(region)->getNextAudioBlock(bufferToFill);
        } else {
            bufferToFill.buffer->clear();
        }

        _currentPosition += bufferToFill.numSamples;
    }

    /*
     Set the next region position in samples (inherited from AudioSource).
     */
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

    /*
     Get the next read position.
     */
    int64 Track::getNextReadPosition() const {
        return _currentPosition;
    }
    
    /*
     Set the total length in samples.
     */
    void Track::setTotalLength(int64 length)
    {
        _totalLength = length;
    }

    /*
     Get the total length in samples.
     */
    int64 Track::getTotalLength() const {
        return _totalLength;
    }

    /*
     Set if the track is looping, currently always false.
     */
    bool Track::isLooping() const {
        return false;
    }
}

#if JUCE_UNIT_TESTS

class TrackTests  : public UnitTest
{
public:
    TrackTests() : UnitTest ("Track") {}
    
    void runTest()
    {
        beginTest ("Track");
        AudioFormatManager manager;
        manager.registerBasicFormats();
        // add the test file to your home directory
        File* file  = new File("~/test.wav");
        AudioFormatReader* reader = manager.createReaderFor(*file);
        Audio::Region* testRegion = new Audio::SampleRegion(reader, 1, file);
        Audio::Track* testTrack = new Audio::Track();
        long rand = random();
        testTrack->add(rand, testRegion);
        for (int j = 10; --j >= 0;)
        {
            for (int i = 20; --i >= 0;)
            {
                expect (testTrack->getTotalLength() == (rand + testRegion->getTotalLength()));
                expect (testRegion->getBaseSampleRate() == (reader->sampleRate));
                expect(reader->bitsPerSample > 0);
            }
        }
    }
};

static TrackTests trackTests;

#endif
