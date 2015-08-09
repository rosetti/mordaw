/*
  ==============================================================================

    TransportControls.cpp
    Created: 4 Aug 2015 8:02:29pm
    Author:  Thomas

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "TransportControls.h"

//==============================================================================
TransportControls::TransportControls(const ApplicationCommandManager &commands) : _commands(commands)
{
    currentTimeCode = samplesToTimeCode(0, 44100.0);
    timerAmount = 60;
    startTimer(timerAmount);
}

TransportControls::~TransportControls()
{
}

void TransportControls::timerCallback()
{
    milliseconds += timerAmount;
    int64 samples = millisecondsToSamples(milliseconds, 44100.0);
    currentTimeCode = samplesToTimeCode(samples, 44100.0);
    repaint();
}

void TransportControls::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (Colours::white);   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::lightblue);
    g.setFont (14.0f);
    g.drawText (currentTimeCode, getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void TransportControls::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
