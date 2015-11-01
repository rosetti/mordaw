/*
  ==============================================================================

    BasePlugin.h
    Created: 1 Nov 2015 6:49:32pm
    Author:  dtl

  ==============================================================================
*/

#ifndef BASEPLUGIN_H_INCLUDED
#define BASEPLUGIN_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class BasePlugin : public AudioProcessor,
                   public ChangeBroadcaster
{
public:
    BasePlugin();
    ~BasePlugin();
};



#endif  // BASEPLUGIN_H_INCLUDED
