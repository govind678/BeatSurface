/*
  ==============================================================================

    SettingsContentComponent.cpp
    Created: 14 Feb 2014 2:41:44am
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#include "SettingsContentComponent.h"


SettingsContentComponent::SettingsContentComponent ()
{
    addAndMakeVisible (audioSetupButton = new TextButton ("audioSetupButton"));
    audioSetupButton->setButtonText (TRANS("Audio Setup"));
    audioSetupButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
//    audioSetupButton->addListener (TabbedContentComponent);
    audioSetupButton->setColour (TextButton::buttonColourId, Colour (0xff3b3b3b));
    audioSetupButton->setColour (TextButton::buttonOnColourId, Colour (0xff797979));
    
    
    addAndMakeVisible (velocitySensitivitySlider = new Slider ("velocitySensitivitySlider"));
    velocitySensitivitySlider->setRange (0.001, 1, 0.001);
    velocitySensitivitySlider->setSliderStyle (Slider::Rotary);
    velocitySensitivitySlider->setTextBoxStyle (Slider::TextBoxBelow, false, 50, 20);
    velocitySensitivitySlider->setColour (Slider::thumbColourId, Colour (0xff363e46));
    velocitySensitivitySlider->setColour (Slider::rotarySliderFillColourId, Colour (0x7f363e45));
    velocitySensitivitySlider->setColour (Slider::textBoxTextColourId, Colour (0xff404952));
    velocitySensitivitySlider->setColour (Slider::textBoxBackgroundColourId, Colour (0x00000000));
    velocitySensitivitySlider->setColour (Slider::textBoxHighlightColourId, Colour (0x00000000));
    velocitySensitivitySlider->setColour (Slider::textBoxOutlineColourId, Colour (0x00808080));
    velocitySensitivitySlider->setVelocityBasedMode(true);
    velocitySensitivitySlider->setVelocityModeParameters(0.01,1,0.0,true);
//    velocitySensitivitySlider->addListener (this);
    
    
    addAndMakeVisible (decayTimeSensitivitySlider = new Slider ("decayTimeSensitivitySlider"));
    decayTimeSensitivitySlider->setRange (0.025, 1, 0.025);
    decayTimeSensitivitySlider->setSliderStyle (Slider::Rotary);
    decayTimeSensitivitySlider->setTextBoxStyle (Slider::TextBoxBelow, false, 50, 20);
    decayTimeSensitivitySlider->setColour (Slider::thumbColourId, Colour (0xff363e46));
    decayTimeSensitivitySlider->setColour (Slider::rotarySliderFillColourId, Colour (0x7f363e45));
    decayTimeSensitivitySlider->setColour (Slider::textBoxTextColourId, Colour (0xff404952));
    decayTimeSensitivitySlider->setColour (Slider::textBoxBackgroundColourId, Colour (0x00000000));
    decayTimeSensitivitySlider->setColour (Slider::textBoxHighlightColourId, Colour (0x00000000));
    decayTimeSensitivitySlider->setColour (Slider::textBoxOutlineColourId, Colour (0x00808080));
    decayTimeSensitivitySlider->setVelocityBasedMode(true);
    decayTimeSensitivitySlider->setVelocityModeParameters(0.01,1,0.0,true);
//    decayTimeSensitivitySlider->addListener (this);
    
    
    
    addAndMakeVisible (velocitySensitivityLabel = new Label ("velocitySensitivityLabel",
                                                             TRANS("Velocity Sensitivity\n")));
    velocitySensitivityLabel->setFont (Font ("Myriad Pro", 11.90f, Font::bold));
    velocitySensitivityLabel->setJustificationType (Justification::centred);
    velocitySensitivityLabel->setEditable (false, false, false);
    velocitySensitivityLabel->setColour (Label::textColourId, Colour (0xff4c5256));
    velocitySensitivityLabel->setColour (TextEditor::textColourId, Colour (0xff3d4248));
    velocitySensitivityLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    velocitySensitivityLabel->setColour (TextEditor::highlightColourId, Colour (0x00000000));
    
    
    
    
    addAndMakeVisible (decayTimeSensitivityLabel = new Label ("decayTimeSensitivityLabel",
                                                              TRANS("Decay Time Sensitivity")));
    decayTimeSensitivityLabel->setFont (Font ("Myriad Pro", 12.00f, Font::bold));
    decayTimeSensitivityLabel->setJustificationType (Justification::centred);
    decayTimeSensitivityLabel->setEditable (false, false, false);
    decayTimeSensitivityLabel->setColour (Label::textColourId, Colour (0xff4c5256));
    decayTimeSensitivityLabel->setColour (TextEditor::textColourId, Colour (0xff3d4248));
    decayTimeSensitivityLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    decayTimeSensitivityLabel->setColour (TextEditor::highlightColourId, Colour (0x00000000));


    setSize (getParentWidth(), getParentHeight());
}



SettingsContentComponent::~SettingsContentComponent()
{
    velocitySensitivitySlider   = nullptr;
    decayTimeSensitivitySlider  = nullptr;
    velocitySensitivityLabel    = nullptr;
    decayTimeSensitivityLabel   = nullptr;
    audioSetupButton            = nullptr;
}



void SettingsContentComponent::paint (Graphics& g)
{
    g.fillAll (Colour (0xff2b2b2b));
}


void SettingsContentComponent::resized()
{
    audioSetupButton->setBounds((getWidth() / 2) - ((120) / 2), (getHeight() / 2) - ((40) / 2) - 20, 120, 40);
    velocitySensitivitySlider->setBounds ((getWidth() / 2) + -90 - ((90) / 2), (getHeight() / 2) + 90 - ((90) / 2), 90, 90);
    decayTimeSensitivitySlider->setBounds ((getWidth() / 2) + 90 - ((90) / 2), (getHeight() / 2) + 90 - ((90) / 2), 90, 90);
    velocitySensitivityLabel->setBounds ((getWidth() / 2) + -90 - ((150) / 2), (getHeight() / 2) + 145 - ((20) / 2), 150, 20);
    decayTimeSensitivityLabel->setBounds ((getWidth() / 2) + 90 - ((150) / 2), (getHeight() / 2) + 145 - ((20) / 2), 150, 20);
}





bool SettingsContentComponent::keyPressed (const KeyPress& key)
{
    return false;  // Return true if your handler uses this key event, or false to allow it to be passed-on.
}


bool SettingsContentComponent::keyStateChanged (const bool isKeyDown)
{
    return false;  // Return true if your handler uses this key event, or false to allow it to be passed-on.
}