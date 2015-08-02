/*
  ==============================================================================

    Engine.cpp
    Created: 1 Aug 2015 4:24:58pm
    Author:  Thomas

  ==============================================================================
*/

#include "Engine.h"
#include "Track.h"
#include "SampleRegion.h"

namespace Audio {
    Engine::Engine()
    {
        AudioIODevice* current;

        _formats.registerBasicFormats();
        _devices.initialiseWithDefaultDevices(2, 2);
        current = _devices.getCurrentAudioDevice();

        _mixer = new Mixer(2, 2, current->getCurrentSampleRate(), current->getCurrentBufferSizeSamples());
        _player.setProcessor(_mixer->getProcessorGraph());
        _devices.addAudioCallback(&_player);
        
        //// CODE FOR YOU TO TEST
        //File &currentDir = File::getCurrentWorkingDirectory();
        //File &file = currentDir.getChildFile("be_cool.wav");
        //
        //AudioFormatReader *reader = _formats.createReaderFor(file);
        //AudioFormatReader *reader2 = _formats.createReaderFor(file);
        //AudioFormatReader *reader3 = _formats.createReaderFor(file);

        //Track *track = new Track();
        //Track *track2 = new Track();
        //Region *region = new SampleRegion(reader, reader->sampleRate / current->getCurrentSampleRate());
        //Region *region2 = new SampleRegion(reader2, reader2->sampleRate / current->getCurrentSampleRate());
        //Region *region3 = new SampleRegion(reader3, reader3->sampleRate / current->getCurrentSampleRate());

        //track->add(0, region);
        //track->add(region->getTotalLength() + region->getTotalLength() / 4, region2);
        //track2->add(region->getTotalLength() / 2, region3);
        //_mixer->add(track);
        //_mixer->add(track2);

        //_mixer->startPlayingAt(0);
    }

    Engine::~Engine()
    {
        _mixer->stop();
        _devices.removeAudioCallback(&_player);
    }
}