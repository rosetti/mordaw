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
: _numberOfClips(numberOfClips), _pixelsPerClip(20)
{
    auto pixels = mixerOffset;
    for(auto clips = 0; clips < numberOfClips; ++clips)
    {
        TimelineClip* clip = new TimelineClip(clips);
        addAndMakeVisible(clip);
        if(clips == 0)
            clip->setBounds(pixels, 0, _pixelsPerClip, 20);
        else
        {
            pixels += _pixelsPerClip;
            clip->setBounds(pixels, 0, _pixelsPerClip, 20);
        }
        _clips.push_back(clip);
    }
}

TimelineComponent::TimelineComponent(int64 numberOfClips, int64 pixelsPerClip, int64 mixerOffset)
: _numberOfClips(numberOfClips), _pixelsPerClip(pixelsPerClip)
{
    auto pixels = mixerOffset;
    for(auto clips = 0; clips < numberOfClips; ++clips)
    {
        TimelineClip* clip = new TimelineClip(clips);
        addAndMakeVisible(clip);
        if(clips == 0)
            clip->setBounds(pixels, 0, _pixelsPerClip, 20);
        else
        {
            pixels += _pixelsPerClip;
            clip->setBounds(pixels, 0, _pixelsPerClip, 20);
        }
        _clips.push_back(clip);
    }
}

TimelineComponent::~TimelineComponent()
{
    for(auto clip : _clips)
        delete clip;
}

void TimelineComponent::paint (Graphics& g)
{
    g.fillAll (Colours::white);   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::lightblue);
}

void TimelineComponent::resized()
{

}
