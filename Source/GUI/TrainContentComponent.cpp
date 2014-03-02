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
    
    addAndMakeVisible (saveTrainingButton = new TextButton ("saveTrainingButton"));
    saveTrainingButton->setButtonText (TRANS("Save Training"));
    saveTrainingButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    saveTrainingButton->setColour (TextButton::buttonColourId, Colour (0xff3b3b3b));
    saveTrainingButton->setColour (TextButton::buttonOnColourId, Colour (0xff797979));
    
    addAndMakeVisible (loadTrainingButton = new TextButton ("loadTrainingButton"));
    loadTrainingButton->setButtonText (TRANS("Load Training"));
    loadTrainingButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    loadTrainingButton->setColour (TextButton::buttonColourId, Colour (0xff3b3b3b));
    loadTrainingButton->setColour (TextButton::buttonOnColourId, Colour (0xff797979));
    
    
    addAndMakeVisible (addClassButton = new ImageButton ("addClassButton"));
    addClassButton->setClickingTogglesState(false);
    addClassButton->setImages (false, true, true,
                                        ImageCache::getFromMemory (BinaryData::AddClass128_png, BinaryData::AddClass128_pngSize), 1.0f, Colour (0x00000000),
                                        Image::null, 0.7f, Colour (0x00000000),
                                        Image::null, 1.0f, Colour (0x77000000));
    
    
    addAndMakeVisible (deleteClassButton = new ImageButton ("deleteClassButton"));
    deleteClassButton->setClickingTogglesState(false);
    deleteClassButton->setTriggeredOnMouseDown(true);
    deleteClassButton->setImages (false, true, true,
                               ImageCache::getFromMemory (BinaryData::DeleteClass128_png, BinaryData::DeleteClass128_pngSize), 1.0f, Colour (0x00000000),
                               Image::null, 0.7f, Colour (0x00000000),
                               Image::null, 1.0f, Colour (0x77000000));
    
    
    
    //--- Labels ---//
    
    addAndMakeVisible (addClassLabel = new Label ("addClassLabel", TRANS("Add Class")));
    addClassLabel->setFont (Font ("Myriad Pro", 11.90f, Font::plain));
    addClassLabel->setJustificationType (Justification::centred);
    addClassLabel->setEditable (false, false, false);
    addClassLabel->setColour (Label::textColourId, Colour (0xFF5C6266));
    addClassLabel->setColour (TextEditor::textColourId, Colour (0xFF3D4248));
    addClassLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    addClassLabel->setColour (TextEditor::highlightColourId, Colour (0x00000000));
    
    addAndMakeVisible (deleteClassLabel = new Label ("deleteClassLabel", TRANS("Delete Class")));
    deleteClassLabel->setFont (Font ("Myriad Pro", 11.90f, Font::plain));
    deleteClassLabel->setJustificationType (Justification::centred);
    deleteClassLabel->setEditable (false, false, false);
    deleteClassLabel->setColour (Label::textColourId, Colour (0xFF5C6266));
    deleteClassLabel->setColour (TextEditor::textColourId, Colour (0xFF3D4248));
    deleteClassLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    deleteClassLabel->setColour (TextEditor::highlightColourId, Colour (0x00000000));
    
    
   
    
    setSize (BeatSurfaceBase::iDocumentInitWidth, BeatSurfaceBase::iDocumentInitHeight);

}

TrainContentComponent::~TrainContentComponent()
{
    saveTrainingButton              = nullptr;
    loadTrainingButton              = nullptr;

    addClassButton                  = nullptr;
    addClassLabel                   = nullptr;
    deleteClassButton               = nullptr;
    deleteClassLabel                = nullptr;
    
    deleteAllChildren();
}



void TrainContentComponent::paint (Graphics& g)
{
    g.fillAll (Colour (0xff2b2b2b));
}


void TrainContentComponent::resized()
{
    saveTrainingButton->setBounds ((getWidth() / 2) + -120 - ((120) / 2), (getHeight() / 2) + 60 - ((40) / 2), 120, 40);
    loadTrainingButton->setBounds ((getWidth() / 2) + 120 - ((120) / 2), (getHeight() / 2) + 60 - ((40) / 2), 120, 40);
    
    addClassButton->setBounds(20, 20, 64, 64);
    addClassLabel-> setBounds(20, 20 + 65, 64, 20);
    
    deleteClassButton->setBounds(20, 130, 64, 64);
    deleteClassLabel->setBounds(20, 130 + 65, 64, 20);

}



bool TrainContentComponent::keyPressed (const KeyPress& key)
{
    return false;  // Return true if your handler uses this key event, or false to allow it to be passed-on.
}

bool TrainContentComponent::keyStateChanged (const bool isKeyDown)
{
    return false;  // Return true if your handler uses this key event, or false to allow it to be passed-on.
}