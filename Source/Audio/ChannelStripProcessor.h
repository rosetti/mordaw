/*
  ==============================================================================

    GainProcessor.h
    Created: 18 Jul 2015 11:24:01am
    Author:  Matt

  ==============================================================================
*/

#ifndef GAINPROCESSOR_H_INCLUDED
#define GAINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
class ChannelStripProcessor : public AudioProcessor
{
public:
	ChannelStripProcessor();
	~ChannelStripProcessor();
    
    enum StripParameter
    {
        GAIN = 1,
        PAN = 2,
        MUTE = 3
    };

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
    const String getName() const override { return "Channel Strip" + (String) _id;};
	AudioProcessorEditor* createEditor() override;
	bool hasEditor() const override;
    
private:
    float _gain;
    float _panning;
    float _muteGain;
    bool _muted;
    int _id;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ChannelStripProcessor)
};

#endif  // GAINPROCESSOR_H_INCLUDED