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
    _strips.push_back(stripComponent);
    addAndMakeVisible(stripComponent);
    resized();
}

std::vector<ChannelStripComponent*>* MixerView::getChannelStrips()
{
	return &_strips;
}

void MixerView::mouseDown(const MouseEvent &) {

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
	//g.setColour(Colours::darkorange);
	//g.drawRect(getLocalBounds(), 2);   // draw an outline around the component
}

void MixerView::resized()
{
    auto i = 0;
	int j = 0;
    for (auto strip : _strips) {
		if (i < 10) {
			strip->setBounds((100 * i++) + 32, 0, 100, getHeight() / 2);
		}
		else if (i > 9) {
			int test = getHeight() / 2;
			std::cout << test;
			strip->setBounds((100 * j++) + 32, getHeight() / 2 - 1, 100, getHeight() / 2);
			i++;
		}
    }
}
