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
    _mixerView(commands),
    _transportControls(commands, engine),
    _commands(commands)
{
	//Left Side
	//addAndMakeVisible(_leftSide);

	//Arrangement and Viewport
	addAndMakeVisible(_arrangePort);
    addAndMakeVisible(_arrangement);
	addChildComponent(_mixPort);
	addChildComponent(_mixerView);
	_arrangePort.setViewedComponent(&_arrangement);
	_arrangePort.setScrollBarsShown(true, true, false, false);
	_mixPort.setViewedComponent(&_mixerView);
	_mixPort.setScrollBarsShown(true, true, false, false);
	
	//Transport
    addAndMakeVisible(_transportControls);
    
	
    //addAndMakeVisible(_mixerView);
    //_mixerView.addTrack(1);
    //_mixerView.addTrack(2);
    //_mixerView.setAlwaysOnTop(true);
	
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
    // This method is where you should set the bounds of any child
    // components that your component contains..
    _leftSide.setBounds(0, 0, 300, getHeight());
	_arrangePort.setBounds(_leftSide.getWidth(), 0, getWidth() - _leftSide.getWidth(), getHeight() - 50);
	_mixPort.setBounds(_leftSide.getWidth(), 0, getWidth() - _leftSide.getWidth(), getHeight() - 50);
    _arrangement.setBounds(_leftSide.getWidth(), 0, getWidth() - _leftSide.getWidth(), getHeight() - 50);
    _mixerView.setBounds(_leftSide.getWidth(), 0, getWidth() - _leftSide.getWidth(), getHeight() - 50);

    // getHeight() - 200
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
		arrangeMenu_->show();
	}
}

void MainComponent::mouseDoubleClick(const MouseEvent & e)
{
	ProjectManager::addTrack;
}

void MainComponent::addTrack(Audio::Track* track) {
    _arrangement.addTrack(track);
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

int MainComponent::getCurrentViewState(String view)
{
	if (view == "mixer") {
		if (_mixerView.isShowing()) {
			return 1;
		}
		else {
			return 0;
		}
	}
	else if (view == "arrangement") {
		if (_arrangement.isShowing()) {
			return 1;
		}
		else {
			return 0;
		}
	}
}


void MainComponent::showArrangement()
{
	_mixerView.setVisible(false);
	_mixPort.setVisible(false);
	_arrangement.setVisible(true);
	_arrangePort.setVisible(true);
	_arrangement.setOpaque(true);
	
	
	//_mixerView.setAlwaysOnTop(false);
	//_arrangement.setAlwaysOnTop(true);
}

void MainComponent::showMixer()
{
	_arrangement.setVisible(false);
	_arrangePort.setVisible(false);
	_mixerView.setVisible(true);
	_mixPort.setVisible(true);
	_mixerView.setOpaque(true);
	//_arrangement.setAlwaysOnTop(false);
	//_mixerView.setAlwaysOnTop(true);
}

