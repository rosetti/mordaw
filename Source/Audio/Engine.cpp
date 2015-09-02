/*
  ==============================================================================

    Engine.cpp
    Created: 1 Aug 2015 4:24:58pm
    Author:	Dan
			Thomas (Refactored + Command Manger)

  ==============================================================================
*/

#include "Engine.h"
#include "SampleRegion.h"
#include "../GUI/MainWindow.h"


namespace Audio
{
    /*
     Constructs an engine object
<<<<<<< HEAD
     @param numInputChannels A pointer to an instance of command manager.
=======
     @param numInputChannels The number of input channels required
>>>>>>> master
     */
    Engine::Engine(ApplicationCommandManager *commands) : _commands(commands),
	_formats(), _recordedFiles(0)
	{
        // pointer for current device
        AudioIODevice* current;

        // register basic file formats
        _formats.registerBasicFormats();
<<<<<<< HEAD
        // initialise the device to a 2 input, 2 output configuration
=======
        // initialise devices with stereo input and output
>>>>>>> master
        _devices.initialiseWithDefaultDevices(2, 2);
        
        // set current device pointer to the current device
        current = _devices.getCurrentAudioDevice();

<<<<<<< HEAD
        // create a new mixer object with the current device settings
        _mixer = new Mixer(2, 2, current->getCurrentSampleRate(), current->getCurrentBufferSizeSamples());
        
        // set the processor player to the mixers graph
        _player.setProcessor(_mixer->getProcessorGraph());
        // add device callbacks
=======
        // create a mixer object with stereo input and output and the devices current sample rate and buffer size
        _mixer = new Mixer(2, 2, current->getCurrentSampleRate(), current->getCurrentBufferSizeSamples());
        // set the processor player to the mixer graph
        _player.setProcessor(_mixer->getProcessorGraph());
        // add the mixer graph callback
>>>>>>> master
        _devices.addAudioCallback(&_player);
        // add the recorder callback (only partially implemented)
        _devices.addAudioCallback(&_recorder);
    }

    // destructor
    Engine::~Engine() {
        // stop the mixer
        _mixer->stop();
        // remove player callback
        _devices.removeAudioCallback(&_player);
<<<<<<< HEAD
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
=======
        // remove the recorder callback
        _devices.addAudioCallback(&_recorder);
    }
    
    /*
     Gets the command info for a particular commandID 
>>>>>>> master
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

<<<<<<< HEAD
   	/*
     Adds all the command ids to the commands array
=======
    /*
     Gets an array of all the commands.
>>>>>>> master
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

    /*
     Perform the command
     */
    bool Engine::perform(const ApplicationCommandTarget::InvocationInfo& info) {
        switch (info.commandID) {
        case start:
            // start playing at position 0
            _mixer->startPlayingAt(0);
            break;
        case pause:
            // Pause the mixer
            break;
        case stop:
            // stop playing
            _mixer->stop();
            break;

        case rewind:
            // trigger stop to set the position to 0, if is playing start playing again from 0
            if (_mixer->isPlaying()) {
                _mixer->stop();
                _mixer->startPlayingAt(0);
            } else {
                _mixer->stop();
            }
            break;

        case forward:
            // set the position to end of track
            _mixer->stop();
            _mixer->goToTheEnd();

            break;
        case record:
            // records the microphone input to file, currently not integrated with UI regions
            const File file (File::getSpecialLocation (File::userDocumentsDirectory)
                                 .getNonexistentChildFile ("recording" + (String) _recordedFiles, ".wav"));
            // if not recording stary recording
            if(!_recorder.isRecording())
            {
                // record the microphone to a file
                _recorder.startRecording(file);
            }
            // if record command is called again stop recording and add file name to the file names array
            else if (_recorder.isRecording())
            {
                _recorder.stop();
                _recordedFileNames.add(file.getFullPathName());

            }
            break;
        }
        // refresh components
        _commands->invokeDirectly(MainWindow::refreshComponents, true);

        return true;
    }
    
    /*
     Get the current device sample rate
     */
    double Engine::getCurrentSamplerate() const
    {
        AudioIODevice*  current = _devices.getCurrentAudioDevice();
        return current->getCurrentSampleRate();
    }
    
    /*
    Get the total length of the project
     */
    int64 Engine::getTotalLength() const
    {
        return _totalLength;
    }
    
    /*
     Get the recorded file name array
     */
    StringArray Engine::getRecordedFileNames() const
    {
        return _recordedFileNames;
    }

    /*
     Return the mixer object
     */
    Mixer* Engine::getMixer() const {
        return _mixer;
    }
    
}
