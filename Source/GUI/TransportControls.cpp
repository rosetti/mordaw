/*
  ==============================================================================

    TransportControls.cpp
    Created: 4 Aug 2015 8:02:29pm
    Author:  Dan

  ==============================================================================
*/

#include "../Resources/Images/TransportImages.h"
#include "../Utility/Conversion.h"
#include "TransportControls.h"

//==============================================================================
TransportControls::TransportControls(ApplicationCommandManager &commands, const Audio::Engine &engine) :
    _engine(engine),
    _commands(commands), 
    _isPlaying(false), 
    _isRecording(false)
{
    _totalLength = secondsToSamples(100.0f, _engine.getCurrentSamplerate());
    resetTimecode(_engine.getCurrentSamplerate());
    timerAmount = 60;
    drawButtons();
    addListeners();
}

TransportControls::~TransportControls()
{
    _recordButton.release();
    _stopButton.release();
    _startButton.release();
    _forwardButton.release();
    _rewindButton.release();
    deleteAllChildren();
}

// draw the buttons
void TransportControls::drawButtons()
{
    _startButton = new ImageButton("Start");
    _startButton->setCommandToTrigger(&_commands, _engine.start, true);
    _rewindButton = new ImageButton("Rewind");
    _rewindButton->setCommandToTrigger(&_commands, _engine.rewind, true);
    _forwardButton = new ImageButton("Forward");
    _forwardButton->setCommandToTrigger(&_commands, _engine.forward, true);
    _recordButton = new ImageButton(("Record"));
    _stopButton = new ImageButton(("Stop"));
    _stopButton->setCommandToTrigger(&_commands, _engine.stop, true);
    setButtonImages();
    addAndMakeVisible(_startButton);
    addAndMakeVisible(_rewindButton);
    addAndMakeVisible(_forwardButton);
    addAndMakeVisible(_recordButton);
    addAndMakeVisible(_stopButton);
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
    _image = ImageCache::getFromMemory(TransportImages::stop_png, TransportImages::stop_pngSize);
    _stopButton->setImages(false, true, true, _image, 0.5f, Colours::transparentBlack, _image, 0.7f, Colours::transparentWhite, _image, 1.0f, Colours::transparentWhite);
}

// stop timer
void TransportControls::start()
{
    startTimer((int)timerAmount);
}

// start timer
void TransportControls::stop()
{
    stopTimer();
}

// reset the timecode
void TransportControls::resetTimecode(double sampleRate)
{
    _milliseconds = 0;
    _currentTimeCode = samplesToTimeCode(0, sampleRate);
}

// add listeners to buttons
void TransportControls::addListeners()
{
    _startButton->addListener(this);
    _rewindButton->addListener(this);
    _forwardButton->addListener(this);
    _recordButton->addListener(this);
    _stopButton->addListener(this);
}

void TransportControls::addListener(TransportControls::Listener *listener)
{
    listenerList.add(listener);
}

void TransportControls::removeListener(TransportControls::Listener *listener)
{
    listenerList.remove(listener);
}

// button listeners
void TransportControls::buttonClicked(Button* button)
{
     if(button == _recordButton)
    {
        _isRecording = !_isRecording;
        if(_isRecording)
        {
            _image = ImageCache::getFromMemory(TransportImages::record_enabled_png, TransportImages::record_enabled_pngSize);
            _recordButton->setImages(false, true, true, _image, 1.0f, Colours::transparentBlack, _image, 0.7f, Colours::transparentWhite, _image, 0.5f, Colours::transparentWhite);
            _commands.invokeDirectly(Audio::Engine::record, true);
        }
        else if(!_isRecording)
        {
            _image = ImageCache::getFromMemory(TransportImages::record_png, TransportImages::record_pngSize);
            _recordButton->setImages(false, true, true, _image, 1.0f, Colours::transparentBlack, _image, 0.7f, Colours::transparentWhite, _image, 0.5f, Colours::transparentWhite);
            _commands.invokeDirectly(Audio::Engine::record, true);
        }
    }
    else if(button == _rewindButton)
    {
        if(_isPlaying)
        {
            stop();
            resetTimecode(_engine.getCurrentSamplerate());
            start();
        }
        else if(!_isPlaying)
        {
            resetTimecode(_engine.getCurrentSamplerate());
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
    else if(button == _stopButton)
    {
        if(_isPlaying)
        {
            stop();
            setTimeCodePosition(0);
        }
        else if(!_isPlaying)
        {
            setTimeCodePosition(0);
        }
        resetTimecode(_engine.getCurrentSamplerate());
        repaint();
    }
}

// update the transport button images on refresh
void TransportControls::refresh() {
    auto isPlaying = _engine.getMixer()->isPlaying();

    if (isPlaying)
    {
        _image = ImageCache::getFromMemory(TransportImages::pause_png, TransportImages::pause_pngSize);
        _startButton->setImages(false, true, true, _image, 1.0f, Colours::transparentBlack, _image, 0.7f, Colours::transparentWhite, _image, 0.5f, Colours::transparentWhite);
        start();

        return;
    }

    _image = ImageCache::getFromMemory(TransportImages::play_png, TransportImages::play_pngSize);
    _startButton->setImages(false, true, true, _image, 1.0f, Colours::transparentBlack, _image, 0.7f, Colours::transparentWhite, _image, 0.5f, Colours::transparentWhite);
    stop();
}

// set the timecode position
void TransportControls::setTimeCodePosition(int64 position)
{
    _currentTimeCode = samplesToTimeCode(position, _engine.getCurrentSamplerate());
}

// set total length of timer
void TransportControls::setTotalLength(int64 samples)
{
    _totalLength = samples;
}

// timer callback for timecode display
void TransportControls::timerCallback()
{
    if(millisecondsToSamples(_milliseconds, _engine.getCurrentSamplerate()) < _totalLength)
    {
        _milliseconds += timerAmount;
        int64 samples = millisecondsToSamples(_milliseconds, _engine.getCurrentSamplerate());
        _currentTimeCode = samplesToTimeCode(samples, _engine.getCurrentSamplerate());
        repaint();
    }
    _totalLength = _engine.getMixer()->getLongestTrack();
}

// paint the component
void TransportControls::paint (Graphics& g)
{
    g.fillAll (Colours::white);   // clear the background

    g.setColour (Colours::steelblue);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    g.setColour(Colours::black);
    g.fillRect(getLocalBounds());

    g.setColour (Colours::green);
    g.setFont(20.0f);
    if(_isRecording)
        g.setColour(Colours::red);
    if(getWidth() < 275)
    {}
    else if(getWidth() < 550)
        g.drawText (_currentTimeCode,  getLocalBounds(),
                Justification::right, true);
    
    else
        g.drawText (_currentTimeCode,  getLocalBounds(),
                    Justification::centred, true);
}

// reset the bounds on resized
void TransportControls::resized()
{
    _rewindButton->setBounds(0, 0, 50, 50);
    _startButton->setBounds(50, 0, 50, 50);
    _recordButton->setBounds(100, 0, 50, 50);
    _forwardButton->setBounds(150, 0, 50, 50);
    _stopButton->setBounds(200, 0, 50, 50);
    if(getWidth() < 300)
    {
        _rewindButton->setBounds(0, 0, 0, 0);
        _startButton->setBounds(0, 0, 0, 0);
        _recordButton->setBounds(0, 0, 0, 0);
        _forwardButton->setBounds(0, 0, 0, 0);
    }
}
