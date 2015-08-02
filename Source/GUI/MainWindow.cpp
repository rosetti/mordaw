/*
  ==============================================================================

    MainWindow.cpp
    Created: 2 Aug 2015 2:34:00am
    Author:  Thomas

  ==============================================================================
*/

#include "MainWindow.h"

MainWindow::MainWindow() : DocumentWindow("KentDAW", Colours::darkgrey, TitleBarButtons::allButtons)
{
    setLookAndFeel(&lookAndFeel);
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
    setMenuBar(&menu);
    getMenuBarComponent()->setColour(PopupMenu::backgroundColourId, Colours::lightgrey);
    setContentOwned(&Content, false);
}
