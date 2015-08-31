/*
  ==============================================================================

    Recorder.cpp
    Created: 27 Aug 2015 3:19:52pm
    Author:  Dan

  ==============================================================================
*/

#include "Recorder.h"

Audio::AudioRecorder::AudioRecorder ()
: backgroundThread ("Recorder Thread"),
sampleRate (0), nextSampleNum (0), activeWriter (nullptr)
{
    backgroundThread.startThread();
}

Audio::AudioRecorder::~AudioRecorder()
{
    stop();
}

void Audio::AudioRecorder::startRecording (const File& file)
{
    stop();
    
    if (sampleRate > 0)
    {
        // Create an OutputStream to write to our destination file...
        file.deleteFile();
        ScopedPointer<FileOutputStream> fileStream (file.createOutputStream());
        
        if (fileStream != nullptr)
        {
            // Now create a WAV writer object that writes to our output stream...
            WavAudioFormat wavFormat;
            AudioFormatWriter* writer = wavFormat.createWriterFor (fileStream, sampleRate, 1, 16, StringPairArray(), 0);
            
            if (writer != nullptr)
            {
                fileStream.release(); // (passes responsibility for deleting the stream to the writer object that is now using it)
                
                // Now we'll create one of these helper objects which will act as a FIFO buffer, and will
                // write the data to disk on our background thread.
                threadedWriter = new AudioFormatWriter::ThreadedWriter (writer, backgroundThread, 32768);
                
                nextSampleNum = 0;
                
                // And now, swap over our active writer pointer so that the audio callback will start using it..
                const ScopedLock sl (writerLock);
                activeWriter = threadedWriter;
            }
        }
    }
}

void Audio::AudioRecorder::stop()
{
    {
        const ScopedLock sl (writerLock);
        activeWriter = nullptr;
    }
    
    threadedWriter = nullptr;
}

bool Audio::AudioRecorder::isRecording() const
{
    return activeWriter != nullptr;
}

void Audio::AudioRecorder::audioDeviceAboutToStart (AudioIODevice* device)
{
    sampleRate = device->getCurrentSampleRate();
}

void Audio::AudioRecorder::audioDeviceStopped()
{
    sampleRate = 0;
}

void Audio::AudioRecorder::audioDeviceIOCallback(const float **inputChannelData, int, float **outputChannelData, int numOutputChannels, int numSamples)
{
    const ScopedLock sl (writerLock);
    
    if (activeWriter != nullptr)
    {
        activeWriter->write (inputChannelData, numSamples);
        
        // wrap our incomming data, note that this does no allocations or copies, it simply references our input data
        const AudioSampleBuffer buffer (const_cast<float**> (inputChannelData), 1, numSamples);
        nextSampleNum += numSamples;
    }
    
    // We need to clear the output buffers, in case they're full of junk..
    for (int i = 0; i < numOutputChannels; ++i)
        if (outputChannelData[i] != nullptr)
            FloatVectorOperations::clear (outputChannelData[i], numSamples);
}
