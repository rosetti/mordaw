/*
  ==============================================================================

    TimelineComponent.cpp
    Created: 16 Aug 2015 8:26:16pm
    Author:  Dan

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "TimelineComponent.h"

//==============================================================================
/* The timeline contains a number of clips; each representing a second on the timeline */
TimelineComponent::TimelineComponent(const Audio::Engine &, int64 numberOfClips, int64 mixerOffset)
// construct a timeline with the default amount of 20px
: TimelineComponent(_engine, numberOfClips, 20, mixerOffset)
{
}

// construct a timeline specifying all parameters
TimelineComponent::TimelineComponent(const Audio::Engine &engine, int64 numberOfClips, int64 pixelsPerClip, int64 mixerOffset)
: _numberOfClips(numberOfClips), _pixelsPerClip(pixelsPerClip), _mixerOffset(mixerOffset), _engine(engine)
{
    // add cursor to the timeline
    addAndMakeVisible(_cursor = new TimelineCursor(_engine, _pixelsPerClip, _mixerOffset));
    addClips(numberOfClips);
    // force cursor always on top
    _cursor->setAlwaysOnTop(true);
}

// delete clips and clear from clips vector
TimelineComponent::~TimelineComponent()
{
    for(auto clip : _clips)
        delete clip;
    _clips.clear();
}

// repaint the specified number of clips
void TimelineComponent::setNumberOfClips(int64 numberOfClips)
{
    for (auto clip : _clips)
        delete clip;
    _clips.clear();
    addClips(numberOfClips);
    repaint();
}

// add clips to timeline a clips vector
void TimelineComponent::addClips(int64 numberOfClips) 
{
    auto pixels = _mixerOffset;
    for (auto clips = 0; clips < numberOfClips; ++clips)
    {
        auto clip = new TimelineClip(clips);
        addAndMakeVisible(clip);
        if (clips == 0)
            clip->setBounds(static_cast<int>(pixels), 0, static_cast<int>(_pixelsPerClip), 20);
        else
        {
            pixels += _pixelsPerClip;
            clip->setBounds(static_cast<int>(pixels), 0, static_cast<int>(_pixelsPerClip), 20);
        }
        _clips.push_back(clip);
    }
}

// get the current number of clips
int TimelineComponent::getNumberOfClips()
{
    return static_cast<int>(_numberOfClips);
}

// inherited from component, all painting is done in the sub component clip
void TimelineComponent::paint (Graphics&)
{
}

// set bounds
void TimelineComponent::resized()
{
    _cursor->setBounds((int)_mixerOffset ,0, getWidth() - (int)_mixerOffset, getHeight());
}
