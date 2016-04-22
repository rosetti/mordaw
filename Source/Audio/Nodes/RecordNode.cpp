/*
 ==============================================================================
 
 RecordNode.cpp
 Created: 28 Aug 2015 12:24:23am
 Author:  Dan
 
 ==============================================================================
 */

#include "RecordNode.h"
#include "../../Utility/Conversion.h"

//==============================================================================
RecordNode::RecordNode()
: _backgroundThread ("Record Thread"),
_activeWriter(nullptr),
_sampleRate(0),
_nextSampleIndex(0)
{
    _backgroundThread.startThread();
}

// stop exporting on destruct
RecordNode::~RecordNode()
{
    stopExporting();
}

// Start exporting to file
void RecordNode::startExporting(const File& file)
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
                _threadedWriter = new AudioFormatWriter::ThreadedWriter (writer, _backgroundThread, 32768);
                _nextSampleIndex = 0;
                
                const ScopedLock scoped(getCallbackLock());
                _activeWriter =_threadedWriter;
            }
        }
    }
}

// Stop exporting
void RecordNode::stopExporting()
{
    const ScopedLock sl (getCallbackLock());
    _activeWriter = nullptr;
    _threadedWriter = nullptr;
    
}

// Check if processor is exporting
bool RecordNode::isExporting()
{
    return _activeWriter != nullptr;
    
}

// get number of parameters
int RecordNode::getNumParameters()
{
    return 3;
}

// no parameters
float RecordNode::getParameter(int)
{
    return 0.0f;
}

// Inherited from AudioSource
void RecordNode::setParameter(int, float )
{
}

// no parameters
const String RecordNode::getParameterName(int)
{
    return "string";
}

// no parameters
const String RecordNode::getParameterText(int)
{
    return "string";
}

// Get the channel name
const String RecordNode::getInputChannelName(int channelIndex) const
{
    return String(channelIndex + 1);
}

// Get the channel name
const String RecordNode::getOutputChannelName(int channelIndex) const
{
    return String(channelIndex + 1);
}

// Processor is stereo pair
bool RecordNode::isInputChannelStereoPair(int) const
{
    return true;
}

// Processor is stereo pair
bool RecordNode::isOutputChannelStereoPair(int) const
{
    return true;
}

// Check if processor accepts midi
bool RecordNode::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

// Check if processor produces midi
bool RecordNode::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

// Inherited to AudioSource
bool RecordNode::silenceInProducesSilenceOut() const
{
    return false;
}

// Prepare the processor to play
void RecordNode::prepareToPlay(double, int)
{
    _sampleRate = getSampleRate();
    setPlayConfigDetails(2, 0, getSampleRate(), getBlockSize());
}

// Set sample rate to 0
void RecordNode::releaseResources()
{
    _sampleRate = 0;
}

// Write the the sample buffer to a file.
void RecordNode::processBlock(AudioSampleBuffer& buffer, MidiBuffer&)
{
    if(_activeWriter != 0)
        _activeWriter->write((const float**)buffer.getArrayOfReadPointers(), buffer.getNumSamples());
}

// Inherited from AudioSource
void RecordNode::getStateInformation(MemoryBlock&)
{
    
}

// Inherited from AudioSource
void RecordNode::setStateInformation(const void*, int)
{
    
}

// Inherited from AudioSource
const void RecordNode::setID(int)
{
}

// Inherited from AudioSource
const int RecordNode::getID()
{
    return 0;
}

// Inherited from AudioSource
bool RecordNode::hasEditor() const
{
    return true;
}

// Inherited from AudioSource
AudioProcessorEditor* RecordNode::createEditor()
{
    return nullptr;
}