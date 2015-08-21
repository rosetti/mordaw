/*
  ==============================================================================

    MixerView.cpp
    Created: 10 Aug 2015 11:10:33pm
    Author:  dtl

  ==============================================================================
*/

#include "MixerView.h"

//==============================================================================
MixerView::MixerView(ApplicationCommandManager &commands, const Audio::Engine &engine)
: _commands(commands), _engine(engine)
{
}

MixerView::~MixerView()
{
}

void MixerView::addTrack(int trackIndex) {
    auto stripComponent = new ChannelStripComponent(trackIndex, _engine);
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
