/*
  ==============================================================================

    TimelineComponent.cpp
    Created: 16 Aug 2015 8:26:16pm
    Author:  dtl

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "TimelineComponent.h"

//==============================================================================
TimelineComponent::TimelineComponent(int64 numberOfClips, int64 mixerOffset)
: TimelineComponent(numberOfClips, 20, mixerOffset)
{
}

TimelineComponent::TimelineComponent(int64 numberOfClips, int64 pixelsPerClip, int64 mixerOffset)
: _numberOfClips(numberOfClips), _pixelsPerClip(pixelsPerClip), _mixerOffset(mixerOffset)
{
    addClips(numberOfClips);
}

TimelineComponent::~TimelineComponent()
{
    for(auto clip : _clips)
        delete clip;
    _clips.clear();
}

void TimelineComponent::setNumberOfClips(int64 numberOfClips)
{
    for (auto clip : _clips)
        delete clip;
    _clips.clear();
    addClips(numberOfClips);
    repaint();
}

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

int TimelineComponent::getNumberOfClips()
{
    return _numberOfClips;
}


void TimelineComponent::paint (Graphics&)
{
}

void TimelineComponent::resized()
{

}
