/*
  ==============================================================================

    MainWindow.cpp
    Created: 2 Aug 2015 2:34:00am
    Author:  Thomas

  ==============================================================================
*/

#include "MainWindow.h"

MainWindow::MainWindow(ApplicationCommandManager &commands) :
    DocumentWindow("KentDAW", Colours::darkgrey, TitleBarButtons::allButtons), 
    _commandsManager(commands), 
    _menu(commands),
    Content(commands)
{
    setLookAndFeel(&_lookAndFeel);
    setUsingNativeTitleBar(true);
    centreWithSize(800, 600);
    maximiseButtonPressed();

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
    getTopLevelComponent()->addKeyListener(&Content);
}

void MainWindow::initializeMenu()
{
    setMenuBar(&_menu);
    getMenuBarComponent()->setColour(PopupMenu::backgroundColourId, Colours::lightgrey);
}
