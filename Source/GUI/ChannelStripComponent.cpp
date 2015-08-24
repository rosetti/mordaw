/*
  ==============================================================================

    ChannelStripComponent.cpp
    Created: 21 Jul 2015 11:43:55am
    Author:  dtl

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include <list>
#include "ChannelStripComponent.h"
#include "../Audio/ChannelStripProcessor.h"


//==============================================================================
ChannelStripComponent::ChannelStripComponent(int trackID, const Audio::Engine &engine) : _trackID(trackID), _engine(engine)
{
    // just for testing purposes
    addAndMakeVisible(label = new Label(String::empty, String::empty));
    label->setFont(Font(11.0f, Font::FontStyleFlags::plain));
    label->setJustificationType(Justification::centred);
    label->setEditable(false, true);
    label->addListener(this);
    
    // some method should be used to return the name of a track
    String trackLabel = "Track" + (String) trackID;
    label->setText(trackLabel, NotificationType::sendNotification);
    
    addAndMakeVisible(volumeSlider = new Slider(trackLabel + " v"));
    volumeSlider->setSliderStyle(Slider::SliderStyle::LinearVertical);
    volumeSlider->setRange(0.0f, 1.0f);
    volumeSlider->setSkewFactor(0.5f);
    volumeSlider->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
    volumeSlider->setValue(0.7f);
    volumeSlider->addListener(this);
    
    addAndMakeVisible(panPot = new Slider(trackLabel + " p"));
    panPot->setRange(0.0f, 1.0f);
    panPot->setSliderStyle(Slider::RotaryVerticalDrag);
    panPot->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
    panPot->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffff));
    panPot->setColour(Slider::rotarySliderOutlineColourId, Colour(0x8cffff));
    panPot->setValue(0.5f);
    panPot->addListener(this);
    
    addAndMakeVisible(muteButton = new ToggleButton("Mute"));
    muteButton->setColour(TextButton::buttonColourId, Colours::blue);
    muteButton->addListener(this);
    
    addAndMakeVisible(soloButton = new ToggleButton("Solo"));
    soloButton->setColour(TextButton::buttonColourId, Colours::yellow);
    soloButton->addListener(this);
    
    KnownPluginList& plugins = _engine.getMixer()->getKnownPluginList();

    
    addAndMakeVisible(plugins1 = new TextButton("Plugins 1"));
    plugins1->addListener(this);
    
    addAndMakeVisible(plugins2 = new TextButton("Plugins 2"));
    plugins2->addListener(this);
    
    addAndMakeVisible(plugins3 = new TextButton("Plugins 3"));
    plugins3->addListener(this);
    
    addAndMakeVisible(plugins4 = new TextButton("Plugins 4"));
    plugins4->addListener(this);
}

ChannelStripComponent::~ChannelStripComponent()
{
	volumeSlider->removeListener(this);
	panPot->removeListener(this);
	muteButton->removeListener(this);
	soloButton->removeListener(this);

	deleteAllChildren();
}

void ChannelStripComponent::paint (Graphics& g)
{
    Colour backgroundColour = Colours::transparentBlack;
    
    g.setGradientFill(ColourGradient(backgroundColour.brighter(0.25f), 0.0f, 0.0f, backgroundColour.darker(0.25f), 0.0f, (float) getHeight(), 0));
    
    g.fillAll ();   // clear the background
    
    g.drawLine(0.0f, 0.0f, (float)getWidth(), (float)getHeight());

}

void ChannelStripComponent::resized()
{
    int panSize = 32;
    int volumeWidth = 15;
    int volumeHeight = 127;
    //int meterWidth = 6;
    int labelHeight = 14;
    
    int remainingWidth = (getWidth() - volumeWidth) / 2;
    //int offsetY = (getHeight() - volumeHeight - panSize - labelHeight);
    //int totalHeight = volumeHeight - 7;
    
    int buttonSize = 16;
    int buttonsOffset = (getHeight() - volumeHeight - panSize - labelHeight) - 5;
    
    muteButton->setBounds(0, buttonsOffset - buttonSize * 3, getWidth(), buttonSize);
    soloButton->setBounds(0, buttonsOffset - buttonSize * 2, getWidth(), buttonSize);
    
    panPot->setBounds((getWidth() - panSize) / 2, (getHeight() - volumeHeight - panSize - labelHeight), panSize, panSize);
    
    volumeSlider->setBounds(remainingWidth, (getHeight() - volumeHeight - labelHeight), volumeWidth, volumeHeight);
    
    label->setBounds(0, getHeight() - labelHeight, getWidth(), labelHeight);
    
    plugins1->setBounds(0, buttonsOffset - buttonSize * 8, getWidth(), buttonSize);
    plugins2->setBounds(0, buttonsOffset - buttonSize * 7, getWidth(), buttonSize);
    plugins3->setBounds(0, buttonsOffset - buttonSize * 6, getWidth(), buttonSize);
    plugins4->setBounds(0, buttonsOffset - buttonSize * 5, getWidth(), buttonSize);

}

void ChannelStripComponent::sliderValueChanged(Slider* movedSlider)
{
    if(movedSlider == volumeSlider)
    {
        _engine.getMixer()->changeGain(ChannelStripProcessor::GAIN, (float)movedSlider->getValue());
    }
    else if(movedSlider == panPot)
    {
        _engine.getMixer()->changeGain(ChannelStripProcessor::PAN, (float)movedSlider->getValue());
    }
}

void ChannelStripComponent::labelTextChanged(Label*)
{
    // change this to something sensible
    //label->setText("This text has been changed", NotificationType::dontSendNotification);
}

bool ChannelStripComponent::getButtonState(String button)
{
	if (!button.compare("mute")) {
		return muteButton->getToggleState();
	}
	else if (!button.compare("solo")) {
		return soloButton->getToggleState();
	}
	return false;
}

float ChannelStripComponent::getSliderValue(String slider)
{
	if (!slider.compare("gain")) {
		return (float)volumeSlider->getValue();
	}
	if (!slider.compare("panning")) {
		return (float)panPot->getValue();
	}
	return 0.0f;
}


void ChannelStripComponent::buttonStateChanged(Button*)
{
    /*if(clickedButton == muteButton)
        _engine.getMixer()->muteTrack(_trackID);
    else if(clickedButton == soloButton)
        _engine.getMixer()->soloTrack(_trackID);*/
}

void ChannelStripComponent::buttonClicked(Button* clickedButton)
{
    if(clickedButton == muteButton)
        _engine.getMixer()->muteTrack(_trackID);
    else if(clickedButton == soloButton)
        _engine.getMixer()->soloTrack(_trackID);
    else if(clickedButton == plugins1)
    {
        KnownPluginList& pluginsList = _engine.getMixer()->getKnownPluginList();
        pluginsList.addToMenu(plugins, KnownPluginList::sortAlphabetically);
        const int index = plugins.show();
        const int pluginIndex = pluginsList.getIndexChosenByMenu(index);
        PluginDescription* desc = pluginsList.getType(pluginIndex);
        _engine.getMixer()->addPostFaderPlugin(_trackID, desc, 0, 0);
        
    }
}

