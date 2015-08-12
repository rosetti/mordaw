/*
  ==============================================================================

    MainWindow.cpp
    Created: 2 Aug 2015 2:34:00am
    Author:  Thomas

  ==============================================================================
*/

#include "MainWindow.h"

MainWindow::MainWindow(ApplicationCommandManager &commands) :
    DocumentWindow("KentDAW", Colours::darkgrey, allButtons),
    Content(commands), 
    _menu(commands),
    _commands(commands)
{
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
    }
}

void MainWindow::getAllCommands(Array<CommandID>& commands) const {
    const CommandID ids[] = {
        refreshComponents
    };

    commands.addArray(ids, numElementsInArray(ids));
}

bool MainWindow::perform(const ApplicationCommandTarget::InvocationInfo& info) {
    switch (info.commandID) {
    case refreshComponents:
        _menu.refresh();
        return true;

    default:
        return false;
    }
}