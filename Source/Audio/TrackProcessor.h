/*
  ==============================================================================

    TrackProcessor.h
    Created: 2 Aug 2015 4:19:11pm
    Author:  Thomas

  ==============================================================================
*/

#ifndef TRACKPROCESSOR_H_INCLUDED
#define TRACKPROCESSOR_H_INCLUDED

#include "Track.h"

namespace Audio
{
    class TrackProcessor : public AudioProcessor
    {
    public:
        TrackProcessor(Track *track, TimeSliceThread *thread);
        ~TrackProcessor();

        AudioTransportSource &getSource();
     
        // Inherited via AudioProcessor
        virtual const String getName() const override;
        virtual void prepareToPlay(double sampleRate, int estimatedSamplesPerBlock) override;
        virtual void releaseResources() override;
        virtual void processBlock(AudioSampleBuffer & buffer, MidiBuffer & midiMessages) override;
        virtual const String getInputChannelName(int channelIndex) const override;
        virtual const String getOutputChannelName(int channelIndex) const override;
        virtual bool isInputChannelStereoPair(int index) const override;
        virtual bool isOutputChannelStereoPair(int index) const override;
        virtual bool silenceInProducesSilenceOut() const override;
        virtual double getTailLengthSeconds() const override;
        virtual bool acceptsMidi() const override;
        virtual bool producesMidi() const override;
        virtual AudioProcessorEditor * createEditor() override;
        virtual bool hasEditor() const override;
        virtual int getNumPrograms() override;
        virtual int getCurrentProgram() override;
        virtual void setCurrentProgram(int index) override;
        virtual const String getProgramName(int index) override;
        virtual void changeProgramName(int index, const String & newName) override;
        virtual void getStateInformation(juce::MemoryBlock & destData) override;
        virtual void setStateInformation(const void * data, int sizeInBytes) override;
   
    private:
        ScopedPointer<Track> _track;
        AudioTransportSource _source;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackProcessor)
    };
}


#endif  // TRACKPROCESSOR_H_INCLUDED
