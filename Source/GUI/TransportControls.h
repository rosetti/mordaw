/*
  ==============================================================================

    TransportControls.h
    Created: 4 Aug 2015 8:02:29pm
    Author:  Thomas

  ==============================================================================
*/

#ifndef TRANSPORTCONTROLS_H_INCLUDED
#define TRANSPORTCONTROLS_H_INCLUDED

#include "../../JuceLibraryCode/JuceHeader.h"
#include "Conversion.h"
//==============================================================================
/*
*/
class TransportControls    : public Component,
                             public Timer
{
public:
    TransportControls(const ApplicationCommandManager &commands);
    ~TransportControls();

    void timerCallback();
    void paint (Graphics&);
    void resized();

private:
    int timerAmount;
    double milliseconds;
    String currentTimeCode;
    const ApplicationCommandManager &_commands;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TransportControls)
};


#endif  // TRANSPORTCONTROLS_H_INCLUDED
