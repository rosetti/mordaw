/*
  ==============================================================================

    MainWindow.h
    Created: 2 Aug 2015 2:34:00am
    Author:  Thomas

  ==============================================================================
*/

#ifndef MAINWINDOW_H_INCLUDED
#define MAINWINDOW_H_INCLUDED

#include "../../JuceLibraryCode/JuceHeader.h"
#include "TopMenu.h"

class MainWindow : public DocumentWindow
{
public:
    MainWindow();
    ~MainWindow();

    void initializeContent();
    void closeButtonPressed() override;

    Component Content;

private:
    TopMenu menu;
    LookAndFeel_V3 lookAndFeel;
  
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
};

#endif  // MAINWINDOW_H_INCLUDED
