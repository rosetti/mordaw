/*
  ==============================================================================

    TransportControls.h
    Created: 4 Aug 2015 8:02:29pm
    Author:  Thomas

  ==============================================================================
*/

#ifndef TRANSPORTCONTROLS_H_INCLUDED
#define TRANSPORTCONTROLS_H_INCLUDED

#include "../Resources/Images/TransportImages.h"
#include "../../JuceLibraryCode/JuceHeader.h"
#include "Conversion.h"
//==============================================================================
/*
*/
class TransportControls    : public Component,
                             public Timer,
                             public Button::Listener
{
public:
    TransportControls(const ApplicationCommandManager &commands);
    ~TransportControls();
    
    void start();
    void stop();
    void pause();

    void timerCallback();
    void paint (Graphics&);
    void resized();
    
    
    void addListener(TransportControls::Listener* listener);
    void removeListener(TransportControls::Listener* listener);
    virtual void buttonClicked(Button* button);
    

private:
    ListenerList<TransportControls::Listener> listenerList;
    int timerAmount;
    double milliseconds;
    String currentTimeCode;
    const ApplicationCommandManager &_commands;
    ScopedPointer<ImageButton> startButton;
    Image image;
    bool _isPlaying;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TransportControls)
};


#endif  // TRANSPORTCONTROLS_H_INCLUDED
