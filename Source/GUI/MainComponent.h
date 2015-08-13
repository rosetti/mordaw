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
    void addTrack(Audio::Track* track);
    TransportControls *getTransportControls();

    enum Commands {
        showArrangement = 0x500,
        showMixer = 0x501,
    };
    
    void getCommandInfo(CommandID commandID, ApplicationCommandInfo &result) const;
    void getAllCommands(Array<CommandID>& commands) const;
    bool perform(const ApplicationCommandTarget::InvocationInfo & info);
    
private:
    LeftSide _leftSide;
    Arrangement _arrangement;
    MixerView _mixerView;
    TransportControls _transportControls;
    
    ApplicationCommandManager &_commands;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
