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

#include "CustomShapeButton.h"
#include "PlayContentComponent.h"
#include "TrainContentComponent.h"
#include "SettingsContentComponent.h"

#include "BeatSurfaceEngine.h"


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


class ClassButtonArray  :   public Component
{
public:
    
    ClassButtonArray();
    ~ClassButtonArray();
    
    void paint (Graphics& g);
    void resized();
    
    void setNumClasses(int numClasses);
    void setArrayAlpha(float* alphaValues);
    
    OwnedArray<CustomShapeButton> m_pcClassButton;
    
private:

    int m_iNumClasses;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ClassButtonArray)
};



//========================================================================================================================

class MainComponent    :       public Component,
                               public ButtonListener,
                               public SliderListener,
                               public Timer
{
    
public:
    
    MainComponent();
    ~MainComponent();
    
    
    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);
    void sliderValueChanged (Slider* sliderThatWasMoved);
    void focusGained (FocusChangeType cause);
    void focusLost (FocusChangeType cause);
    void timerCallback();
    
    
    
    
private:
    
    void currentTabChanged(int newCurrentTabIndex, const String& newCurrentTabName);
    
    PlayContentComponent* playComponent;
    TrainContentComponent* trainComponent;
    SettingsContentComponent* settingsComponent;
    
    
    ScopedPointer<TabbedComponent> tabbedComponent;
    
    ScopedPointer<ImageButton> beatSurfaceLogo;
    ScopedPointer<Label>       beatSurfaceLabel;
    ScopedPointer<Label>       gtcmtLabel;
    
    ScopedPointer<Slider>      classProbSlider;
    ScopedPointer<Slider>      classNumSelectorSlider;
    
    ScopedPointer<AudioSetupDisplay> audioSetup;
    
    ScopedPointer<ClassButtonArray> shapeButtonArray;
    
    LookAndFeel_V3 lookAndFeelV3;
    
    ScopedPointer<BeatSurfaceEngine> beatSurfaceEngine;

    ScopedPointer<float> m_pfCurrentOnsetProbabilities;
    
    int     m_iNumClasses;
    int     m_iCurentSelectedClass;
    
    
    
    
    
    
    //======================================================================
    // Application Command Target Stuff
    //======================================================================
    
//    ApplicationCommandTarget* getNextCommandTarget() override;
//    void getAllCommands (Array<CommandID>& commands) override;
//    void getCommandInfo (CommandID commandID, ApplicationCommandInfo& result) override;
//    bool perform (const InvocationInfo& info) override;
    
    
    
    //======================================================================


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
