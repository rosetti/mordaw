/*
  ==============================================================================

    TrackComponent.h
    Created: 12 Aug 2015 10:17:59pm
    Author:  Thomas

  ==============================================================================
*/

#ifndef TRACKCOMPONENT_H_INCLUDED
#define TRACKCOMPONENT_H_INCLUDED

#include "../Audio/Track.h"
#include "RegionComponent.h"

//==============================================================================
/*
*/
class TrackComponent    : public Component
{
public:
    TrackComponent(Audio::Track *track);
    ~TrackComponent();

    void paint (Graphics&);
    void resized();
    void createRegionGUI(Audio::Region* region, AudioFormatManager& formatManager, File& audioFile);

private:
    Audio::Track *_track;
    std::vector<RegionComponent *> _regions;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TrackComponent)
};


#endif  // TRACKCOMPONENT_H_INCLUDED
