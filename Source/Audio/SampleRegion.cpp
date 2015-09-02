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
    /*
     Constructs an track object with a total length of 0.
     @param fileFormatReader The format reader associated with the file.
     @param resampleRatio The ratio for resampling
     @param file The file being added to the region
     */
    SampleRegion::SampleRegion(AudioFormatReader* fileFormatReader, double resampleRatio, File*)
        : Region(static_cast<int64>(ceil(fileFormatReader->lengthInSamples / resampleRatio))), _reader(fileFormatReader) {
    }

    /*
     Delete the format reader
     */
    SampleRegion::~SampleRegion() {
        delete _reader;
    }

    /*
     Get the files sample rate
     */
    double SampleRegion::getBaseSampleRate() const {
        return _reader->sampleRate;
    }

    /*
     Process the next block (inherited from AudioSource)
     */
    void SampleRegion::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) {
        _reader->read(bufferToFill.buffer, bufferToFill.startSample, bufferToFill.numSamples, _currentPosition, true, true);
        _currentPosition += bufferToFill.numSamples;
    }
}
