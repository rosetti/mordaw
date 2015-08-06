/*
  ==============================================================================

    MainWindow.h
    Created: 2 Aug 2015 2:34:00am
    Author:  Thomas

  ==============================================================================
*/

#ifndef MAINWINDOW_H_INCLUDED
#define MAINWINDOW_H_INCLUDED

#include "TopMenu.h"
#include "MainComponent.h"

class MainWindow : public DocumentWindow
{
public:
    MainWindow(ApplicationCommandManager &commands);
    ~MainWindow();

    MainComponent Content;

    void closeButtonPressed() override;

private:
    void initializeContent();
    void initializeMenu();
   
    TopMenu _menu;
    LookAndFeel_V3 _lookAndFeel;
    ApplicationCommandManager &_commands;
  
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
};

#endif  // MAINWINDOW_H_INCLUDED
