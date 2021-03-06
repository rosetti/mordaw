/*
  ==============================================================================

    Arrangement.h
    Created: 10 Aug 2015 10:58:25pm
    Author:  Matt & Dan

  ==============================================================================
*/

#ifndef ARRANGEMENT_H_INCLUDED
#define ARRANGEMENT_H_INCLUDED

#include "TrackComponent.h"
#include "../../JuceLibraryCode/JuceHeader.h"
#include "TimelineCursor.h"
#include "TimelineComponent.h"
#include "../Audio/Engine.h"

//==============================================================================
/*
*/
class Arrangement : public Component, ButtonListener {
public:
    Arrangement(ApplicationCommandManager &commands, const Audio::Engine &engine);
    ~Arrangement();

    void paint(Graphics&);
    void resized();
    void addTrack(Audio::Track* track);
	void removeTrack(int trackNumber);
	void addRegionToTrack(int _trackID);
    void setPixelsPerClip(int64 pixels);
	int64 getLongestTrackLength();
	int64 getPixelsPerClip();
	int64 getMixerOffset();

	std::vector<TrackComponent*>* getTrackMap();
    
    virtual void buttonClicked(Button* button) override;

	void mouseDown(const MouseEvent & e);
	void mouseDoubleClick(const MouseEvent &e);

private:
	int _trackNumber;
    int64 _mixerOffset, _pixelsPerClip;
	std::vector<TrackComponent *> _tracks;
    ScopedPointer<TimelineComponent> _timeline;
    ScopedPointer<TimelineCursor> _cursor;
    ScopedPointer<TextButton> _addTrackButton;
    ScopedPointer<TextButton> _zoomInButton, _zoomOutButton;
    ApplicationCommandManager &_commands;
    const Audio::Engine &_engine;
    Button::Listener *listener;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Arrangement)
};


#endif // ARRANGEMENT_H_INCLUDED

