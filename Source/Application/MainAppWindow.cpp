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
ScopedPointer<GUIUpdater>   guiUpdater;
ScopedPointer<GlobalClock>  globalClock;


MainAppWindow::MainAppWindow()  : DocumentWindow (JUCEApplication::getInstance()->getApplicationName(),
                                                  Colours::lightgrey,
                                                  DocumentWindow::allButtons)
{
    setUsingNativeTitleBar (true);
    
#if JUCE_MAC || JUCE_WINDOWS || JUCE_LINUX
    setResizable (true, false);
    setResizeLimits (200, 200, 10000, 10000);
    setBounds (0, 0, getParentWidth(), getParentHeight());
    
#elif JUCE_ANDROID || JUCE_IOS
    setResizable (false, false);
//    setResizeLimits (getParentWidth(), getParentHeight(), getParentWidth(), getParentHeight());
    setTitleBarHeight(0);
//    setFullScreen(true);
//    setBounds (0, 0, getParentHeight(), getParentWidth());
#endif

    
    std::cout << "Width: " << getParentWidth() << " Height: " << getParentHeight() << std::endl;
    
    mainComponent = new MainComponent;
    setContentOwned(mainComponent, true);
    
#if JUCE_MAC
    centreWithSize (getParentWidth(), getParentHeight() - 15);
#elif JUCE_IOS
    centreWithSize (getParentHeight(), getParentWidth());
#endif
    
    addKeyListener (commandManager->getKeyMappings());
    
    globalClock->setMainComponent(mainComponent);
    
    
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
