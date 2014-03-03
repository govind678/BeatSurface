/*
  ==============================================================================

    TrainContentComponent.cpp
    Created: 14 Feb 2014 2:43:50am
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#include "TrainContentComponent.h"



TrainContentComponent::TrainContentComponent ()
{
    //--- Buttons ---//
    
    
    addAndMakeVisible(toolbarButtons = new Component);
    
    toolbarButtons->addAndMakeVisible (saveTrainingButton = new ImageButton ("saveTrainingButton"));
    saveTrainingButton->setClickingTogglesState(false);
    saveTrainingButton->setImages (false, true, true,
                               ImageCache::getFromMemory (BinaryData::Save128_png, BinaryData::Save128_pngSize), 1.0f, Colour (0x00000000),
                               Image::null, 0.7f, Colour (0x00000000),
                               Image::null, 1.0f, Colour (0x77000000));
    
    
    toolbarButtons->addAndMakeVisible (loadTrainingButton = new ImageButton ("loadTrainingButton"));
    loadTrainingButton->setClickingTogglesState(false);
    loadTrainingButton->setImages (false, true, true,
                               ImageCache::getFromMemory (BinaryData::Load128_png, BinaryData::Load128_pngSize), 1.0f, Colour (0x00000000),
                               Image::null, 0.7f, Colour (0x00000000),
                               Image::null, 1.0f, Colour (0x77000000));
    
    
    toolbarButtons->addAndMakeVisible (addClassButton = new ImageButton ("addClassButton"));
    addClassButton->setClickingTogglesState(false);
    addClassButton->setImages (false, true, true,
                                        ImageCache::getFromMemory (BinaryData::AddClass128_png, BinaryData::AddClass128_pngSize), 1.0f, Colour (0x00000000),
                                        Image::null, 0.7f, Colour (0x00000000),
                                        Image::null, 1.0f, Colour (0x77000000));
    
    
    toolbarButtons->addAndMakeVisible (deleteClassButton = new ImageButton ("deleteClassButton"));
    deleteClassButton->setClickingTogglesState(false);
    deleteClassButton->setTriggeredOnMouseDown(true);
    deleteClassButton->setImages (false, true, true,
                               ImageCache::getFromMemory (BinaryData::DeleteClass128_png, BinaryData::DeleteClass128_pngSize), 1.0f, Colour (0x00000000),
                               Image::null, 0.7f, Colour (0x00000000),
                               Image::null, 1.0f, Colour (0x77000000));
    
    
    
    //--- Labels ---//
    
    toolbarButtons->addAndMakeVisible (addClassLabel = new Label ("addClassLabel", TRANS("Add Class")));
    addClassLabel->setFont (Font ("Myriad Pro", 11.90f, Font::plain));
    addClassLabel->setJustificationType (Justification::centred);
    addClassLabel->setEditable (false, false, false);
    addClassLabel->setColour (Label::textColourId, Colour (0xFF5C6266));
    addClassLabel->setColour (TextEditor::textColourId, Colour (0xFF3D4248));
    addClassLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    addClassLabel->setColour (TextEditor::highlightColourId, Colour (0x00000000));
    
    toolbarButtons->addAndMakeVisible (deleteClassLabel = new Label ("deleteClassLabel", TRANS("Delete Class")));
    deleteClassLabel->setFont (Font ("Myriad Pro", 11.90f, Font::plain));
    deleteClassLabel->setJustificationType (Justification::centred);
    deleteClassLabel->setEditable (false, false, false);
    deleteClassLabel->setColour (Label::textColourId, Colour (0xFF5C6266));
    deleteClassLabel->setColour (TextEditor::textColourId, Colour (0xFF3D4248));
    deleteClassLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    deleteClassLabel->setColour (TextEditor::highlightColourId, Colour (0x00000000));
    
    toolbarButtons->addAndMakeVisible (saveTrainingLabel = new Label ("saveTrainingLabel", TRANS("Save")));
    saveTrainingLabel->setFont (Font ("Myriad Pro", 11.90f, Font::plain));
    saveTrainingLabel->setJustificationType (Justification::centred);
    saveTrainingLabel->setEditable (false, false, false);
    saveTrainingLabel->setColour (Label::textColourId, Colour (0xFF5C6266));
    saveTrainingLabel->setColour (TextEditor::textColourId, Colour (0xFF3D4248));
    saveTrainingLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    saveTrainingLabel->setColour (TextEditor::highlightColourId, Colour (0x00000000));
    
    toolbarButtons->addAndMakeVisible (loadTrainingLabel = new Label ("loadTrainingLabel", TRANS("Load")));
    loadTrainingLabel->setFont (Font ("Myriad Pro", 11.90f, Font::plain));
    loadTrainingLabel->setJustificationType (Justification::centred);
    loadTrainingLabel->setEditable (false, false, false);
    loadTrainingLabel->setColour (Label::textColourId, Colour (0xFF5C6266));
    loadTrainingLabel->setColour (TextEditor::textColourId, Colour (0xFF3D4248));
    loadTrainingLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    loadTrainingLabel->setColour (TextEditor::highlightColourId, Colour (0x00000000));
    
    
   
    addAndMakeVisible(shapeButtonArray = new ClassButtonArray);
    
    setSize (getParentWidth(), getParentHeight());

}

TrainContentComponent::~TrainContentComponent()
{
    saveTrainingButton              = nullptr;
    loadTrainingButton              = nullptr;
    saveTrainingLabel               = nullptr;
    loadTrainingLabel               = nullptr;

    addClassButton                  = nullptr;
    addClassLabel                   = nullptr;
    deleteClassButton               = nullptr;
    deleteClassLabel                = nullptr;
    
    toolbarButtons                  = nullptr;
    
    shapeButtonArray                = nullptr;
    
    deleteAllChildren();
}



void TrainContentComponent::paint (Graphics& g)
{
    g.fillAll (Colour (0xff2b2b2b));
}


void TrainContentComponent::resized()
{
    toolbarButtons->setBounds(20, getHeight()/2 - 210, 64, 420);
    
    addClassButton->setBounds(0, 0, 64, 64);
    addClassLabel-> setBounds(0, 65, 64, 20);
    
    deleteClassButton->setBounds(0, 110, 64, 64);
    deleteClassLabel->setBounds(-2, 110 + 65, 66, 20);
    
    saveTrainingButton->setBounds (0, 220, 64, 64);
    saveTrainingLabel->setBounds(0, 220 + 65, 64, 20);
    
    loadTrainingButton->setBounds (0, 330, 64, 64);
    loadTrainingLabel->setBounds(0, 330 + 65, 64, 20);
    
    shapeButtonArray->setBounds(getWidth()/2 - getHeight()/4, getHeight()/2 - getHeight()/4 + 30, getHeight()/2, getHeight()/2);

}