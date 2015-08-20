/*
  ==============================================================================

    MixerView.h
    Created: 10 Aug 2015 11:10:33pm
    Author:  dtl

  ==============================================================================
*/

#ifndef MIXERVIEW_H_INCLUDED
#define MIXERVIEW_H_INCLUDED

#include "../../JuceLibraryCode/JuceHeader.h"
#include "ChannelStripComponent.h"

//==============================================================================
/*
*/
class MixerView    : public Component
{
public:
    MixerView(const ApplicationCommandManager &commands);
    ~MixerView();

    void paint (Graphics&);
    void resized();
    
    void addTrack(int trackIndex);

private:

    Array<ChannelStripComponent*> _strips;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MixerView)
};


#endif  // MIXERVIEW_H_INCLUDED
