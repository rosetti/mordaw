/*
  ==============================================================================

    Recorder.h
    Created: 27 Aug 2015 3:19:52pm
    Author:  dtl

  ==============================================================================
*/

#ifndef RECORDER_H_INCLUDED
#define RECORDER_H_INCLUDED

#include "../../JuceLibraryCode/JuceHeader.h"

namespace Audio {
    
    class AudioRecorder  : public AudioIODeviceCallback
    {
    public:
        AudioRecorder();
        ~AudioRecorder();
  
        //==============================================================================
        void startRecording (const File& file);
        void stop();
        bool isRecording() const;
    
        //==============================================================================
        void audioDeviceAboutToStart (AudioIODevice* device) override;
        void audioDeviceStopped() override;
    
        void audioDeviceIOCallback (const float** inputChannelData, int /*numInputChannels*/,
                                float** outputChannelData, int numOutputChannels,
                                int numSamples) override;
    
    private:
        TimeSliceThread backgroundThread; // the thread that will write our audio data to disk
        ScopedPointer<AudioFormatWriter::ThreadedWriter> threadedWriter; // the FIFO used to buffer the incoming data
        double sampleRate;
        int64 nextSampleNum;
    
        CriticalSection writerLock;
        AudioFormatWriter::ThreadedWriter* volatile activeWriter;
};
}



#endif  // RECORDER_H_INCLUDED
