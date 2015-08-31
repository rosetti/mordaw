/*
  ==============================================================================

    MixerView.h
    Created: 10 Aug 2015 11:10:33pm
    Author:  Dan

  ==============================================================================
*/

#ifndef MIXERVIEW_H_INCLUDED
#define MIXERVIEW_H_INCLUDED

#include "../Audio/Engine.h"
#include "../../JuceLibraryCode/JuceHeader.h"
#include "ChannelStripComponent.h"

//==============================================================================
/*
*/
class MixerView    : public Component
{
public:
    MixerView(ApplicationCommandManager &commands, const Audio::Engine &engine);
    ~MixerView();

    void paint (Graphics&);
    void resized();
    
    void addTrack(int trackIndex);

	std::vector<ChannelStripComponent*> *getChannelStrips();

	void mouseDown(const MouseEvent & e);

private:
    const Audio::Engine &_engine;
    ApplicationCommandManager& _commands;
	std::vector<ChannelStripComponent*> _strips;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MixerView)
};


#endif  // MIXERVIEW_H_INCLUDED
