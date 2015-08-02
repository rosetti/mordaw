/*
  ==============================================================================

    Region.h
    Created: 1 Aug 2015 11:27:32pm
    Author:  Thomas

  ==============================================================================
*/

#ifndef REGION_H_INCLUDED
#define REGION_H_INCLUDED

#include "../../JuceLibraryCode/JuceHeader.h"

namespace Audio
{
    class Region : public PositionableAudioSource
    {
    public:
        virtual ~Region();

        virtual double getBaseSampleRate() const = 0;

        // Inherited via PositionableAudioSource
        virtual void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
        virtual void releaseResources() override;
        virtual void setNextReadPosition(int64 newPosition) override;
        virtual int64 getNextReadPosition() const override;
        virtual int64 getTotalLength() const override;
        virtual bool isLooping() const override;

    protected:
        Region(int64 length);

        int _samples;
        double _rate;
        int64 _currentPosition;
        int64 _totalLength;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Region)

    };
}


#endif  // REGION_H_INCLUDED
