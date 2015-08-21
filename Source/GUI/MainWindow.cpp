/*
  ==============================================================================

    MainWindow.cpp
    Created: 2 Aug 2015 2:34:00am
    Author:  Thomas

  ==============================================================================
*/

#include "MainWindow.h"

MainWindow::MainWindow(ApplicationCommandManager &commands, const Audio::Engine &engine) :
	DocumentWindow("KentDAW", Colours::darkgrey, allButtons),
	Content(commands, engine),
	_menu(commands),
	_commands(commands)
{
	_arrangeEnabled = 1;
	_mixerEnabled = 0;
    setLookAndFeel(&_lookAndFeel);
    setUsingNativeTitleBar(true);
    centreWithSize(800, 600);
    maximiseButtonPressed();
    setResizable(true, true);
    initializeContent();
    setVisible(true);
    addToDesktop();
}

MainWindow::~MainWindow()
{
    setMenuBar(nullptr);
}

void MainWindow::closeButtonPressed()
{
    JUCEApplication::quit();
}

void MainWindow::initializeContent()
{
    initializeMenu();
    setContentOwned(&Content, false);
    getTopLevelComponent()->addKeyListener(this);
}

void MainWindow::initializeMenu()
{
    setMenuBar(&_menu);
    getMenuBarComponent()->setColour(PopupMenu::backgroundColourId, Colours::lightgrey);
}

bool MainWindow::keyPressed(const KeyPress & key, Component * originatingComponent)
{
    auto keyMapping = _commands.getKeyMappings();

    return keyMapping->keyPressed(key, originatingComponent);
}

void MainWindow::getCommandInfo(CommandID commandID, ApplicationCommandInfo& result) const {
    const String global("Global");
    int flags;

    switch (commandID) {
    case refreshComponents:
        result.setInfo("Refresh Components", "Internal command to refresh all components.", global, 0);
        break;
    default:
        break;
	case showArrangement:
		//THE NEXT LINE BREAKS THE BUILD FOR SOME REASON
		//flags = _arrangementShowing ? ApplicationCommandInfo::isDisabled : 0;
		result.setInfo("Show Arrangement", "Display the arrangement in the main window.", global, 0);
		result.addDefaultKeypress(KeyPress::tabKey, 0);
		break;
	case showMixer:
		//THE NEXT LINE BREAKS THE BUILD FOR SOME REASON
		result.setInfo("Show Mixer", "Display the mixer in the main window.", global, 0);
		result.addDefaultKeypress(KeyPress::tabKey, 0);
		break;
    }
}

void MainWindow::getAllCommands(Array<CommandID>& commands) const {
    const CommandID ids[] = {
        refreshComponents,
		showArrangement,
		showMixer
    };

    commands.addArray(ids, numElementsInArray(ids));
}

bool MainWindow::perform(const ApplicationCommandTarget::InvocationInfo& info) {
    switch (info.commandID) {
    case refreshComponents:
        _menu.refresh();
        Content.getTransportControls()->refresh();
        return true;
	
	case showArrangement:
		Content.switchView(false);
		_arrangeEnabled = 1;
		_mixerEnabled = 0;
		return true;

	case showMixer:
		Content.switchView(true);
		_mixerEnabled = 1;
		_arrangeEnabled = 0;
		return true;

    default:
        return false;
    }
}