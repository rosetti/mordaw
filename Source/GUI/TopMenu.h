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
#include <map>

class TopMenu : public MenuBarModel
{
public:
    TopMenu(ApplicationCommandManager &commandManager);
    ~TopMenu();

    // Inherited via MenuBarModel
    virtual StringArray getMenuBarNames() override;
    virtual PopupMenu getMenuForIndex(int topLevelMenuIndex, const String & menuName) override;
    virtual void menuItemSelected(int menuItemID, int topLevelMenuIndex) override;

private:
    StringArray _menusNames;
    std::map<String, PopupMenu> _menus;
    ApplicationCommandManager &_commandsManager;
};

#endif  // TOPMENU_H_INCLUDED
