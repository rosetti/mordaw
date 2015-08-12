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

class MainWindow : public DocumentWindow, public KeyListener
{
public:
    MainWindow(ApplicationCommandManager &commands);
    ~MainWindow();

    MainComponent Content;

    void closeButtonPressed() override;
    bool keyPressed(const KeyPress& key, Component* originatingComponent) override;

    void getCommandInfo(CommandID commandID, ApplicationCommandInfo &result) const;
    void getAllCommands(Array<CommandID>& commands) const;
    bool perform(const ApplicationCommandTarget::InvocationInfo & info);

    enum Commands {
        refreshComponents = 0x300,
    };

private:
    void initializeContent();
    void initializeMenu();
   
    TopMenu _menu;
    LookAndFeel_V3 _lookAndFeel;
    ApplicationCommandManager &_commands;
  
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
};

#endif  // MAINWINDOW_H_INCLUDED
