/*
  ==============================================================================

    MixerView.cpp
    Created: 10 Aug 2015 11:10:33pm
    Author:  dtl

  ==============================================================================
*/

#include "MixerView.h"
#include "MainWindow.h"
#include "../Core/ProjectManager.h"

//==============================================================================
MixerView::MixerView(ApplicationCommandManager &commands, const Audio::Engine &engine)	: _commands(commands),
	_engine(engine)
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

void MixerView::mouseDown(const MouseEvent &e) {

	ModifierKeys modifiers = ModifierKeys::getCurrentModifiersRealtime();

	// check the mod keys ..
	if (modifiers.isPopupMenu() || modifiers.isCtrlDown())
	{
		ScopedPointer<PopupMenu> arrangeMenu_ = new PopupMenu();
		arrangeMenu_->clear();
		arrangeMenu_->addCommandItem(&_commands, ProjectManager::addTrack);
		arrangeMenu_->addCommandItem(&_commands, MainWindow::showArrangement);
		arrangeMenu_->show();
	}
}

void MixerView::paint (Graphics& g)
{
}

void MixerView::resized()
{
    auto i = 0;
    for (auto strip : _strips) {
        strip->setBounds((100 * i++) + 20, 0, 100, getHeight());
    }
}
