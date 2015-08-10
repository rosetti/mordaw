/*
  ==============================================================================

    TransportControls.cpp
    Created: 4 Aug 2015 8:02:29pm
    Author:  Thomas

  ==============================================================================
*/

#include "../Resources/Images/TransportImages.h"
#include "../../JuceLibraryCode/JuceHeader.h"
#include "TransportControls.h"

//==============================================================================
TransportControls::TransportControls(const ApplicationCommandManager &commands) : _commands(commands), _isPlaying(false)
{
    _totalLength = 750000;
    resetTimecode(44100.0f);
    timerAmount = 60;
    _startButton = new ImageButton("Start");
    _rewindButton = new ImageButton("Rewind");
    _forwardButton = new ImageButton("Forward");
    setButtonImages();
    addAndMakeVisible(_startButton);
    addAndMakeVisible(_rewindButton);
    addAndMakeVisible(_forwardButton);
    _startButton->addListener(this);
    _rewindButton->addListener(this);
    _forwardButton->addListener(this);
}

TransportControls::~TransportControls()
{
}

void TransportControls::setButtonImages()
{
    _image = ImageCache::getFromMemory(TransportImages::rewind_png, TransportImages::rewind_pngSize);
    _rewindButton->setImages(false, true, true, _image, 0.5f, Colours::transparentBlack, _image, 0.7f, Colours::transparentWhite, _image, 1.0f, Colours::transparentWhite);
    _image = ImageCache::getFromMemory(TransportImages::play_png, TransportImages::play_pngSize);
    _startButton->setImages(false, true, true, _image, 0.5f, Colours::transparentBlack, _image, 0.7f, Colours::transparentWhite, _image, 1.0f, Colours::transparentWhite);
    _image = ImageCache::getFromMemory(TransportImages::forward_png, TransportImages::forward_pngSize);
    _forwardButton->setImages(false, true, true, _image, 0.5f, Colours::transparentBlack, _image, 0.7f, Colours::transparentWhite, _image, 1.0f, Colours::transparentWhite);
}

void TransportControls::start()
{
    startTimer(timerAmount);
}

void TransportControls::stop()
{
    stopTimer();
}

void TransportControls::resetTimecode(double sampleRate)
{
    _milliseconds = 0;
    _currentTimeCode = samplesToTimeCode(0, sampleRate);
}

void TransportControls::addListener(TransportControls::Listener *listener)
{
    listenerList.add(listener);
}

void TransportControls::removeListener(TransportControls::Listener *listener)
{
    listenerList.remove(listener);
}

void TransportControls::buttonClicked(Button* button)
{
    if(button == _startButton)
    {
        _isPlaying = !_isPlaying;
        if(_isPlaying)
        {
            _image = ImageCache::getFromMemory(TransportImages::pause_png, TransportImages::pause_pngSize);
            _startButton->setImages(false, true, true, _image, 1.0f, Colours::transparentBlack, _image, 0.7f, Colours::transparentWhite, _image, 0.5f, Colours::transparentWhite);
            start();
        }
        else if(!_isPlaying)
        {
            _image = ImageCache::getFromMemory(TransportImages::play_png, TransportImages::play_pngSize);
            _startButton->setImages(false, true, true, _image, 1.0f, Colours::transparentBlack, _image, 0.7f, Colours::transparentWhite, _image, 0.5f, Colours::transparentWhite);
            stop();
        }
    }
    else if(button == _rewindButton)
    {
        if(_isPlaying)
        {
            stop();
            resetTimecode(44100.0);
            start();
        }
        else if(!_isPlaying)
        {
            resetTimecode(44100.0);
            repaint();
        }
    }
    else if(button == _forwardButton)
    {
        if(_isPlaying)
        {
            stop();
            setTimeCodePosition(_totalLength);
        }
        else if(!_isPlaying)
        {
            setTimeCodePosition(_totalLength);
        }
    }
    //repaint();
}

void TransportControls::setTimeCodePosition(int64 position)
{
    _currentTimeCode = samplesToTimeCode(position, 44100.0);
}

void TransportControls::setTotalLength(int64 samples)
{
    _totalLength = samples;
}

void TransportControls::timerCallback()
{
    if(millisecondsToSamples(_milliseconds, 44100.0) < _totalLength)
    {
        _milliseconds += timerAmount;
        int64 samples = millisecondsToSamples(_milliseconds, 44100.0);
        _currentTimeCode = samplesToTimeCode(samples, 44100.0);
        repaint();
    }
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
    g.drawText (_currentTimeCode, getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void TransportControls::resized()
{
    _rewindButton->setBounds(0, 0, 50, 50);
    _startButton->setBounds(50, 0, 50, 50);
    _forwardButton->setBounds(100, 0, 50, 50);

}
