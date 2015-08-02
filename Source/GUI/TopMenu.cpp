/*
==============================================================================

TopMenu.cpp
Created: 2 Aug 2015 12:24:35pm
Author:  Thomas

==============================================================================
*/

#include "TopMenu.h"

TopMenu::TopMenu() : MenuBarModel()
{
    PopupMenu fileMenu;

    fileMenu.addItem(1, "New...");
    fileMenu.addItem(2, "Open...");
    fileMenu.addSeparator();
    fileMenu.addItem(3, "Exit");
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
