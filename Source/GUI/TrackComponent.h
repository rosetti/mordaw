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
class TrackMixerComponent : public Component
                            //public ButtonListener
{
public:
    TrackMixerComponent(const int trackID);
    ~TrackMixerComponent();
    
    void paint (Graphics&);
    void resized();
    
private:
    const int _trackID;
    ScopedPointer<Label> _trackLabel;
    ScopedPointer<ToggleButton> _muteButton, _soloButton;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TrackMixerComponent)

};

class TrackComponent    : public Component, public FileDragAndDropTarget
{
public:
    TrackComponent(ApplicationCommandManager &commands, Audio::Track *track, int trackID, double sampleRate);
    ~TrackComponent();

    void paint (Graphics&);
    void resized();
    void createRegionGUI(int64 posX, Audio::Region* region, AudioFormatManager& formatManager, File& audioFile);
    bool isInterestedInFileDrag(const StringArray& files) override;
    void filesDropped(const StringArray& files, int x, int y) override;
    

private:
    int _trackID;
    double _sampleRate;
    int64 _mixerOffset;
    Audio::Track *_track;
    ScopedPointer<TrackMixerComponent> _trackMixer;
    std::vector<RegionComponent *> _regions;
    std::vector<int64> _posX, _sizeSamps;
    ApplicationCommandManager &_commands;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TrackComponent)
};


#endif  // TRACKCOMPONENT_H_INCLUDED
