/*
  ==============================================================================

    Engine.cpp
    Created: 1 Aug 2015 4:24:58pm
    Author:	Dan
			Thomas (Refactored)

  ==============================================================================
*/

#include "Engine.h"
#include "SampleRegion.h"
#include "../GUI/MainWindow.h"

namespace Audio
{
    Engine::Engine(ApplicationCommandManager *commands) : _commands(commands),
	_formats(), _recordedFiles(0)
	{

        AudioIODevice* current;

        _formats.registerBasicFormats();
        _devices.initialiseWithDefaultDevices(2, 2);
        
        current = _devices.getCurrentAudioDevice();

        _mixer = new Mixer(2, 2, current->getCurrentSampleRate(), current->getCurrentBufferSizeSamples());
        _player.setProcessor(_mixer->getProcessorGraph());
        _devices.addAudioCallback(&_player);
        _devices.addAudioCallback(&_recorder);
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

        case rewind:
            result.setInfo("Rewind", "Rewind to the beginning of the timeline.", audio, 0);
            result.addDefaultKeypress('B', ModifierKeys::commandModifier);
            break;

        case forward:
            result.setInfo("Forward", "Forward to the end of the timeline.", audio, 0);
            result.addDefaultKeypress('F', ModifierKeys::commandModifier);
            break;
        
        case record:
            result.setInfo("Records", "Records to file and adds to timeline.", audio, 0);
            result.addDefaultKeypress('R', ModifierKeys::commandModifier);
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
            rewind,
            forward,
            record
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
            //_mixer->resetOutput();
            break;

        case rewind:
            if (_mixer->isPlaying()) {
                _mixer->stop();
                _mixer->startPlayingAt(0);
            } else {
                _mixer->stop();
            }
            break;

        case forward:
            _mixer->stop();
            _mixer->goToTheEnd();

            break;
        case record:
            const File file (File::getSpecialLocation (File::userDocumentsDirectory)
                                 .getNonexistentChildFile ("recording" + (String) _recordedFiles, ".wav"));
            if(!_recorder.isRecording())
            {
                _recorder.startRecording(file);
            }
            else if (_recorder.isRecording())
            {
                _recorder.stop();
                _recordedFileNames.add(file.getFullPathName());

            }
            break;
        }

        _commands->invokeDirectly(MainWindow::refreshComponents, true);

        return true;
    }
    
    double Engine::getCurrentSamplerate() const
    {
        AudioIODevice*  current = _devices.getCurrentAudioDevice();
        return current->getCurrentSampleRate();
    }
    
    // seemingly useless
    int64 Engine::getTotalLength() const
    {
        return _totalLength;
    }
    
    StringArray Engine::getRecordedFileNames() const
    {
        return _recordedFileNames;
    }

    Mixer* Engine::getMixer() const {
        return _mixer;
    }
    
}
