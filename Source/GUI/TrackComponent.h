/*
  ==============================================================================

    TrackComponent.h
    Created: 12 Aug 2015 10:17:59pm
    Author: TrackMixerComponent	: Dan 
			TrackComponent		: Matt

  ==============================================================================
*/

#ifndef TRACKCOMPONENT_H_INCLUDED
#define TRACKCOMPONENT_H_INCLUDED

#include "../Audio/Track.h"
#include "../Audio/Engine.h"
#include "RegionComponent.h"


//==============================================================================
/* TrackMixerComponent is a sub component TrackComponent which holds mute and solo controls and the track label */
class TrackMixerComponent : public Component,
                            public ButtonListener
{
public:
    TrackMixerComponent(int trackID, const Audio::Engine& engine, ApplicationCommandManager& commands);
    ~TrackMixerComponent();
    
    // inherited from Component
    void paint (Graphics&);
    void resized();

    // track identifier methods
	int getTrackID();
	void setTrackID(int trackID);
    
    // inherited from ButtonListener
    void buttonClicked(Button* button);
    void buttonStateChanged(Button* button);
    
private:
    const Audio::Engine& _engine;
    int _trackID;
	ApplicationCommandManager& _commands;
    ScopedPointer<Label> _trackLabel;
    ScopedPointer<ToggleButton> _muteButton, _soloButton;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TrackMixerComponent)

};

/* The track component can contain a number of region components, regions can be added using drag and drop or with a menu */
class TrackComponent    : public Component, public FileDragAndDropTarget
{
public:
    TrackComponent(ApplicationCommandManager& commands, Audio::Track *track, int trackID, const Audio::Engine& engine, int64 pixelsPerClip);
    ~TrackComponent();

    // inherited from Component
    void paint (Graphics&);
	void resized();
    
    // create a region at the specified position
    void createRegionGUI(int64 posX, Audio::Region* region, AudioFormatManager& formatManager, File& audioFile);
    // drag and drop methods, inherited from FileDragAndDropTarget
    bool isInterestedInFileDrag(const StringArray& files) override;
    void filesDropped(const StringArray& files, int x, int y) override;
    
    // track information methods
	int getTrackID();
	void setTrackID(int trackID);
	int64 findTrackLength();
    
    // set number of clips, this shoudln't he called directly in this class.
    void setNumberofClips(int64 clips);

    // get the mixer offset in pixels
	int64 getMixerOffset();

    // return the map of regions
	std::map<int64, String> *getRegionMap();

    // set the number of pixels, should not be called directly in this class
    void setPixelsPerClip(int64 pixels);
    
    // mouse events
    void mouseDown (const MouseEvent& e) override;
    void mouseDrag (const MouseEvent& e) override;

private:
    int _trackID;
	int64 _trackLength;
    double _sampleRate;
    int64 _mixerOffset;
    int64 _pixelsPerClip;
    int64 _numberOfClips;
    Audio::Track *_track;
    ScopedPointer<TrackMixerComponent> _trackMixer;
    std::vector<RegionComponent *> _regionComponents;
    std::vector<int64> _posX, _sizeSamps;
	std::map< int64, String> _regions;
    const Audio::Engine& _engine;
    ApplicationCommandManager &_commands;
    
    void timerCallback();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TrackComponent)
};


#endif  // TRACKCOMPONENT_H_INCLUDED
