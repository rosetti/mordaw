/*
  ==============================================================================

    MixerView.cpp
    Created: 10 Aug 2015 11:10:33pm
    Author:  dtl

  ==============================================================================
*/

#include "MixerView.h"

//==============================================================================
MixerView::MixerView(const ApplicationCommandManager &commands)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

MixerView::~MixerView()
{
}

void MixerView::addTrack(int trackIndex) {
    auto stripComponent = new ChannelStripComponent(trackIndex);
    _strips.add(stripComponent);
    addAndMakeVisible(stripComponent);
    resized();
}

void MixerView::paint (Graphics& g)
{
    g.fillAll (Colours::white);   // clear the background
}

void MixerView::resized()
{
    auto i = 0;
    for (auto strip : _strips) {
        strip->setBounds((100 * i++) + 20, 0, 100, getHeight());
    }
}
