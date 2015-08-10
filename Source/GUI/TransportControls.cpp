/*
  ==============================================================================

    TransportControls.cpp
    Created: 4 Aug 2015 8:02:29pm
    Author:  dtl

  ==============================================================================
*/

#include "../Resources/Images/TransportImages.h"
#include "../../JuceLibraryCode/JuceHeader.h"
#include "TransportControls.h"

//==============================================================================
TransportControls::TransportControls(const ApplicationCommandManager &commands) : _commands(commands), _isPlaying(false), _isRecording(false)
{
    _totalLength = 750000;
    resetTimecode(44100.0f);
    timerAmount = 60;
    drawButtons();
    addListeners();
}

TransportControls::~TransportControls()
{
}

void TransportControls::drawButtons()
{
    _startButton = new ImageButton("Start");
    _rewindButton = new ImageButton("Rewind");
    _forwardButton = new ImageButton("Forward");
    _recordButton = new ImageButton(("Record"));
    setButtonImages();
    addAndMakeVisible(_startButton);
    addAndMakeVisible(_rewindButton);
    addAndMakeVisible(_forwardButton);
    addAndMakeVisible(_recordButton);
}


void TransportControls::setButtonImages()
{
    _image = ImageCache::getFromMemory(TransportImages::rewind_png, TransportImages::rewind_pngSize);
    _rewindButton->setImages(false, true, true, _image, 0.5f, Colours::transparentBlack, _image, 0.7f, Colours::transparentWhite, _image, 1.0f, Colours::transparentWhite);
    _image = ImageCache::getFromMemory(TransportImages::play_png, TransportImages::play_pngSize);
    _startButton->setImages(false, true, true, _image, 0.5f, Colours::transparentBlack, _image, 0.7f, Colours::transparentWhite, _image, 1.0f, Colours::transparentWhite);
    _image = ImageCache::getFromMemory(TransportImages::record_png, TransportImages::record_pngSize);
    _recordButton->setImages(false, true, true, _image, 0.5f, Colours::transparentBlack, _image, 0.7f, Colours::transparentWhite, _image, 1.0f, Colours::transparentWhite);
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

void TransportControls::addListeners()
{
    _startButton->addListener(this);
    _rewindButton->addListener(this);
    _forwardButton->addListener(this);
    _recordButton->addListener(this);
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
    else if(button == _recordButton)
    {
        _isRecording = !_isRecording;
        if(_isRecording)
        {
            _image = ImageCache::getFromMemory(TransportImages::record_enabled_png, TransportImages::record_enabled_pngSize);
            _recordButton->setImages(false, true, true, _image, 1.0f, Colours::transparentBlack, _image, 0.7f, Colours::transparentWhite, _image, 0.5f, Colours::transparentWhite);
        }
        else if(!_isRecording)
        {
            _image = ImageCache::getFromMemory(TransportImages::record_png, TransportImages::record_pngSize);
            _recordButton->setImages(false, true, true, _image, 1.0f, Colours::transparentBlack, _image, 0.7f, Colours::transparentWhite, _image, 0.5f, Colours::transparentWhite);
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
        repaint();
    }
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
    g.fillAll (Colours::white);   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    g.setColour(Colours::black);
    g.fillRect(getLocalBounds());

    g.setColour (Colours::green);
    g.setFont(20.0f);
    if(_isRecording)
        g.setColour(Colours::red);
    if(getWidth() < 275)
    {}
    else if(getWidth() < 475)
        g.drawText (_currentTimeCode,  getLocalBounds(),
                Justification::right, true);
    
    else
        g.drawText (_currentTimeCode,  getLocalBounds(),
                    Justification::centred, true);
}

void TransportControls::resized()
{
    _rewindButton->setBounds(0, 0, 50, 50);
    _startButton->setBounds(50, 0, 50, 50);
    _recordButton->setBounds(100, 0, 50, 50);
    _forwardButton->setBounds(150, 0, 50, 50);
    if(getWidth() < 200)
    {
        _rewindButton->setBounds(0, 0, 0, 0);
        _startButton->setBounds(0, 0, 0, 0);
        _recordButton->setBounds(0, 0, 0, 0);
        _forwardButton->setBounds(0, 0, 0, 0);
    }
}
