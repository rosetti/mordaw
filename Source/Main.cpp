#include "../JuceLibraryCode/JuceHeader.h"
#include "Audio/Engine.h"
#include "GUI/MainWindow.h"
#include "Core/ProjectManager.h"
#include "Resources/Images/SplashImage.h"

//==============================================================================
class KentDAWApplication  : public JUCEApplication
{
public:
    //==============================================================================
    KentDAWApplication() :
        _engine(&_commandsManager) {
        
    }

    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return true; }

    //==============================================================================
    void initialise (const String&) override
    {
		/*
		Image image_ = ImageCache::getFromMemory(SplashImage::mordaw_jpg, SplashImage::mordaw_jpgSize);
		SplashScreen splash_("Welcome to morDAW",
			image_, 
			false);
        
        
		splash_.deleteAfterDelay(RelativeTime::seconds(4), false);
		*/
		registerCommands();

		_mainWindow = new MainWindow(_commandsManager, _engine);
		_projectManager = new ProjectManager(_commandsManager, _engine, *_mainWindow);
		_projectManager->createBasicProjectFramework("Untitled_Project");
    }

    void shutdown() override
    {
        delete _mainWindow;
    }

    void registerCommands()
    {
        _commandsManager.registerAllCommandsForTarget(this);
        _commandsManager.registerAllCommandsForTarget(getInstance());
    }

    ApplicationCommandTarget * getNextCommandTarget() override
    {
        return JUCEApplication::getNextCommandTarget();
    }

    void getAllCommands(Array<CommandID>& commands) override
    {
        JUCEApplication::getAllCommands(commands);
        _projectManager->getAllCommands(commands);
        _engine.getAllCommands(commands);
        _mainWindow->getAllCommands(commands);
    }

    void getCommandInfo(CommandID commandID, ApplicationCommandInfo & result) override
    {
        JUCEApplication::getCommandInfo(commandID, result);
        _projectManager->getCommandInfo(commandID, result);
        _engine.getCommandInfo(commandID, result);
        _mainWindow->getCommandInfo(commandID, result);
    }

    bool perform(const InvocationInfo & info) override
    {
        ApplicationCommandInfo commandInfo(info.commandID);

        getCommandInfo(info.commandID, commandInfo);

        if (commandInfo.categoryName == "Project Management" || commandInfo.categoryName == "Selection") {
            return _projectManager->perform(info);
        }

        if (commandInfo.categoryName == "Audio") {
            return _engine.perform(info);
        }

        if (commandInfo.categoryName == "Global") {
            return _mainWindow->perform(info);
        }

        return JUCEApplication::perform(info);
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        quit();
    }

    void anotherInstanceStarted (const String&) override
    {
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.
    }

private:
    ScopedPointer<ProjectManager> _projectManager;
    MainWindow* _mainWindow;
    Audio::Engine _engine;
    ApplicationCommandManager _commandsManager;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KentDAWApplication)
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (KentDAWApplication)
