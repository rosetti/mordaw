/*
  ==============================================================================

    ExportProcessor.cpp
    Created: 28 Aug 2015 12:24:23am
    Author:  Dan

  ==============================================================================
*/

#include "ExportProcessor.h"
#include "../Utility/Conversion.h"

//==============================================================================
ExportProcessor::ExportProcessor()
: backgroundThread ("Export Thread"), _sampleRate(0), _nextSampleIndex(0), _activeWriter(nullptr)
{
    backgroundThread.startThread();
}

// stop exporting on destruct
ExportProcessor::~ExportProcessor()
{
    stopExporting();
}

// Start exporting to file
void ExportProcessor::startExporting(const File& file)
{
    if(_sampleRate > 0)
    {
        file.deleteFile();
        FileOutputStream* fileStream (file.createOutputStream());

        if(fileStream != nullptr)
        {
            WavAudioFormat wavFormat;
            AudioFormatWriter* writer = wavFormat.createWriterFor (fileStream, _sampleRate, 2, 32, StringPairArray(), 0);
            
            if (writer != nullptr)
            {
                _threadedWriter = new AudioFormatWriter::ThreadedWriter (writer, backgroundThread, 32768);
                _nextSampleIndex = 0;
                
                const ScopedLock scoped(getCallbackLock());
                _activeWriter =_threadedWriter;
            }
        }
    }
}

// Stop exporting
void ExportProcessor::stopExporting()
{
    const ScopedLock sl (getCallbackLock());
    _activeWriter = nullptr;
    _threadedWriter = nullptr;

}

// Check if processor is exporting
bool ExportProcessor::isExporting()
{
    return _activeWriter != nullptr;

}

// get number of parameters
int ExportProcessor::getNumParameters()
{
    return 3;
}

// no parameters
float ExportProcessor::getParameter(int)
{
     return 0.0f;
}

// Inherited from AudioSource
void ExportProcessor::setParameter(int, float )
{
}

// no parameters
const String ExportProcessor::getParameterName(int)
{
	return "string";
}

// no parameters
const String ExportProcessor::getParameterText(int)
{
	return "string";
}

// Get the channel name
const String ExportProcessor::getInputChannelName(int channelIndex) const
{
    return String(channelIndex + 1);
}

// Get the channel name
const String ExportProcessor::getOutputChannelName(int channelIndex) const
{
    return String(channelIndex + 1);
}

// Processor is stereo pair
bool ExportProcessor::isInputChannelStereoPair(int) const
{
    return true;
}

// Processor is stereo pair
bool ExportProcessor::isOutputChannelStereoPair(int) const
{
    return true;
}

// Check if processor accepts midi
bool ExportProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

// Check if processor produces midi
bool ExportProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

// Inherited to AudioSource
bool ExportProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

// Prepare the processor to play
void ExportProcessor::prepareToPlay(double, int)
{
    _sampleRate = getSampleRate();
    setPlayConfigDetails(2, 0, getSampleRate(), getBlockSize());
}

// Set sample rate to 0
void ExportProcessor::releaseResources()
{
    _sampleRate = 0;
}

// Write the the sample buffer to a file.
void ExportProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer&)
{
    if(_activeWriter != 0)
        _activeWriter->write((const float**)buffer.getArrayOfReadPointers(), buffer.getNumSamples());
}

// Inherited from AudioSource
void ExportProcessor::getStateInformation(MemoryBlock&)
{

}

// Inherited from AudioSource
void ExportProcessor::setStateInformation(const void*, int)
{

}

// Inherited from AudioSource
const void ExportProcessor::setID(int)
{
}

// Inherited from AudioSource
const int ExportProcessor::getID()
{
	return 0;
}

// Inherited from AudioSource
bool ExportProcessor::hasEditor() const
{
    return true;
}

// Inherited from AudioSource
AudioProcessorEditor* ExportProcessor::createEditor()
{
    return nullptr;
}