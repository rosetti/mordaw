/*
 ==============================================================================
 
 EQNode.cpp
 Created: 29 Oct 2015 6:25:49pm
 Author:  dtl
 
 ==============================================================================
 */

#include "EQNode.h"

EQNode::EQNode()
{
    
}

EQNode::~EQNode()
{
}

//==============================================================================
void EQNode::prepareToPlay (double newSampleRate, int /*samplesPerBlock*/)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void EQNode::releaseResources()
{
    
}

void EQNode::reset()
{
    
}

void EQNode::calculateCoefficients(float freq, float gain, float q)
{
    float a, omega, alpha, cos_omega, sin_omega;
    
    a = pow(10.0f, gain / 40.0f);
    omega = (2 * M_PI * freq) / getSampleRate();
    cos_omega = cos(omega);
    sin_omega = sin(omega);
    alpha = sin_omega / (2.0f * q);
    
    _b0 = 1 + (alpha * a);
    _b1 = -2 * cos_omega;
    _b2 = 1 - (alpha * a);
    _a0 = 1 + (alpha / static_cast<float>(a));
    _a1 = -2 * cos_omega;
    _a2 = 1 - (alpha / static_cast<float>(a));
}

void EQNode::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    float in_samp_m1, in_samp_m2;
    float out_samp_m1, out_samp_m2;
    for(int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        for(int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            float in_samp = buffer.getSample(channel, sample);
            float out_samp = (_b0 * in_samp + _b1 * in_samp_m1 + _b2 * in_samp_m2 - _a1 * out_samp_m1 - _a2 * out_samp_m2) / _a0;
            
            in_samp_m2 = in_samp_m1;
            in_samp_m1 = in_samp;
            out_samp_m2 = out_samp_m1;
            out_samp_m1 = out_samp;
            buffer.setSample(channel, sample, out_samp);
        }
    }
}

//==============================================================================
AudioProcessorEditor* EQNode::createEditor()
{
    return nullptr;
}

//==============================================================================
void EQNode::getStateInformation (MemoryBlock& destData)
{
    
}

void EQNode::setStateInformation (const void* data, int sizeInBytes)
{
    
}

const String EQNode::getInputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

const String EQNode::getOutputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

bool EQNode::isInputChannelStereoPair (int /*index*/) const
{
    return true;
}

bool EQNode::isOutputChannelStereoPair (int /*index*/) const
{
    return true;
}

bool EQNode::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool EQNode::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool EQNode::silenceInProducesSilenceOut() const
{
    return false;
}

double EQNode::getTailLengthSeconds() const
{
    return 0.0;
}