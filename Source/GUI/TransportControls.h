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
#include "../Audio/Engine.h"
#include "../Audio/Recorder.h"

//==============================================================================
/*
*/
class TransportControls    : public Component,
                             public Timer,
                             public Button::Listener
{
public:
    TransportControls(ApplicationCommandManager &commands, const Audio::Engine &engine);
    ~TransportControls();
    
    void start();
    void stop();
    void pause();
    void record();
    void resetTimecode(double sampleRate);
    void setTotalLength(int64 samples);
    void setTimeCodePosition(int64 position);
    
    void timerCallback();
    void paint (Graphics&);
    void resized();
    
    void setButtonImages();
    void drawButtons();
    
    void addListeners();
    void addListener(TransportControls::Listener* listener);
    void removeListener(TransportControls::Listener* listener);
    virtual void buttonClicked(Button* button);
    void refresh();

private:
    ListenerList<TransportControls::Listener> listenerList;
    Image _image;
    ScopedPointer<ImageButton> _startButton, _rewindButton, _forwardButton, _recordButton, _stopButton;
    double _sampleRate;
    int64 timerAmount;
    double _milliseconds;
    String _currentTimeCode;
    bool _isPlaying, _isRecording;
    int64 _totalLength;
    
    ApplicationCommandManager &_commands;
    const Audio::Engine &_engine;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TransportControls)
};


#endif  // TRANSPORTCONTROLS_H_INCLUDED
