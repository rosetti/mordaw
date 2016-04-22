/*
 ==============================================================================
 
 LeftSide.cpp
 Created: 4 Aug 2015 7:58:42pm
 Author:  Matt
 
 ==============================================================================
 */

#include "../../JuceLibraryCode/JuceHeader.h"
#include "LeftSide.h"
#include "../Audio/Mixer.h"

//==============================================================================

LeftSide::LeftSide(ApplicationCommandManager &commands, const Audio::Engine &engine, AudioPluginFormatManager &)
: _commands(commands),
_tsThread("File Tree"),
_directoryList(nullptr, _tsThread),
_engine(engine),
_tabbedComponent(TabbedButtonBar::Orientation::TabsAtLeft),
_fileTree(_directoryList),
_projectTab(_commands),
_masterStrip(commands, 0, engine)
{
    _pluginList = new PluginListComponent(_engine.getMixer()->getFormatManager(), _engine.getMixer()->getKnownPluginList(), File::nonexistent, nullptr);
#if defined(__APPLE__)
    _pluginList->scanFor(_auFormat);
#endif
    
    //_tabbedComponent = new TabbedComponent(TabbedButtonBar::Orientation::TabsAtLeft);
    //Set Up the Project Tab
    //_projectTab = new ProjectTab(_commands);
    //Set Up the File Tree and Directories
    prepareFileTree();
    //Set Up the Plugin List
    /*
     _formats.registerBasicFormats();
     
     _pluginList = new PluginListComponent(_formats, _plugins, nullptr, nullptr);
     preparePluginList();
     */
    //Add Tabs to the component
    addTabs();
    addAndMakeVisible(_tabbedComponent);
}

LeftSide::~LeftSide()
{
    _tabbedComponent.removeAllChildren();
}

void LeftSide::prepareFileTree()
{
    //_fileTree = new FileTreeComponent(_directoryList);
    _directoryList.setDirectory(File::getSpecialLocation(File::userHomeDirectory), true, true);
    _tsThread.startThread(3);
    //_fileTree->setDragAndDropDescription("File Tree Drag and Drop");
}

void LeftSide::preparePluginList()
{
    
}

void LeftSide::addTabs()
{
    _tabbedComponent.addTab("Project", Colours::darkgrey, &_projectTab, false);
    _tabbedComponent.addTab("Files", Colours::darkgrey, &_fileTree, false);
    _tabbedComponent.addTab("Plugins", Colours::darkgrey, _pluginList, false);
    _tabbedComponent.addTab("Master", Colours::darkgrey, &_masterStrip, false);
}

void LeftSide::paint(Graphics& g)
{
    g.setColour(Colours::steelblue);
    g.drawRect(getLocalBounds(), 2);   // draw an outline around the component
    
    g.setColour(Colours::black);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component
}

void LeftSide::resized()
{
    _tabbedComponent.setBounds(0, 0, getWidth(), getHeight());
    _masterStrip.setBoundsRelative(0.0f, 0.25f, 0.8f, 0.5f);
}