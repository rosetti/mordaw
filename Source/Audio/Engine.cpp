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
        _devices.initialiseWithDefaultDevices(0, 2);
        _formats.registerBasicFormats();

        //AudioDeviceManager *manager = new AudioDeviceManager();
        //
        //File &currentDir = File::getCurrentWorkingDirectory();
        //File &file = currentDir.getChildFile("reveil.flac");
        //
        //AudioFormatManager *formats = new AudioFormatManager();
        //formats->registerBasicFormats();

        //AudioFormatReader *reader = formats->createReaderFor(file);

        //Track *track = new Track();
        //Region *region = new SampleRegion(reader);

        //track->add(0, region);

        //AudioTransportSource *transport = new AudioTransportSource();
        //transport->setSource(track, 0, nullptr, reader->sampleRate, 2);
        //transport->start();

        //AudioSourcePlayer *player = new AudioSourcePlayer();
        //player->setSource(transport);

        //manager->initialiseWithDefaultDevices(0, 2);
        //manager->addAudioCallback(player);
    }

    Engine::~Engine()
    {
    }
}