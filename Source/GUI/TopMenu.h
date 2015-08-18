/*
  ==============================================================================

    TopMenu.h
    Created: 2 Aug 2015 12:24:35pm
    Author:  Thomas

  ==============================================================================
*/

#ifndef TOPMENU_H_INCLUDED
#define TOPMENU_H_INCLUDED

#include "../../JuceLibraryCode/JuceHeader.h"
#include <list>

class TopMenu : public MenuBarModel
{
public:
    TopMenu(ApplicationCommandManager &commandManager);
    ~TopMenu();

    void addFileMenu(ApplicationCommandManager* commands);
    void addEditMenu(ApplicationCommandManager* commands);
    void addViewMenu(ApplicationCommandManager* commands);
	void addProjectMenu(ApplicationCommandManager * commands);
    void addTransportMenu(ApplicationCommandManager* commands);

    // Inherited via MenuBarModel
    virtual StringArray getMenuBarNames() override;
    virtual PopupMenu getMenuForIndex(int topLevelMenuIndex, const String & menuName) override;
    virtual void menuItemSelected(int menuItemID, int topLevelMenuIndex) override;
    void refresh();
private:
    StringArray _menusNames;
    std::list<std::pair<String, PopupMenu>> _menus;
    ApplicationCommandManager &_commandsManager;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TopMenu)
};

#endif  // TOPMENU_H_INCLUDED
