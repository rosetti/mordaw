/*
  ==============================================================================

    MainComponent.cpp
    Created: 3 Aug 2015 10:18:09pm
    Author:  Thomas

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent(const ApplicationCommandManager &manager) : _manager(manager)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

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

}

bool MainComponent::keyPressed(const KeyPress & key, Component * originatingComponent)
{
    auto keyMapping = _manager.getKeyMappings();

    return keyMapping->keyPressed(key, originatingComponent);
}
