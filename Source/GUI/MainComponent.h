/*
  ==============================================================================

    MainComponent.h
    Created: 3 Aug 2015 10:18:09pm
    Author:  Thomas

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "LeftSide.h"
#include "Arrangement.h"
#include "MixerView.h"

//==============================================================================
/*
*/
class MainComponent : public Component
{
public:
    MainComponent(ApplicationCommandManager &manager, const Audio::Engine &engine);
    ~MainComponent();

    void paint(Graphics&) override;
    void resized() override;
	void mouseDown(const MouseEvent & e);
	void mouseDoubleClick(const MouseEvent & e);
    	
    void addTrack(Audio::Track* track);

    TransportControls *getTransportControls();
	Arrangement *getArrangement();
	MixerView *getMixer();

	bool arrangeIsShowing() const;
	bool mixerIsShowing() const;

	void switchView(bool arrangeEnabled);

private:
    int _trackCounter;
    LeftSide _leftSide;
	Viewport _arrangePort;
	Viewport _mixPort;
    Arrangement _arrangement;
    MixerView _mixerView;
    TransportControls _transportControls;
	AudioFormatManager _formatManager;
    AudioPluginFormatManager _pluginManager;
    ApplicationCommandManager &_commands;

	bool _arrangeVisible;
	bool _mixerVisible;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
