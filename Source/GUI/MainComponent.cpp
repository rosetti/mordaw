/*
  ==============================================================================

    MainComponent.cpp
    Created: 3 Aug 2015 10:18:09pm
    Author:  Thomas

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"
#include "MainWindow.h"
#include "../Core/ProjectManager.h"

//==============================================================================
MainComponent::MainComponent(ApplicationCommandManager &commands, const Audio::Engine &engine) :
    _leftSide(commands, engine),
    _arrangement(commands, engine),
	_arrangePort(),
	_mixPort(),
    _mixerView(commands, engine),
    _transportControls(commands, engine),
    _commands(commands),
    _trackCounter(0)
{
	_arrangeVisible = true;
	_mixerVisible = false;
	//Left Side
	addAndMakeVisible(_leftSide);

	//Arrangement, Mixer and Viewports
	addChildComponent(_arrangePort);
	addChildComponent(_arrangement);
	addChildComponent(_mixPort);
	addChildComponent(_mixerView);
	_arrangePort.setViewedComponent(&_arrangement);
	_arrangePort.setScrollBarsShown(true, true, false, false);
	_mixPort.setViewedComponent(&_mixerView);
	_mixPort.setScrollBarsShown(true, true, false, false);
	switchView(false);
	
	//Transport
    addAndMakeVisible(_transportControls);
}

MainComponent::~MainComponent()
{
}

void MainComponent::paint (Graphics& g)
{
	//g.setColour(Colours::darkorange);
    //g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void MainComponent::resized()
{
    _leftSide.setBounds(0, 0, 300, getHeight());
	_arrangePort.setBounds(_leftSide.getWidth(), 0, getWidth() - _leftSide.getWidth(), getHeight() - 50);
	_mixPort.setBounds(_leftSide.getWidth(), 0, getWidth() - _leftSide.getWidth(), getHeight() - 50);
    _arrangement.setBounds(_leftSide.getWidth(), 0, getWidth() - _leftSide.getWidth(), getHeight() - 50);
    _mixerView.setBounds(_leftSide.getWidth(), 0, getWidth() - _leftSide.getWidth(), getHeight() - 50);
    _transportControls.setBounds(_leftSide.getWidth(), getHeight() - 50, getWidth() - _leftSide.getWidth(), 50);
}

void MainComponent::mouseDown(const MouseEvent &e) {

	ModifierKeys modifiers = ModifierKeys::getCurrentModifiersRealtime();

	// check the mod keys ..
	if (modifiers.isPopupMenu() || modifiers.isCtrlDown())
	{
		ScopedPointer<PopupMenu> arrangeMenu_ = new PopupMenu();
		arrangeMenu_->clear();
		arrangeMenu_->addCommandItem(&_commands, ProjectManager::addTrack);
		arrangeMenu_->addCommandItem(&_commands, MainWindow::showMixer);
		arrangeMenu_->show();
	}
}

void MainComponent::mouseDoubleClick(const MouseEvent & e)
{
}

void MainComponent::addTrack(Audio::Track* track) {
    _trackCounter++;
    _arrangement.addTrack(track);
    _mixerView.addTrack(_trackCounter);
}

TransportControls* MainComponent::getTransportControls() {
    return &_transportControls;
}

Arrangement * MainComponent::getArrangement()
{
	return &_arrangement;
}

MixerView * MainComponent::getMixer()
{
	return &_mixerView;
}

bool MainComponent::arrangeIsShowing() const
{
	if (_arrangeVisible) {
		return true;
	}
	return false;
}

bool MainComponent::mixerIsShowing() const
{
	if (_mixerVisible) {
		return true;
	}
	return false;
}

void MainComponent::switchView(bool arrangeEnabled)
{
	if (!arrangeEnabled)
	{
		//Disable the Mixer
		_mixerView.setVisible(false);
		_mixPort.setVisible(false);
		//Enable the Arrange
		_arrangement.setVisible(true);
		_arrangePort.setVisible(true);
	}
	else
	{
		//Disable the Arrange
		_arrangement.setVisible(false);
		_arrangePort.setVisible(false);
		//Enable the Mixer
		_mixerView.setVisible(true);
		_mixPort.setVisible(true);
	}
}
