/*
  ==============================================================================

    MainComponent.h
    Created: 3 Aug 2015 10:18:09pm
    Author:  Thomas

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class MainComponent : public Component, public KeyListener
{
public:
    MainComponent(const ApplicationCommandManager &manager);
    ~MainComponent();

    void paint(Graphics&);
    void resized();

    const ApplicationCommandManager &_manager;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)

        // Inherited via KeyListener
        virtual bool keyPressed(const KeyPress & key, Component * originatingComponent) override;
};


#endif  // MAINCOMPONENT_H_INCLUDED
