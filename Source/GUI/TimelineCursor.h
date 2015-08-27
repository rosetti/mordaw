/*
  ==============================================================================

    TimelineCursor.h
    Created: 13 Aug 2015 10:14:54am
    Author:  dtl

  ==============================================================================
*/

#ifndef TIMELINECURSOR_H_INCLUDED
#define TIMELINECURSOR_H_INCLUDED

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../Audio/Engine.h"

//==============================================================================
/*
*/
class TimelineCursor    : public Component,
                          public Timer

{
public:
    TimelineCursor(const Audio::Engine &engine, int64 pixelsPerClip, int64 mixerOffset);
    ~TimelineCursor();
    
    void setZoomRatio(double zoomRatio);
    void setStartOffsetRatio(double startOffset);
    void setCursorVisability(bool displayCursor);
    void positionChanged(int64 position);
    void updateCursorPosition();

    void paint (Graphics&);
    void resized();
    void timerCallback();
    
    void mouseDown(const MouseEvent &e);
    void mouseUp(const MouseEvent &e);
    void mouseDrag(const MouseEvent &e);

private:
    int64 _lengthSamples;
    int64 _mixerOffset;
    int64 _pixelsPerClip;
    int64 _positionInSamples;
    bool _showCursor, _stopTimer;
    Image _cursor;
    int _currentXCoords;
    const Audio::Engine& _engine;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TimelineCursor)
};


#endif  // TIMELINECURSOR_H_INCLUDED
