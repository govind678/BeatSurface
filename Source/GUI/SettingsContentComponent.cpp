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
    audioSetupButton->setColour (TextButton::buttonColourId, Colour (0xFF5B5B5B));
    audioSetupButton->setColour (TextButton::buttonOnColourId, Colour (0xFF9A9A9A));
    
    
    addAndMakeVisible (velocitySensitivitySlider = new Slider ("velocitySensitivitySlider"));
    velocitySensitivitySlider->setRange (0.001, 1, 0.001);
    velocitySensitivitySlider->setSliderStyle (Slider::Rotary);
    velocitySensitivitySlider->setTextBoxStyle (Slider::TextBoxBelow, false, 50, 20);
    velocitySensitivitySlider->setColour (Slider::thumbColourId, Colour (0xFF464E56));
    velocitySensitivitySlider->setColour (Slider::rotarySliderFillColourId, Colour (0x7F464E55));
    velocitySensitivitySlider->setColour (Slider::textBoxTextColourId, Colour (0xFF404952));
    velocitySensitivitySlider->setColour (Slider::textBoxBackgroundColourId, Colour (0x00000000));
    velocitySensitivitySlider->setColour (Slider::textBoxHighlightColourId, Colour (0x00000000));
    velocitySensitivitySlider->setColour (Slider::textBoxOutlineColourId, Colour (0x00808080));
    velocitySensitivitySlider->setVelocityBasedMode(true);
    velocitySensitivitySlider->setVelocityModeParameters(0.01,1,0.0,true);
    
    
    addAndMakeVisible (decayTimeSensitivitySlider = new Slider ("decayTimeSensitivitySlider"));
    decayTimeSensitivitySlider->setRange (0.025, 1, 0.025);
    decayTimeSensitivitySlider->setSliderStyle (Slider::Rotary);
    decayTimeSensitivitySlider->setTextBoxStyle (Slider::TextBoxBelow, false, 50, 20);
    decayTimeSensitivitySlider->setColour (Slider::thumbColourId, Colour (0xFF464E56));
    decayTimeSensitivitySlider->setColour (Slider::rotarySliderFillColourId, Colour (0x7F464E55));
    decayTimeSensitivitySlider->setColour (Slider::textBoxTextColourId, Colour (0xFF404952));
    decayTimeSensitivitySlider->setColour (Slider::textBoxBackgroundColourId, Colour (0x00000000));
    decayTimeSensitivitySlider->setColour (Slider::textBoxHighlightColourId, Colour (0x00000000));
    decayTimeSensitivitySlider->setColour (Slider::textBoxOutlineColourId, Colour (0x00808080));
    decayTimeSensitivitySlider->setVelocityBasedMode(true);
    decayTimeSensitivitySlider->setVelocityModeParameters(0.01,1,0.0,true);
    
    
    
    
    addAndMakeVisible (velocitySensitivityLabel = new Label ("velocitySensitivityLabel",
                                                             TRANS("Velocity Sensitivity\n")));
    velocitySensitivityLabel->setFont (Font ("Myriad Pro", 11.90f, Font::bold));
    velocitySensitivityLabel->setJustificationType (Justification::centred);
    velocitySensitivityLabel->setEditable (false, false, false);
    velocitySensitivityLabel->setColour (Label::textColourId, Colour (0xFF5C6266));
    velocitySensitivityLabel->setColour (TextEditor::textColourId, Colour (0xff3d4248));
    velocitySensitivityLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    velocitySensitivityLabel->setColour (TextEditor::highlightColourId, Colour (0x00000000));
    
    
    
    
    addAndMakeVisible (decayTimeSensitivityLabel = new Label ("decayTimeSensitivityLabel",
                                                              TRANS("Decay Time Sensitivity")));
    decayTimeSensitivityLabel->setFont (Font ("Myriad Pro", 12.00f, Font::bold));
    decayTimeSensitivityLabel->setJustificationType (Justification::centred);
    decayTimeSensitivityLabel->setEditable (false, false, false);
    decayTimeSensitivityLabel->setColour (Label::textColourId, Colour (0xFF5C6266));
    decayTimeSensitivityLabel->setColour (TextEditor::textColourId, Colour (0xff3d4248));
    decayTimeSensitivityLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    decayTimeSensitivityLabel->setColour (TextEditor::highlightColourId, Colour (0x00000000));
    
    

    
    addAndMakeVisible (tempoNumBox = new Label ("tempoNumBox", TRANS("120.00")));
    tempoNumBox->setFont (Font ("Myriad Pro", 16.00f, Font::bold));
    tempoNumBox->setJustificationType (Justification::centred);
    tempoNumBox->setEditable (true, false, false);
    tempoNumBox->setColour(Label::textColourId, Colour (0xFF000000));
    tempoNumBox->setColour(Label::backgroundColourId, Colour(0xDD999999));
    tempoNumBox->setColour(Label::outlineColourId, Colour(0xFF000000));
    tempoNumBox->setColour(TextEditor::textColourId, Colour (0xFF000000));
    tempoNumBox->setColour(TextEditor::backgroundColourId, Colour (0xDD999999));
    tempoNumBox->setColour(TextEditor::highlightColourId, Colour (0xFFDDDDDD));
    tempoNumBox->setBorderSize(0, 0);
    
    
    addAndMakeVisible (numeratorNumBox = new Label ("numeratorNumBox", TRANS("4")));
    numeratorNumBox->setFont (Font ("Myriad Pro", 15.00f, Font::bold));
    numeratorNumBox->setJustificationType (Justification::centred);
    numeratorNumBox->setEditable (true, false, false);
    numeratorNumBox->setColour(Label::textColourId, Colour (0xFF000000));
    numeratorNumBox->setColour(Label::backgroundColourId, Colour(0xDD999999));
    numeratorNumBox->setColour(Label::outlineColourId, Colour(0xFF000000));
    numeratorNumBox->setColour(TextEditor::textColourId, Colour (0xFF000000));
    numeratorNumBox->setColour(TextEditor::backgroundColourId, Colour (0xDD999999));
    numeratorNumBox->setColour(TextEditor::highlightColourId, Colour (0xFFDDDDDD));
    numeratorNumBox->setBorderSize(0, 0);
    
    
    addAndMakeVisible (denominatorNumBox = new Label ("denominatorNumBox", TRANS("4")));
    denominatorNumBox->setFont (Font ("Myriad Pro", 15.00f, Font::bold));
    denominatorNumBox->setJustificationType (Justification::centred);
    denominatorNumBox->setEditable (true, false, false);
    denominatorNumBox->setColour(Label::textColourId, Colour (0xFF000000));
    denominatorNumBox->setColour(Label::backgroundColourId, Colour(0xDD999999));
    denominatorNumBox->setColour(Label::outlineColourId, Colour(0xFF000000));
    denominatorNumBox->setColour(TextEditor::textColourId, Colour (0xFF000000));
    denominatorNumBox->setColour(TextEditor::backgroundColourId, Colour (0xDD999999));
    denominatorNumBox->setColour(TextEditor::highlightColourId, Colour (0xFFDDDDDD));
    denominatorNumBox->setBorderSize(0, 0);
    
    
    
    addAndMakeVisible (tempoLabel = new Label ("tempoLabel", TRANS("Tempo")));
    tempoLabel->setFont (Font ("Myriad Pro", 14.00f, Font::bold));
    tempoLabel->setJustificationType (Justification::right);
    tempoLabel->setEditable (false, false, false);
    tempoLabel->setColour (Label::textColourId, Colour (0xFF667788));
    tempoLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    tempoLabel->setColour (TextEditor::highlightColourId, Colour (0x00000000));
    
    
    addAndMakeVisible (meterLabel = new Label ("meterLabel", TRANS("Meter")));
    meterLabel->setFont (Font ("Myriad Pro", 14.00f, Font::bold));
    meterLabel->setJustificationType (Justification::right);
    meterLabel->setEditable (false, false, false);
    meterLabel->setColour (Label::textColourId, Colour (0xFF667788));
    meterLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    meterLabel->setColour (TextEditor::highlightColourId, Colour (0x00000000));
    
    

    setSize (getParentWidth(), getParentHeight());
}



SettingsContentComponent::~SettingsContentComponent()
{
    velocitySensitivitySlider   = nullptr;
    decayTimeSensitivitySlider  = nullptr;
    velocitySensitivityLabel    = nullptr;
    decayTimeSensitivityLabel   = nullptr;
    audioSetupButton            = nullptr;
    tempoNumBox                 = nullptr;
    numeratorNumBox             = nullptr;
    denominatorNumBox           = nullptr;
    tempoLabel                  = nullptr;
    meterLabel                  = nullptr;
    
    deleteAllChildren();
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
    
    tempoNumBox          -> setBounds(getWidth() - 80, 90, 60, 20);
    numeratorNumBox      -> setBounds(getWidth() - 80, 110, 25, 20);
    denominatorNumBox    -> setBounds(getWidth() - 45, 110, 25, 20);
    
    tempoLabel           -> setBounds(getWidth() - 140, 90, 60, 20);
    meterLabel           -> setBounds(getWidth() - 140, 110, 60, 20);
}