/*
  ==============================================================================

    TimelineCursor.cpp
    Created: 13 Aug 2015 10:14:54am
    Author:  Dan

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "TimelineCursor.h"
#include "../Utility/Conversion.h"

//==============================================================================
TimelineCursor::TimelineCursor(const Audio::Engine &engine, int64 pixelsPerClip, int64)
: _engine(engine), _pixelsPerClip(pixelsPerClip)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
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
        _positionInSamples = ((int64)_currentXCoords /_pixelsPerClip) * (int64)_engine.getCurrentSamplerate();
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
        _positionInSamples = ((int64)_currentXCoords /_pixelsPerClip) * (int64)_engine.getCurrentSamplerate();
        repaint();
    }
}

void TimelineCursor::timerCallback()
{
    repaint(_currentXCoords, 0, 5, getHeight());
}

void TimelineCursor::setCursorVisability(bool displayCursor)
{
    _showCursor = displayCursor;
}

void TimelineCursor::resized()
{
    _cursor = Image(Image::RGB, 3, jmax(10, getHeight()), true);
    Graphics g(_cursor);
    g.fillAll(Colours::black);
    g.setColour(Colours::grey);
    g.drawVerticalLine(1, 0.0f, (float) _cursor.getHeight());
}
