/*
  ==============================================================================

    SampleRegion.h
    Created: 1 Aug 2015 11:38:11pm
    Author:  Thomas

  ==============================================================================
*/

#ifndef SAMPLEREGION_H_INCLUDED
#define SAMPLEREGION_H_INCLUDED

#include "Region.h"

namespace Audio
{
    class SampleRegion : public Region
    {
    public:
        SampleRegion(AudioFormatReader *fileFormatReader);
        ~SampleRegion();

    private:
        AudioFormatReader *_reader;

        // Inherited via Region
        virtual void getNextAudioBlock(const AudioSourceChannelInfo & bufferToFill) override;
    };
}



#endif  // SAMPLEREGION_H_INCLUDED
