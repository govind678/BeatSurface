/*
  ==============================================================================

    MainWindow.cpp
    Created: 13 Feb 2014 6:04:01pm
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#include "MainAppWindow.h"


ScopedPointer<ApplicationCommandManager> commandManager;
ScopedPointer<AudioDeviceManager> sharedAudioDeviceManager;
ScopedPointer<GlobalClock>  globalClock;
ScopedPointer<GUIUpdater>   guiUpdater;


//--- Document Window Init Size ---//
static const int iDocumentInitWidth                 = 1024;
static const int iDocumentInitHeight                = 680;


MainAppWindow::MainAppWindow()  : DocumentWindow (JUCEApplication::getInstance()->getApplicationName(),
                                                  Colours::lightgrey,
                                                  DocumentWindow::allButtons)
{
    setUsingNativeTitleBar (true);
    setResizable (true, false);
    setResizeLimits (400, 400, 10000, 10000);

    setBounds (0, 0, getParentWidth(), getParentHeight());

    
    mainComponent = new MainComponent;
    setContentOwned(mainComponent, true);
    
    #if JUCE_MAC
    centreWithSize (iDocumentInitWidth, iDocumentInitHeight);
    #endif
    
    addKeyListener (commandManager->getKeyMappings());
    
    
    setVisible (true);
}



MainAppWindow::~MainAppWindow()
{
    clearContentComponent();
//    commandManager              = nullptr;
    
}



void MainAppWindow::closeButtonPressed()
{
    JUCEApplication::getInstance()->systemRequestedQuit();
}



//AudioDeviceManager& MainAppWindow::getSharedAudioDeviceManager()
//{
//    if (sharedAudioDeviceManager == nullptr)
//    {
//        sharedAudioDeviceManager = new AudioDeviceManager();
//        sharedAudioDeviceManager->initialise(BeatSurfaceBase::iNumInputChannels, BeatSurfaceBase::iNumOutputChannels, 0, true, String::empty, 0);
//    }
//    
//    return *sharedAudioDeviceManager;
//}


//ApplicationCommandManager& MainAppWindow::getApplicationCommandManager()
//{
//    if (commandManager == nullptr)
//        commandManager = new ApplicationCommandManager();
//    
//    return *commandManager;
//}



MainAppWindow* MainAppWindow::getMainAppWindow()
{
    for (int i = TopLevelWindow::getNumTopLevelWindows(); --i >= 0;)
        if (MainAppWindow* maw = dynamic_cast<MainAppWindow*> (TopLevelWindow::getTopLevelWindow (i)))
            return maw;
    
    return nullptr;
}


//void MainAppWindow::handleAsyncUpdate()
//{
//    // This registers all of our commands with the command manager but has to be done after the window has
//    // been created so we can find the number of rendering engines available
//    //ApplicationCommandManager& commandManager = MainAppWindow::getApplicationCommandManager();
////    commandManager.registerAllCommandsForTarget (tabbedComponent);
//    //commandManager.registerAllCommandsForTarget (JUCEApplication::getInstance());
//}


MainComponent* MainAppWindow::getMainComponent()
{
    return mainComponent;
}
