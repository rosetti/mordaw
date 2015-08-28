/*
  ==============================================================================

    ExportProcessor.cpp
    Created: 28 Aug 2015 12:24:23am
    Author:  dtl

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

ExportProcessor::~ExportProcessor()
{
    stopExporting();
}

void ExportProcessor::startExporting(const File& file)
{
    if(_sampleRate > 0)
    {
        file.deleteFile();
        ScopedPointer<FileOutputStream> fileStream (file.createOutputStream());

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

void ExportProcessor::stopExporting()
{
    const ScopedLock sl (getCallbackLock());
    _activeWriter = nullptr;
    _threadedWriter = nullptr;

}

bool ExportProcessor::isExporting()
{
    return _activeWriter != nullptr;

}

int ExportProcessor::getNumParameters()
{
    return 3;
}

float ExportProcessor::getParameter(int index)
{
     return 0.0f;
}

void ExportProcessor::setParameter(int index, float newValue)
{
}

const String ExportProcessor::getParameterName(int index)
{
    
}

const String ExportProcessor::getParameterText(int index)
{

}

const String ExportProcessor::getInputChannelName(int channelIndex) const
{
    return String(channelIndex + 1);
}

const String ExportProcessor::getOutputChannelName(int channelIndex) const
{
    return String(channelIndex + 1);
}

bool ExportProcessor::isInputChannelStereoPair(int) const
{
    return true;
}

bool ExportProcessor::isOutputChannelStereoPair(int) const
{
    return true;
}

bool ExportProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool ExportProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool ExportProcessor::silenceInProducesSilenceOut() const
{
    return false;
}


//==============================================================================
void ExportProcessor::prepareToPlay(double, int)
{
    _sampleRate = getSampleRate();
}

void ExportProcessor::releaseResources()
{
    _sampleRate = 0;
}

void ExportProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer&)
{
    if(_activeWriter != 0)
        _activeWriter->write((const float**)buffer.getArrayOfReadPointers(), buffer.getNumSamples());
}

void ExportProcessor::getStateInformation(MemoryBlock&)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ExportProcessor::setStateInformation(const void*, int)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

const void ExportProcessor::setID(int ident)
{
}

const int ExportProcessor::getID()
{
}

bool ExportProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* ExportProcessor::createEditor()
{
    return nullptr;
}