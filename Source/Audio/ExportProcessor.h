/*
  ==============================================================================

    RecordProcessor.h
    Created: 27 Aug 2015 9:23:06pm
    Author:  dtl

  ==============================================================================
*/

#ifndef RECORDPROCESSOR_H_INCLUDED
#define RECORDPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class ExportProcessor : public AudioProcessor
{
public:
    ExportProcessor(double sampleRate);
    ~ExportProcessor();
    
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
    const String getProgramName (int) override { return "ExportProcessor";};
    void changeProgramName (int, const String&) {};
    
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool silenceInProducesSilenceOut() const override;
    
    void getStateInformation(MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;
    
    const void setID(int ident);
    const int getID();
    const String getName() const override { return "ExportProcessor";};
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    
    void writeFile(double sampleRate);
    
private:
    std::vector<float> _output;
    double _sampleRate;
    int _currentChannels;
};



#endif  // RECORDPROCESSOR_H_INCLUDED
