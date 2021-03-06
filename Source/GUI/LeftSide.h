/*
  ==============================================================================

    LeftSide.h
    Created: 4 Aug 2015 7:58:42pm
    Author:  Matt

  ==============================================================================
*/

#ifndef LEFTSIDE_H_INCLUDED
#define LEFTSIDE_H_INCLUDED

#include "TransportControls.h"
#include "ProjectTab.h"
#include "ChannelStripComponent.h"

//==============================================================================
/*
*/
class LeftSide    : public Component
{
public:
    LeftSide(ApplicationCommandManager &commands, const Audio::Engine &engine, AudioPluginFormatManager &pluginManager);
	~LeftSide();

	void prepareFileTree();

	void preparePluginList();

	void addTabs();

    void paint (Graphics&);
    void resized();
    
    void menuItemSelected();

private:
    ApplicationCommandManager &_commands;

    TimeSliceThread _tsThread;
	DirectoryContentsList _directoryList;

	const Audio::Engine &_engine;
	AudioFormatManager _formats;
    
    #if defined(__APPLE__)
    AudioUnitPluginFormat _auFormat;
    #endif
    VSTPluginFormat _vstFormat;
	
	TabbedComponent _tabbedComponent;
	FileTreeComponent _fileTree;
	ProjectTab _projectTab;
	ChannelStripComponent _masterStrip;
    
    StringArray _strings;
    
	ScopedPointer<PluginListComponent> _pluginList;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LeftSide)
};


#endif  // LEFTSIDE_H_INCLUDED
