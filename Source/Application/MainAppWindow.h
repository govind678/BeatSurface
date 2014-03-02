/*
  ==============================================================================

    MainAppWindow.h
    Created: 13 Feb 2014 6:04:01pm
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#ifndef MAINAPPWINDOW_H_INCLUDED
#define MAINAPPWINDOW_H_INCLUDED

#include "JuceHeader.h"
#include "MainComponent.h"


class MainAppWindow   : public DocumentWindow
{
public:

    MainAppWindow();
    ~MainAppWindow();
    
    // return the MainWindow if it exists
    static MainAppWindow* getMainAppWindow();
    
    // called by the OS when the window's close button is pressed.
    void closeButtonPressed() override;
    
//    // return the Shared Audio Device Manager
//    static AudioDeviceManager& getSharedAudioDeviceManager();
    
//    // return the command manager object used to dispatch command events
//    static ApplicationCommandManager& getApplicationCommandManager();

    MainComponent* getMainComponent();
    
    
private:
    
    MainComponent* mainComponent;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainAppWindow)
};


#endif  // MAINWINDOW_H_INCLUDED
