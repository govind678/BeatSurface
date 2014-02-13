/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "BeatSurface.h"


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   :  public Component,
                                public ButtonListener,
                                public SliderListener
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

    void paint (Graphics&);
    void resized();

private:
    //==============================================================================
    
    
    void buttonClicked(Button* buttonThatWasClicked);
    void sliderValueChanged (Slider* sliderValueThatHasChanged);
    
    
    //--- GUI Components ---//
    ScopedPointer<TextButton> audioToggleButton;
    ScopedPointer<TextButton> readAudioFileButton;
    
    ScopedPointer<TextButton> saveTrainingButton;
    ScopedPointer<TextButton> loadTrainingButton;
    
    ScopedPointer<TextButton> trainClass1Button;
    ScopedPointer<TextButton> trainClass2Button;
    ScopedPointer<TextButton> trainClass3Button;
    
    ScopedPointer<Slider> velocitySensitivitySlider;
    ScopedPointer<Label>  velocitySensitivityLabel;
    ScopedPointer<Slider> decayTimeSensitivitySlider;
    ScopedPointer<Label>  decayTimeSensitivityLabel;
    
    Colour* backgroundColour;
    
    
    
    //--- Interface ---//
    BeatSurface* beatSurface;
    
    
    //--- Audio ---//
    int iNumInputChannels;
    int iNumOutputChannels;
    AudioDeviceManager deviceManager;
    AudioDeviceManager::AudioDeviceSetup deviceSetup;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
