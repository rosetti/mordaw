/*
==============================================================================

TopMenu.cpp
Created: 2 Aug 2015 12:24:35pm
Author:  Thomas

==============================================================================
*/

#include "TopMenu.h"
#include "MainWindow.h"
#include "../Core/ProjectManager.h"

TopMenu::TopMenu(ApplicationCommandManager &commandsManager) : MenuBarModel(), _commandsManager(commandsManager)
{
    PopupMenu fileMenu;
    ApplicationCommandManager *commands = &_commandsManager;

    fileMenu.addCommandItem(commands, ProjectManager::newProject);
    fileMenu.addCommandItem(commands, ProjectManager::openProject);
    fileMenu.addSeparator();
    fileMenu.addCommandItem(commands, StandardApplicationCommandIDs::quit);
    _menus.insert(std::pair<String, PopupMenu>("File", fileMenu));

    menuItemsChanged();

    for (auto current = _menus.begin(), end = _menus.end(); current != end; ++current) {
        _menusNames.add(current->first);
    }
}

TopMenu::~TopMenu()
{
}

StringArray TopMenu::getMenuBarNames()
{
    return _menusNames;
}

PopupMenu TopMenu::getMenuForIndex(int topLevelMenuIndex, const String & menuName)
{
    return _menus.at(menuName);
}

void TopMenu::menuItemSelected(int menuItemID, int topLevelMenuIndex)
{
    
}
