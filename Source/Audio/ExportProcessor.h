/*
  ==============================================================================

    ExportProcessor.h
    Created: 28 Aug 2015 12:24:23am
    Author:  dtl

  ==============================================================================
*/

#ifndef EXPORTPROCESSOR_H_INCLUDED
#define EXPORTPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
class ExportProcessor : public AudioProcessor
{
public:
    ExportProcessor();
    ~ExportProcessor();
    
    void startExporting(const File& file);
    void stopExporting();
    bool isExporting();
    
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    
    void processBlock(AudioSampleBuffer&, MidiBuffer&) override;
    
    int getNumParameters() override;
    float getParameter(int index) override;
    void setParameter(int index, float newValue) override;
    void setMuteParameter();
    const String getParameterName(int index) override;
    const String getParameterText(int index) override;
    
    const String getInputChannelName(int channelIndex) const override;
    const String getOutputChannelName(int channelIndex) const override;
    bool isInputChannelStereoPair(int index) const override;
    bool isOutputChannelStereoPair(int index) const override;
    
    double getTailLengthSeconds() const override { return 0.0f;};
    
    int getNumPrograms() override { return 0;};
    int getCurrentProgram() override { return 0;};
    void setCurrentProgram (int) override {};
    const String getProgramName (int) override { return "ChannelStrip";};
    void changeProgramName (int, const String&) {};
    
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool silenceInProducesSilenceOut() const override;
    
    void getStateInformation(MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;
    
    const void setID(int ident);
    const int getID();
    const String getName() const override { return "Channel Strip";};
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    
private:
    TimeSliceThread backgroundThread; 
    ScopedPointer<AudioFormatWriter::ThreadedWriter> _threadedWriter;
    AudioFormatWriter::ThreadedWriter* volatile _activeWriter;
    double _sampleRate;
    int64 _nextSampleIndex;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ExportProcessor)
};

#endif  // EXPORTPROCESSOR_H_INCLUDED
