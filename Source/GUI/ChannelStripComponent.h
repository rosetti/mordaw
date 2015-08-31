/*
  ==============================================================================

    ChannelStripComponent.h
    Created: 21 Jul 2015 11:43:55am
    Author:  Dan

  ==============================================================================
*/

#ifndef CHANNELSTRIPCOMPONENT_H_INCLUDED
#define CHANNELSTRIPCOMPONENT_H_INCLUDED

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../Audio/Engine.h"

//==============================================================================
/*
*/
class ChannelStripComponent    : public Component,
                                 public SliderListener,
                                 public ButtonListener,
                                 public LabelListener
{
public:
    ChannelStripComponent(ApplicationCommandManager &commands, int trackID, const Audio::Engine &engine);
    ~ChannelStripComponent();
    
    void sliderValueChanged(Slider* movedSlider) override;
    void buttonClicked(Button* clickedButton) override;    
    void buttonStateChanged(Button* clickedButton) override;
    void labelTextChanged(Label* changedLabel) override;

	void mouseDown(const MouseEvent & e);

	bool getButtonState(String button);
	void setButtonState(String button, bool buttonState);
	float getSliderValue(String slider);
	void setSliderValue(String slider, double sliderValue);

	PopupMenu getPluginsMenu();
    
    void paint(Graphics& g);
    void resized();
    
private:
    Label* label;
    Slider* volumeSlider;
    Slider* panPot;
    
    ToggleButton* muteButton;
    ToggleButton* soloButton;
    
	TextButton* _pluginsButton;
	TextButton* plugins1,* plugins2,* plugins3,* plugins4,* plugins5,* plugins6,* plugins7,* plugins8;
    
    PopupMenu _plugins;

	ApplicationCommandManager& _commands;
    int _trackID;
    const Audio::Engine &_engine;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChannelStripComponent)

};


#endif  // CHANNELSTRIPCOMPONENT_H_INCLUDED

