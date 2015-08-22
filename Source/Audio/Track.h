/*
  ==============================================================================

    Track.h
    Created: 1 Aug 2015 4:27:45pm
    Author:  Thomas

  ==============================================================================
*/

#ifndef TRACK_H_INCLUDED
#define TRACK_H_INCLUDED

#include <map>
#include "Region.h"

namespace Audio
{
    class Track : public PositionableAudioSource
    {
    public:
        Track();
        ~Track();

        void add(int64 positionInTimeline, Region *region);
        Region *findCurrentRegion() const;
        Region *findRegionAt(int64 position) const;
        const std::map<int64, Region *> &getRegions() const;
        bool move(const Region *region, int64 position);

        // Inherited via PositionableAudioSource
        virtual void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
        virtual void releaseResources() override;
        virtual void getNextAudioBlock(const AudioSourceChannelInfo & bufferToFill) override;
        virtual void setNextReadPosition(int64 newPosition) override;
        virtual int64 getNextReadPosition() const override;
        virtual int64 getTotalLength() const override;
        virtual bool isLooping() const override;

		std::map<int64, Region *> *getRegionMap();

    private:
        int _samples;
        double _rate;
        int64 _currentPosition;
        int64 _totalLength;

        std::map<int64, Region *> _regions;
        std::map<Region *, ResamplingAudioSource *> _resampled;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Track)
    };
}



#endif  // TRACK_H_INCLUDED
