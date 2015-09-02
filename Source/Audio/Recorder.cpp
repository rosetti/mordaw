/*
  ==============================================================================

    Recorder.cpp
    Created: 27 Aug 2015 3:19:52pm
    Author:  Dan

  ==============================================================================
*/

#include "Recorder.h"

/*
	Constructs a recorder object.
	*/
Audio::AudioRecorder::AudioRecorder ()
: backgroundThread ("Recorder Thread"),
sampleRate (0), nextSampleNum (0), activeWriter (nullptr)
{
    backgroundThread.startThread();
}

/*
	Stop playing on destruct
	*/
Audio::AudioRecorder::~AudioRecorder()
{
    stop();
}

/*
	Start recording to a specified file
	*/
void Audio::AudioRecorder::startRecording (const File& file)
{
    // make sure recorder is stopped first
    stop();
    if (sampleRate > 0)
    {
        // delete file contents first
        file.deleteFile();
        // create file stream for file
        ScopedPointer<FileOutputStream> fileStream (file.createOutputStream());
        
        if (fileStream != nullptr)
        {
            // create a wav format writer
            WavAudioFormat wavFormat;
            AudioFormatWriter* writer = wavFormat.createWriterFor (fileStream, sampleRate, 1, 16, StringPairArray(), 0);
            
            if (writer != nullptr)
            {
                fileStream.release();
                
                // create a thread for the writer.
                threadedWriter = new AudioFormatWriter::ThreadedWriter (writer, backgroundThread, 32768);
                
                // set the next sample number to start
                nextSampleNum = 0;
                
                // scoped lock for writer
                const ScopedLock sl (writerLock);
                activeWriter = threadedWriter;
            }
        }
    }
}

/*
	Stop recording to file
	*/
void Audio::AudioRecorder::stop()
{
    {
        const ScopedLock sl (writerLock);
        activeWriter = nullptr;
    }
    
    threadedWriter = nullptr;
}

/*
	Check to see if the recorder is recording
	*/
bool Audio::AudioRecorder::isRecording() const
{
    return activeWriter != nullptr;
}

/*
	Set the sample rate to 0 on device start up
	*/
void Audio::AudioRecorder::audioDeviceAboutToStart (AudioIODevice* device)
{
    sampleRate = device->getCurrentSampleRate();
}

/*
	When audio device is stopped set sample rate to 0
	*/
void Audio::AudioRecorder::audioDeviceStopped()
{
    sampleRate = 0;
}

/*
	Write the device output to the file.
	*/
void Audio::AudioRecorder::audioDeviceIOCallback(const float **inputChannelData, int, float **outputChannelData, int numOutputChannels, int numSamples)
{
    const ScopedLock sl (writerLock);
    
    if (activeWriter != nullptr)
    {
        activeWriter->write (inputChannelData, numSamples);
        const AudioSampleBuffer buffer (const_cast<float**> (inputChannelData), 1, numSamples);
        nextSampleNum += numSamples;
    }
    
    // clear buffers
    for (int i = 0; i < numOutputChannels; ++i)
        if (outputChannelData[i] != nullptr)
            FloatVectorOperations::clear (outputChannelData[i], numSamples);
}
