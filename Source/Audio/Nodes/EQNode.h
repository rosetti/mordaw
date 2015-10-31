/*
 ==============================================================================
 
 EQNode.h
 Created: 29 Oct 2015 6:25:49pm
 Author:  dtl
 
 ==============================================================================
 */

#ifndef EQNODE_H_INCLUDED
#define EQNODE_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================
/**
 As the name suggest, this class does the actual audio processing.
 */
class EQNode  : public AudioProcessor
{
public:
    //==============================================================================
    EQNode();
    ~EQNode();
    
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages) override;
    void reset() override;
    
    //==============================================================================
    bool hasEditor() const override                  { return true; }
    AudioProcessorEditor* createEditor() override;
    
    //==============================================================================
    const String getName() const override            { return "EQNode"; }
    
    const String getInputChannelName (int channelIndex) const override;
    const String getOutputChannelName (int channelIndex) const override;
    bool isInputChannelStereoPair (int index) const override;
    bool isOutputChannelStereoPair (int index) const override;
    
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool silenceInProducesSilenceOut() const override;
    double getTailLengthSeconds() const override;
    
    //==============================================================================
    int getNumPrograms() override                                               { return 1; }
    int getCurrentProgram() override                                            { return 0; }
    void setCurrentProgram (int /*index*/) override                             {}
    const String getProgramName (int /*index*/) override                        { return "Default"; }
    void changeProgramName (int /*index*/, const String& /*newName*/) override  {}
    
    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    void calculateCoefficients(float freq, float gain, float q);
    float calculateFrequency(float freq);
    float calculateGain(float gain);
    float calculateQFactor(float q);
    
    //==============================================================================
    
private:
    //==============================================================================
    AudioSampleBuffer _sampleBuffer;
    
    // processor parameters 0..1
    float _frequency;
    float _gain;
    float _qFactor;
    
    // values of each parameter in appropriate measures
    float _frequencyHz;
    float _gainDB;
    float _q;
    
    float _xn_1;
    float _xn_2;
    float _yn_1;
    float _yn_2;
    
    float _a0, _a1, _a2, _b0, _b1, _b2;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EQNode)
};

#endif  // EQNODE_H_INCLUDED
