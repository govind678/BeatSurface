/*
  ==============================================================================

    MainComponent.h
    Created: 14 Feb 2014 2:41:14am
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "JuceHeader.h"
#include "BeatSurfaceHeader.h"


#include "PlayContentComponent.h"
#include "SettingsContentComponent.h"

#include <stdio.h>
#include <vector>

//========================================================================================================================

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
    // Tabbed Component Methods
    void currentTabChanged(int newCurrentTabIndex, const String& newCurrentTabName);
    //==============================================================================
    
    
    
    
    //======================================================================
    // GUI Components
    
    //--- Main Tabs ---//
    ScopedPointer<PlayContentComponent> playComponent;
    ScopedPointer<SettingsContentComponent> settingsComponent;    
    
    ScopedPointer<TabbedComponent> tabbedComponent;
    
    
    //--- Logos ---//
    ScopedPointer<ImageButton> beatSurfaceLogo;
    ScopedPointer<Label>       beatSurfaceLabel;
    ScopedPointer<Label>       gtcmtLabel;
    
    
    //--- Audio Device Setup Component Window ---//
    ScopedPointer<AudioSetupDisplay> audioSetup;
    
    //--- Metronome Display ---//
    ScopedPointer<ClockDisplayComponent> clockDisplay;
    
    
    //--- Look and Feel Class (Must Work on it!) ---//
    LookAndFeel_V3 lookAndFeelV3;
    
    //======================================================================
    
    
    
    
    //======================================================================
    // Internal Methods and Member Variables
    
    //--- Beat Surface Engine ---//
    void addClass();
    void deleteClass(int classIndex);
    
    int                             m_iNumClasses;
    int                             m_iCurentSelectedClass;
    
    std::vector<float>              m_pfOnsetProbabilities;
    
    
    
    //--- Launch Audio Setup Preferences ---//
    void launchPreferences();
    
    //======================================================================


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
