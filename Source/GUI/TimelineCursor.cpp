/*
  ==============================================================================

    TimelineCursor.cpp
    Created: 13 Aug 2015 10:14:54am
    Author:  dtl

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "TimelineCursor.h"
#include "../Utility/Conversion.h"

//==============================================================================
TimelineCursor::TimelineCursor(const Audio::Engine &engine)
: _engine(engine)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    _lengthSamples = engine.getTotalLength();
    _showCursor = true;
}

TimelineCursor::~TimelineCursor()
{
    stopTimer();
}

void TimelineCursor::paint (Graphics& g)
{
    if(_showCursor)
        g.drawImageAt(_cursor, _currentXCoords, 0);
}

void TimelineCursor::mouseDown(const MouseEvent &e)
{
    if(_showCursor)
    {
        setMouseCursor(MouseCursor::IBeamCursor);
        _currentXCoords = e.x;
        _positionInSamples = pixelsToSamples(_currentXCoords, getWidth(), _lengthSamples);
        _stopTimer = false;
        startTimer(40);
        repaint();
    }
}

void TimelineCursor::mouseUp(const MouseEvent &)
{
    if(_showCursor)
    {
        setMouseCursor(MouseCursor::NormalCursor);
        if(_engine.getMixer()->isPlaying())
        {
            _engine.getMixer()->stop();
            _engine.getMixer()->startPlayingAt(_positionInSamples);
        }
        else
        {
            _engine.getMixer()->startPlayingAt(_positionInSamples);
            _engine.getMixer()->pause();
        }
    }
}

void TimelineCursor::mouseDrag(const MouseEvent &e)
{
    if(_showCursor)
    {
        _currentXCoords = e.x;
        _positionInSamples = pixelsToSamples(_currentXCoords, getWidth(), _lengthSamples);
        repaint();
    }
}

void TimelineCursor::timerCallback()
{
    repaint(_currentXCoords, 0, 5, getHeight());
    /*if(_stopTimer)
    {
        _stopTimer = false;
        stopTimer();
    }*/
    if(_engine.getMixer()->isPlaying())
    {
        
    }
}

void TimelineCursor::setCursorVisability(bool displayCursor)
{
    _showCursor = displayCursor;
}

void TimelineCursor::resized()
{
    _rangePixels = getWidth();
    _cursor = Image(Image::RGB, 3, jmax(10, getHeight()), true);
    Graphics g(_cursor);
    g.fillAll(Colours::black);
    g.setColour(Colours::grey);
    g.drawVerticalLine(1, 0.0f, (float) _cursor.getHeight());
}
