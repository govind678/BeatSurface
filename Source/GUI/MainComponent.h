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
#include "TrainContentComponent.h"
#include "SettingsContentComponent.h"


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
                                public ApplicationCommandTarget,
                                public Timer
{
    
public:
    
    //==============================================================================
    // Constructor and Destructor
    
    MainComponent();
    ~MainComponent();
    void initialize();
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
    // Timer
    
    void timerCallback();
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
    
    ScopedPointer<PlayContentComponent> playComponent;
    ScopedPointer<TrainContentComponent> trainComponent;
    ScopedPointer<SettingsContentComponent> settingsComponent;    
    
    ScopedPointer<TabbedComponent> tabbedComponent;
    
    ScopedPointer<ImageButton> beatSurfaceLogo;
    ScopedPointer<Label>       beatSurfaceLabel;
    ScopedPointer<Label>       gtcmtLabel;
    
    
    ScopedPointer<AudioSetupDisplay> audioSetup;
    
//    ScopedPointer<ClassButtonArray> shapeButtonArray;
    
    LookAndFeel_V3 lookAndFeelV3;
    
    //======================================================================
    
    
    
    
    //======================================================================
    // Beat Surface Engine - Internal Methods
    
    void addClass();
    void deleteClass(int classIndex);
    void launchPreferences();
    
    int     m_iNumClasses;
    int     m_iCurentSelectedClass;
    
    //======================================================================


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
