/*
  ==============================================================================

    SampleRegion.cpp
    Created: 1 Aug 2015 11:38:11pm
    Author:  Thomas

  ==============================================================================
*/

#include "SampleRegion.h"

namespace Audio
{
    SampleRegion::SampleRegion(AudioFormatReader *fileFormatReader)
        : Region(fileFormatReader->lengthInSamples), _reader(fileFormatReader)
    {
    }

    SampleRegion::~SampleRegion()
    {
    }

    void SampleRegion::getNextAudioBlock(const AudioSourceChannelInfo & bufferToFill)
    {
        _reader->read(bufferToFill.buffer, bufferToFill.startSample, bufferToFill.numSamples, _currentPosition, true, true);
        _currentPosition += bufferToFill.numSamples;
    }
}