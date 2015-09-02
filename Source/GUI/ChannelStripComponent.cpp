/*
  ==============================================================================

    ChannelStripComponent.cpp
    Created: 21 Jul 2015 11:43:55am
    Author:  Dan

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "ChannelStripComponent.h"
#include "MainWindow.h"
#include "../Audio/ChannelStripProcessor.h"


//==============================================================================
ChannelStripComponent::ChannelStripComponent(ApplicationCommandManager &commands, int trackID, const Audio::Engine &engine) :
	_commands(commands),
	_trackID(trackID),
	_engine(engine)
{
    addAndMakeVisible(label = new Label(String::empty, String::empty));
    label->setFont(Font(11.0f, Font::FontStyleFlags::plain));
    label->setJustificationType(Justification::centred);
    label->setEditable(false, true);
    label->addListener(this);
    
    // some method should be used to return the name of a track
    auto trackLabel = "Track" + String(trackID);
    label->setText(trackLabel, NotificationType::sendNotification);
    
    addAndMakeVisible(volumeSlider = new Slider(trackLabel + " v"));
    volumeSlider->setSliderStyle(Slider::SliderStyle::LinearVertical);
    volumeSlider->setRange(0.0f, 1.0f);
    volumeSlider->setSkewFactor(0.5f);
    volumeSlider->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
    volumeSlider->setValue(0.7f);
	_engine.getMixer()->changeGain(ChannelStripProcessor::GAIN, static_cast<float>(volumeSlider->getValue()));
    volumeSlider->addListener(this);
    
    addAndMakeVisible(panPot = new Slider(trackLabel + " p"));
    panPot->setRange(0.0f, 1.0f);
    panPot->setSliderStyle(Slider::RotaryVerticalDrag);
    panPot->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
    panPot->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffff));
    panPot->setColour(Slider::rotarySliderOutlineColourId, Colour(0x8cffff));
    panPot->setValue(0.5f);
	_engine.getMixer()->changePan(ChannelStripProcessor::PAN, static_cast<float>(panPot->getValue()));
    panPot->addListener(this);
    
    addAndMakeVisible(muteButton = new ToggleButton("Mute"));
    muteButton->setColour(TextButton::buttonColourId, Colours::blue);
	setButtonState("Mute", false);
    muteButton->addListener(this);
    
	if (trackID != 0)
	{
		addAndMakeVisible(soloButton = new ToggleButton("Solo"));
		soloButton->setColour(TextButton::buttonColourId, Colours::yellow);
		setButtonState("Solo", false);
		soloButton->addListener(this);
	}
	addAndMakeVisible(_pluginsButton = new TextButton("Plugins"));
	_pluginsButton->addListener(this);

    addAndMakeVisible(plugins1 = new TextButton("Plugin 1"));
    plugins1->addListener(this);
    
    addAndMakeVisible(plugins2 = new TextButton("Plugin 2"));
    plugins2->addListener(this);
	plugins2->setEnabled(false);
    
    addAndMakeVisible(plugins3 = new TextButton("Plugin 3"));
    plugins3->addListener(this);
	plugins3->setEnabled(false);

    addAndMakeVisible(plugins4 = new TextButton("Plugin 4"));
    plugins4->addListener(this);
	plugins4->setEnabled(false);

	addAndMakeVisible(plugins5 = new TextButton("Plugin 1"));
	plugins5->addListener(this);

	addAndMakeVisible(plugins6 = new TextButton("Plugin 2"));
	plugins6->addListener(this);
	plugins6->setEnabled(false);

	addAndMakeVisible(plugins7 = new TextButton("Plugin 3"));
	plugins7->addListener(this);
	plugins7->setEnabled(false);

	addAndMakeVisible(plugins8 = new TextButton("Plugin 4"));
	plugins8->addListener(this);
	plugins8->setEnabled(false);
}

ChannelStripComponent::~ChannelStripComponent()
{
    removeAllChildren();

    label->removeListener(this);
    delete label;
    volumeSlider->removeListener(this);
    delete volumeSlider;
	panPot->removeListener(this);
    delete panPot;
	muteButton->removeListener(this);
    delete muteButton;
	if (_trackID != 0)
	{
		soloButton->removeListener(this);
		delete soloButton;
	}
	_pluginsButton->removeListener(this);
	delete _pluginsButton;

    plugins1->removeListener(this);
    delete plugins1;
    plugins2->removeListener(this);
    delete plugins2;
    plugins3->removeListener(this);
    delete plugins3;
    plugins4->removeListener(this);
    delete plugins4;
	plugins5->removeListener(this);
	delete plugins5;
	plugins6->removeListener(this);
	delete plugins6;
	plugins7->removeListener(this);
	delete plugins7;
	plugins8->removeListener(this);
	delete plugins8;
}

void ChannelStripComponent::paint (Graphics& g)
{
    Colour backgroundColour = Colours::transparentBlack;
    
    g.setGradientFill(ColourGradient(backgroundColour.brighter(0.25f), 0.0f, 0.0f, backgroundColour.darker(0.25f), 0.0f, static_cast<float>(getHeight()), 0));
    
    g.fillAll ();   // clear the background
    
    g.drawLine(0.0f, 0.0f, static_cast<float>(getWidth()), static_cast<float>(getHeight()));
	
	g.setColour(Colours::steelblue);
	g.drawRect (getLocalBounds(), 2);   // draw an outline around the component

	g.setColour(Colours::black);
	g.drawRect(getLocalBounds(), 1);   // draw an outline around the component
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
	
	_pluginsButton->setBounds(5, buttonsOffset - buttonSize * 5, getWidth() - 10, buttonSize);
	muteButton->setBounds(0, buttonsOffset - buttonSize * 3, getWidth(), buttonSize);

	if (_trackID != 0)
		soloButton->setBounds(0, buttonsOffset - buttonSize * 2, getWidth(), buttonSize);

    panPot->setBounds((getWidth() - panSize) / 2, (getHeight() - volumeHeight - panSize - labelHeight), panSize, panSize);
    volumeSlider->setBounds(remainingWidth, (getHeight() - volumeHeight - labelHeight - 3), volumeWidth, volumeHeight);  
    label->setBounds(0, getHeight() - labelHeight - 3, getWidth(), labelHeight);
}

void ChannelStripComponent::mouseDown(const MouseEvent &) {

	ModifierKeys modifiers = ModifierKeys::getCurrentModifiersRealtime();

	// check the mod keys ..
	if (modifiers.isPopupMenu() || modifiers.isCtrlDown())
	{
		ScopedPointer<PopupMenu> arrangeMenu_ = new PopupMenu();
		arrangeMenu_->clear();
		arrangeMenu_->addCommandItem(&_commands, MainWindow::showArrangement);
		arrangeMenu_->show();
	}
}

void ChannelStripComponent::sliderValueChanged(Slider* movedSlider)
{
    if(movedSlider == volumeSlider)
    {
        _engine.getMixer()->changeGain(ChannelStripProcessor::GAIN, static_cast<float>(movedSlider->getValue()));
    }
    else if(movedSlider == panPot)
    {
        _engine.getMixer()->changePan(ChannelStripProcessor::PAN, static_cast<float>(movedSlider->getValue()));
		
    }
}

void ChannelStripComponent::labelTextChanged(Label*)
{
    // change this to something sensible
    //label->setText("This text has been changed", NotificationType::dontSendNotification);
}

bool ChannelStripComponent::getButtonState(String button)
{
	if (!button.compare("Mute")) {
		return muteButton->getToggleState();
	}
	else if (!button.compare("Solo")) {
		return soloButton->getToggleState();
	}
	return false;
}

void ChannelStripComponent::setButtonState(String button, bool buttonState)
{
	if (!button.compare("Mute")) {
		muteButton->setToggleState(buttonState, sendNotificationAsync);
	}
	else if (!button.compare("Solo")) {
		soloButton->setToggleState(buttonState, sendNotificationAsync);
	}
}

float ChannelStripComponent::getSliderValue(String slider)
{
	if (!slider.compare("Gain")) {
		return static_cast<float>(volumeSlider->getValue());
	}
	if (!slider.compare("Panning")) {
		return static_cast<float>(panPot->getValue());
	}
	return 0.0f;
}

void ChannelStripComponent::setSliderValue(String slider, double sliderValue)
{
	if (!slider.compare("Gain")) {
		volumeSlider->setValue(static_cast<float>(sliderValue), sendNotificationAsync);
	}
	else if (!slider.compare("Panning")) {
		panPot->setValue(static_cast<float>(sliderValue), sendNotificationAsync);
	}
}

PopupMenu ChannelStripComponent::getPluginsMenu()
{
	PopupMenu allPlugins_;
	_plugins.clear();

	if (_trackID != 0)
	{
		PopupMenu preFaderPlugins_;
		preFaderPlugins_.addCustomItem(1, plugins1, 80, 20, false);
		preFaderPlugins_.addCustomItem(2, plugins2, 80, 20, false);
		preFaderPlugins_.addCustomItem(3, plugins3, 80, 20, false);
		preFaderPlugins_.addCustomItem(4, plugins4, 80, 20, false);

		allPlugins_.addSubMenu("Pre-Fader Plugins", preFaderPlugins_);
	}
	PopupMenu postFaderPlugins_;

		postFaderPlugins_.addCustomItem(5, plugins5, 80, 20, false);
		postFaderPlugins_.addCustomItem(6, plugins6, 80, 20, false);
		postFaderPlugins_.addCustomItem(7, plugins7, 80, 20, false);
		postFaderPlugins_.addCustomItem(8, plugins8, 80, 20, false);

		allPlugins_.addSubMenu("Post-Fader Plugins", postFaderPlugins_);
	return allPlugins_;
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
	else if (clickedButton == _pluginsButton)
	{
		getPluginsMenu().showMenuAsync(PopupMenu::Options().withTargetComponent(_pluginsButton), nullptr);
		
	}
    else if(clickedButton == plugins1 || plugins2 || plugins3 || plugins4 || plugins5 || plugins6 || plugins7 || plugins8)
    {
        _plugins.clear();
        KnownPluginList& pluginsList = _engine.getMixer()->getKnownPluginList();
        pluginsList.addToMenu(_plugins, KnownPluginList::sortAlphabetically);
        const int index = _plugins.show();
        const int pluginIndex = pluginsList.getIndexChosenByMenu(index);
        PluginDescription* desc = pluginsList.getType(pluginIndex);
        
		if(clickedButton == plugins1)
        {
            _engine.getMixer()->addPlugin(_trackID, 0, desc, 0, 0);
            plugins1->setEnabled(false);
			plugins2->setEnabled(true);
        }
        else if (clickedButton == plugins2)
        {
            _engine.getMixer()->addPlugin(_trackID, 1, desc, 0, 0);
            plugins2->setEnabled(false);
			plugins3->setEnabled(true);
        }
        else if (clickedButton == plugins3)
        {
            _engine.getMixer()->addPlugin(_trackID, 2, desc, 0, 0);
            plugins3->setEnabled(false);
			plugins4->setEnabled(true);
        }
        else if (clickedButton == plugins4)
        {
            _engine.getMixer()->addPlugin(_trackID, 3, desc, 0, 0);
            plugins4->setEnabled(false);

        }
		else if (clickedButton == plugins5)
		{
			_engine.getMixer()->addPlugin(_trackID, 4, desc, 0, 0);
			plugins5->setEnabled(false);
			plugins6->setEnabled(true);
		}
		else if (clickedButton == plugins6)
		{
			_engine.getMixer()->addPlugin(_trackID, 5, desc, 0, 0);
			plugins6->setEnabled(false);
			plugins7->setEnabled(true);
		}
		else if (clickedButton == plugins7)
		{
			_engine.getMixer()->addPlugin(_trackID, 2, desc, 0, 0);
			plugins7->setEnabled(false);
			plugins8->setEnabled(true);
		}
		else if (clickedButton == plugins8)
		{
			_engine.getMixer()->addPlugin(_trackID, 3, desc, 0, 0);
			plugins8->setEnabled(false);
		}
    }
}

