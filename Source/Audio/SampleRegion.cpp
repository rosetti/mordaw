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
    SampleRegion::SampleRegion(AudioFormatReader* fileFormatReader, double resampleRatio, File* file)
        : Region(static_cast<int64>(ceil(fileFormatReader->lengthInSamples / resampleRatio))), _reader(fileFormatReader), _file(file) {
    }

    SampleRegion::~SampleRegion() {
        delete _reader;
    }

    double SampleRegion::getBaseSampleRate() const {
        return _reader->sampleRate;
    }

	String SampleRegion::getFilePath()
	{
		return _file->getFullPathName();
	}

	File& SampleRegion::getFile()
	{
		return *_file;
	}

    void SampleRegion::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) {
        _reader->read(bufferToFill.buffer, bufferToFill.startSample, bufferToFill.numSamples, _currentPosition, true, true);
        _currentPosition += bufferToFill.numSamples;
    }
}
