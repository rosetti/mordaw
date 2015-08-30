/*
==============================================================================

TopMenu.cpp
Created: 2 Aug 2015 12:24:35pm
Author:  Thomas

==============================================================================
*/

#include "TopMenu.h"
#include "MainComponent.h"
#include "../Core/ProjectManager.h"
#include "../Audio/Engine.h"

TopMenu::TopMenu(ApplicationCommandManager& commandsManager) : MenuBarModel(), _commandsManager(commandsManager) {
    auto commands = &_commandsManager;

    addFileMenu(commands);
    addEditMenu(commands);
    addViewMenu(commands);
	addProjectMenu(commands);
    addTransportMenu(commands);

    menuItemsChanged();

    for (auto current = _menus.begin(), end = _menus.end(); current != end; ++current) {
        _menusNames.add(current->first);
    }
}

TopMenu::~TopMenu() {
}

void TopMenu::addFileMenu(ApplicationCommandManager* commands) {
    PopupMenu fileMenu;
	PopupMenu exportMenu;

	exportMenu.addCommandItem(commands, ProjectManager::exportWav);

    fileMenu.addCommandItem(commands, ProjectManager::newProject);
    fileMenu.addCommandItem(commands, ProjectManager::openProject);
    fileMenu.addCommandItem(commands, ProjectManager::saveProject);
    fileMenu.addCommandItem(commands, ProjectManager::saveProjectAs);
    fileMenu.addSeparator();
	fileMenu.addSubMenu("Export Project...", exportMenu, true);
	fileMenu.addSeparator();
    fileMenu.addCommandItem(commands, StandardApplicationCommandIDs::quit);
    _menus.push_back(std::pair<String, PopupMenu>("File", fileMenu));
}

void TopMenu::addEditMenu(ApplicationCommandManager* commands) {
    PopupMenu editMenu;

    editMenu.addCommandItem(commands, StandardApplicationCommandIDs::undo);
    editMenu.addCommandItem(commands, StandardApplicationCommandIDs::redo);
    editMenu.addSeparator();
    editMenu.addCommandItem(commands, StandardApplicationCommandIDs::cut);
    editMenu.addCommandItem(commands, StandardApplicationCommandIDs::copy);
    editMenu.addCommandItem(commands, StandardApplicationCommandIDs::paste);
    _menus.push_back(std::pair<String, PopupMenu>("Edit", editMenu));
}

void TopMenu::addViewMenu(ApplicationCommandManager* commands) {
    PopupMenu viewMenu;
    
    viewMenu.addCommandItem(commands, MainWindow::showArrangement);
    viewMenu.addCommandItem(commands, MainWindow::showMixer);
    _menus.push_back(std::pair<String, PopupMenu>("View", viewMenu));
}

void TopMenu::addProjectMenu(ApplicationCommandManager* commands) {
	PopupMenu projectMenu;

	projectMenu.addCommandItem(commands, ProjectManager::addTrack);
	projectMenu.addCommandItem(commands, ProjectManager::addRegion);
	_menus.push_back(std::pair<String, PopupMenu>("Project", projectMenu));
}

void TopMenu::addTransportMenu(ApplicationCommandManager* commands) {
    PopupMenu transportMenu;

    transportMenu.addCommandItem(commands, Audio::Engine::start);
    transportMenu.addCommandItem(commands, Audio::Engine::pause);
    transportMenu.addCommandItem(commands, Audio::Engine::stop);
    _menus.push_back(std::pair<String, PopupMenu>("Transport", transportMenu));
}


StringArray TopMenu::getMenuBarNames() {
    return _menusNames;
}

PopupMenu TopMenu::getMenuForIndex(int, const String& menuName) {
    for (auto it = _menus.begin(), end = _menus.end(); it != end; ++it) {
        if (it->first == menuName) {
            return it->second;
        }
    }

    return PopupMenu();
}

void TopMenu::menuItemSelected(int, int) {
}

void TopMenu::refresh() {
    _menus.clear();

    addFileMenu(&_commandsManager);
    addEditMenu(&_commandsManager);
    addViewMenu(&_commandsManager);
    addTransportMenu(&_commandsManager);

    menuItemsChanged();
}