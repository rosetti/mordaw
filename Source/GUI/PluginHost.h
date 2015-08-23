/*
  ==============================================================================

    PluginHost.h
    Created: 23 Aug 2015 4:40:08am
    Author:  dtl

  ==============================================================================
*/

#ifndef PLUGINHOST_H_INCLUDED
#define PLUGINHOST_H_INCLUDED

#include "../../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/

class PluginHost
{
public:
    class Listener
    {
    public:
        virtual ~Listener();
        virtual void pluginAdded();
        virtual void pluginRemoved();
        virtual void pluginHostChanged();
        
    protected:
        Listener() {};
    };
    
    PluginHost();
    ~PluginHost();
    void loadPlugin(const File& pluginFile);
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginHost)
};




#endif  // PLUGINHOST_H_INCLUDED
