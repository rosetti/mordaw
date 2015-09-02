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
    /*
     Constructs an engine object
     @param numInputChannels A pointer to an instance of command manager.
     */
    Engine::Engine(ApplicationCommandManager *commands) : _commands(commands),
	_formats(), _recordedFiles(0)
	{

        AudioIODevice* current;

        // register basic file formats
        _formats.registerBasicFormats();
        // initialise the device to a 2 input, 2 output configuration
        _devices.initialiseWithDefaultDevices(2, 2);
        
        current = _devices.getCurrentAudioDevice();

        // create a new mixer object with the current device settings
        _mixer = new Mixer(2, 2, current->getCurrentSampleRate(), current->getCurrentBufferSizeSamples());
        
        // set the processor player to the mixers graph
        _player.setProcessor(_mixer->getProcessorGraph());
        // add device callbacks
        _devices.addAudioCallback(&_player);
        _devices.addAudioCallback(&_recorder);
    }

    // destructor
    Engine::~Engine() {
        _mixer->stop();
        _devices.removeAudioCallback(&_player);
        _devices.removeAudioCallback(&_recorder);
        // loop until lock is gained and delete commands
        for(;;)
        {
            MessageManagerLock mm;
            if(mm.lockWasGained())
            {
                delete _commands;
                break;
            }
        }
    }
    
    /*
     Gets the command info for a specified commandID
     */
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

   	/*
     Adds all the command ids to the commands array
     */
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
    
    /*
     Get the current sample rate.
     */
    double Engine::getCurrentSamplerate() const
    {
        AudioIODevice*  current = _devices.getCurrentAudioDevice();
        return current->getCurrentSampleRate();
    }
    
    /*
     Get the total length of project in samples.
     */
    int64 Engine::getTotalLength() const
    {
        return _totalLength;
    }
    
    /*
     Get the recorded file names to add as a region.
     */
    StringArray Engine::getRecordedFileNames() const
    {
        return _recordedFileNames;
    }

    /*
     Return the mixer graph.
     */
    Mixer* Engine::getMixer() const {
        return _mixer;
    }
    
}
