/*
  ==============================================================================

    SettingsContentComponent.h
    Created: 14 Feb 2014 2:41:44am
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#ifndef SETTINGSCONTENTCOMPONENT_H_INCLUDED
#define SETTINGSCONTENTCOMPONENT_H_INCLUDED


#include "BeatSurfaceHeader.h"

class SettingsContentComponent  :   public Component
{
    
friend class MainComponent;

public:
    
    SettingsContentComponent ();
    ~SettingsContentComponent();
    
    
    void paint (Graphics& g);
    void resized();    
    
    
private:

    
    ScopedPointer<Slider>           velocitySensitivitySlider;
    ScopedPointer<Slider>           decayTimeSensitivitySlider;
    ScopedPointer<Label>            velocitySensitivityLabel;
    ScopedPointer<Label>            decayTimeSensitivityLabel;
    ScopedPointer<TextButton>       audioSetupButton;
    
    ScopedPointer<Label>            tempoNumBox;
    ScopedPointer<Label>            numeratorNumBox;
    ScopedPointer<Label>            denominatorNumBox;
    
    ScopedPointer<Label>            tempoLabel;
    ScopedPointer<Label>            meterLabel;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SettingsContentComponent)
};


#endif  // SETTINGSCONTENTCOMPONENT_H_INCLUDED
