/*
  ==============================================================================

    MainComponent.h
    Created: 14 Feb 2014 2:41:14am
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "BeatSurfaceHeader.h"

#include "PlayContentComponent.h"
#include "SettingsContentComponent.h"
#include "TranscriptionComponent.h"

#include <stdio.h>
#include <vector>

//========================================================================================================================

/**
    GUI class to set audio preferences
*/

class AudioSetupDisplay : public Component
{
    
public:
    AudioSetupDisplay();
    ~AudioSetupDisplay();
    
    void paint (Graphics& g);
    void resized();
    
    ScopedPointer<TextButton> applyAudioSettingsButton;
    
private:
    
    ScopedPointer<AudioDeviceSelectorComponent> deviceSelectorComponent;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioSetupDisplay)
};




//========================================================================================================================


/**
    Main Component running inside the window. Implements all the GUI action listeners.
 */

class MainComponent    :        public Component,
                                public ButtonListener,
                                public SliderListener,
                                public LabelListener,
                                public ApplicationCommandTarget,
                                public Timer,
                                public ActionListener
{
    
public:
    
    //==============================================================================
    // Constructor and Destructor
    MainComponent();
    ~MainComponent();
    //==============================================================================
    
    
    //==============================================================================
    // Component
    void paint (Graphics& g);
    void resized();
    void focusGained (FocusChangeType cause);
    void focusLost (FocusChangeType cause);
    //==============================================================================
    
    
    //==============================================================================
    // Button Listener
    
    void buttonClicked (Button* buttonThatWasClicked);
    void loadAudioFileToPlay (int index);
    void setAudioFileParam (int index, bool include, bool looping, bool toggle);
    void setMidiOutput(int classIndex, bool include, int channelNo, int noteNum, int duration_ms);
    //==============================================================================
    
    
    //==============================================================================
    // Slider Listener
    void sliderValueChanged (Slider* sliderThatWasMoved);
    //==============================================================================
    
    
    //==============================================================================
    // Label Listener
    void labelTextChanged (Label *labelThatHasChanged);
    //==============================================================================
    
    
    //==============================================================================
    // Timer to Update GUI
    void timerCallback();
    //==============================================================================
    
    
    
    //==============================================================================
    // Action Listener Callback
    void actionListenerCallback (const String &message);
    //==============================================================================
    
    
    //==============================================================================
    // Application Command Target
	ApplicationCommandTarget* getNextCommandTarget();
	void getAllCommands (Array <CommandID>& commands);
	void getCommandInfo (const CommandID commandID, ApplicationCommandInfo& result);
	bool perform (const InvocationInfo& info);
    ApplicationCommandManager* getCommandManager();
    //==============================================================================
    
    
    
    
private:
    
    //==============================================================================
    // Update
    void updateGUIOnLoadTraining();
    //==============================================================================
    
    
    
    
    //======================================================================
    // GUI Components
    
    //--- Main Tabs ---//
    ScopedPointer<PlayContentComponent>     playComponent;
    ScopedPointer<SettingsContentComponent> settingsComponent;
    ScopedPointer<TranscriptionComponent>   transcriptionComponent;
    
    ScopedPointer<TabbedComponent> tabbedComponent;
    
    
    //--- Logos ---//
    ScopedPointer<ImageButton> beatSurfaceLogo;
    ScopedPointer<Label>       beatSurfaceLabel;
    ScopedPointer<Label>       gtcmtLabel;
    
    
    //--- Audio Device Setup Component Window ---//
    ScopedPointer<AudioSetupDisplay> audioSetup;
    
    
    //--- Confidence Progress Bar ---//
    ScopedPointer<Slider>      confidenceDisplayBar;
    
    
    //--- Look and Feel Class (Must Work on it!) ---//
    LookAndFeel_V3 lookAndFeelV3;
    
    //======================================================================
    
    
    
    
    //======================================================================
    // Internal Methods and Member Variables
    
    //--- Beat Surface Engine ---//
    void addClass(bool updateBackend);
    void deleteClass(int classIndex, bool updateBackend);
    
    int                             m_iNumClasses;
    int                             m_iCurentSelectedClass;
    
    
    std::vector<float>              m_pfOnsetProbabilities;
    
    
    
    //--- Launch Audio Setup Preferences ---//
    void launchPreferences();
    
    //======================================================================


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
