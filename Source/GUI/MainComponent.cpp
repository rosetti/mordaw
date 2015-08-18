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
    _leftSide(commands),
    _arrangement(commands, engine),
	_arrangementView(),
    _mixerView(commands),
    _transportControls(commands, engine),
    _commands(commands)
{
    addAndMakeVisible(_leftSide);
    addAndMakeVisible(_arrangement);
	addAndMakeVisible(_arrangementView);
	_arrangementView.setViewedComponent(&_arrangement);
	_arrangementView.setScrollBarsShown(true, true, false, false);
    addAndMakeVisible(_transportControls);
}

MainComponent::~MainComponent()
{
}

void MainComponent::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (Colours::white);   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::lightblue);
    g.setFont (14.0f);
    g.drawText ("MainComponent", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void MainComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    _leftSide.setBounds(0, 0, 300, getHeight());
	_arrangementView.setBounds(_leftSide.getWidth(), 0, getWidth() - _leftSide.getWidth(), getHeight() - 50);
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

void MainComponent::addTrack(Audio::Track* track) {
    _arrangement.addTrack(track);
}

TransportControls* MainComponent::getTransportControls() {
    return &_transportControls;
}

void MainComponent::getCommandInfo(CommandID commandID, ApplicationCommandInfo& result) const {
    const String view("View");
    int flags;
    
    switch (commandID) {
        case showArrangement:
            flags = _arrangement.isShowing() ? ApplicationCommandInfo::isDisabled : 0;
            result.setInfo("Show Arrangement", "Display the arrangement in the main window.", view, flags);
            result.addDefaultKeypress(KeyPress::tabKey, 0);
            break;
        case showMixer:
            flags = _mixerView.isShowing() ? ApplicationCommandInfo::isDisabled : 0;
            result.setInfo("Show Mixer", "Display the mixer in the main window.", view, flags);
            result.addDefaultKeypress(KeyPress::tabKey, 0);
            break;
        default:
            break;
    }
}

void MainComponent::getAllCommands(Array<CommandID>& commands) const {
    const CommandID ids[] = {
        showArrangement,
        showMixer
    };
    
    commands.addArray(ids, numElementsInArray(ids));
}

bool MainComponent::perform(const ApplicationCommandTarget::InvocationInfo& info) {
    switch (info.commandID) {
        case showArrangement:
            if(_arrangement.isShowing())
            {
                _arrangement.setVisible(false);
                _mixerView.setVisible(true);
            }
            return true;
            
        case showMixer:
            if(_mixerView.isShowing())
            {
                _mixerView.setVisible(false);
                _arrangement.setVisible(true);
            }
            return true;
            
        default:
            return false;
    }
}