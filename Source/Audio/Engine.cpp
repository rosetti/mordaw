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
#include "../GUI/MainWindow.h"

namespace Audio
{
    Engine::Engine(ApplicationCommandManager *commands) : _commands(commands) {
        AudioIODevice* current;

        _formats.registerBasicFormats();
        _devices.initialiseWithDefaultDevices(2, 2);
        current = _devices.getCurrentAudioDevice();

        _mixer = new Mixer(2, 2, current->getCurrentSampleRate(), current->getCurrentBufferSizeSamples());
        _player.setProcessor(_mixer->getProcessorGraph());
        _devices.addAudioCallback(&_player);

        //// CODE FOR YOU TO TEST
        const File &currentDir = File::getCurrentWorkingDirectory();
        const File &file = currentDir.getChildFile("be_cool.wav");
        
        AudioFormatReader *reader = _formats.createReaderFor(file);
        AudioFormatReader *reader2 = _formats.createReaderFor(file);
        AudioFormatReader *reader3 = _formats.createReaderFor(file);

        Track *track = new Track();
        Track *track2 = new Track();
        Region *region = new SampleRegion(reader, reader->sampleRate / current->getCurrentSampleRate());
        Region *region2 = new SampleRegion(reader2, reader2->sampleRate / current->getCurrentSampleRate());
        Region *region3 = new SampleRegion(reader3, reader3->sampleRate / current->getCurrentSampleRate());

        track->add(0, region);
        track->add(region->getTotalLength() + region->getTotalLength() / 4, region2);
        track2->add(region->getTotalLength() / 2, region3);
        _mixer->add(track);
        _mixer->add(track2);
    }

    Engine::~Engine() {
        _mixer->stop();
        _devices.removeAudioCallback(&_player);
    }

    void Engine::getCommandInfo(CommandID commandID, ApplicationCommandInfo& result) const {
        const String audio("Audio");
        int flags;

        switch (commandID) {
        case start:
            flags = _mixer->isPlaying() ? ApplicationCommandInfo::isDisabled : 0;
            result.setInfo("Play", "Play the project at the position on the timeline.", audio, flags);
            result.addDefaultKeypress(KeyPress::spaceKey, 0);
            break;

        case pause:
            flags = _mixer->isPlaying() ? 0 : ApplicationCommandInfo::isDisabled;
            result.setInfo("Pause", "Pause the player.", audio, flags);
            result.addDefaultKeypress(KeyPress::spaceKey, 0);
            break;

        case stop:
            flags = _mixer->isPlaying() ? 0 : ApplicationCommandInfo::isDisabled;
            result.setInfo("Stop", "Stop the player and reset the timeline.", audio, flags);
            result.addDefaultKeypress(KeyPress::escapeKey, 0);
            break;

        default:
            break;
        }
    }

    void Engine::getAllCommands(Array<CommandID>& commands) const {
        const CommandID ids[] = {
            start,
            pause,
            stop,
        };

        commands.addArray(ids, numElementsInArray(ids));
    }

    bool Engine::perform(const ApplicationCommandTarget::InvocationInfo& info) {
        switch (info.commandID) {
        case start:
            // @todo replace 0 with timeline position
            _mixer->startPlayingAt(0);
            break;
        case pause:
            // Pause the mixer
            break;
        case stop:
            _mixer->stop();
            break;
        default:
            return false;
        }

        _commands->invokeDirectly(MainWindow::refreshComponents, true);

        return true;
    }

    Mixer* Engine::getMixer() const {
        return _mixer;
    }
}
