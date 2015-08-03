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
    SampleRegion::SampleRegion(AudioFormatReader* fileFormatReader, double resampleRatio)
        : Region(static_cast<int64>(ceil(fileFormatReader->lengthInSamples / resampleRatio))), _reader(fileFormatReader) {
    }

    SampleRegion::~SampleRegion() {
        delete _reader;
    }

    double SampleRegion::getBaseSampleRate() const {
        return _reader->sampleRate;
    }

    void SampleRegion::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) {
        _reader->read(bufferToFill.buffer, bufferToFill.startSample, bufferToFill.numSamples, _currentPosition, true, true);
        _currentPosition += bufferToFill.numSamples;
    }
}
