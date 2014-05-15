/*
  ==============================================================================

    PlayContentComponent.cpp
    Created: 14 Feb 2014 2:41:27am
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#include "PlayContentComponent.h"



ToolBarControls::ToolBarControls()
{
    
    addAndMakeVisible (audioStreamToggleButton = new ImageButton ("playButton"));
    audioStreamToggleButton->setClickingTogglesState(true);
    audioStreamToggleButton->setImages (false, true, true,
                                        ImageCache::getFromMemory (BinaryData::Play128_png, BinaryData::Play128_pngSize), 1.0f, Colour (0x00000000),
                                        Image::null, 0.7f, Colour (0x00000000),
                                        ImageCache::getFromMemory (BinaryData::Stop128_png, BinaryData::Stop128_pngSize), 1.0f, Colour (0x00000000));
    
    
    addAndMakeVisible (recordToggleButton = new ImageButton ("recordButton"));
    recordToggleButton->setClickingTogglesState(true);
    recordToggleButton->setImages (false, true, true,
                                   ImageCache::getFromMemory (BinaryData::RecordOff128_png, BinaryData::RecordOff128_pngSize), 1.0f, Colour (0x00000000),
                                   Image::null, 0.7f, Colour (0x00000000),
                                   ImageCache::getFromMemory (BinaryData::RecordOn128_png, BinaryData::RecordOn128_pngSize), 1.0f, Colour (0x00000000));
    
    addAndMakeVisible (saveTrainingButton = new ImageButton ("saveTrainingButton"));
    saveTrainingButton->setClickingTogglesState(false);
    saveTrainingButton->setImages (false, true, true,
                                   ImageCache::getFromMemory (BinaryData::Save128_png, BinaryData::Save128_pngSize), 1.0f, Colour (0x00000000),
                                   Image::null, 0.7f, Colour (0x00000000),
                                   Image::null, 1.0f, Colour (0x77000000));
    
    
    addAndMakeVisible (loadTrainingButton = new ImageButton ("loadTrainingButton"));
    loadTrainingButton->setClickingTogglesState(false);
    loadTrainingButton->setImages (false, true, true,
                                   ImageCache::getFromMemory (BinaryData::Load128_png, BinaryData::Load128_pngSize), 1.0f, Colour (0x00000000),
                                   Image::null, 0.7f, Colour (0x00000000),
                                   Image::null, 1.0f, Colour (0x77000000));
    
    
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
    
    addAndMakeVisible (trainClassLabel = new Label ("trainClassLabel", TRANS("Train")));
    trainClassLabel->setFont (Font ("Myriad Pro", 11.90f, Font::plain));
    trainClassLabel->setJustificationType (Justification::centred);
    trainClassLabel->setEditable (false, false, false);
    trainClassLabel->setColour (Label::textColourId, Colour (0xFF5C6266));
    trainClassLabel->setColour (TextEditor::textColourId, Colour (0xFF3D4248));
    trainClassLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    trainClassLabel->setColour (TextEditor::highlightColourId, Colour (0x00000000));
    
    addAndMakeVisible (playLabel = new Label ("playLabel", TRANS("Play")));
    playLabel->setFont (Font ("Myriad Pro", 11.90f, Font::plain));
    playLabel->setJustificationType (Justification::centred);
    playLabel->setEditable (false, false, false);
    playLabel->setColour (Label::textColourId, Colour (0xFF5C6266));
    playLabel->setColour (TextEditor::textColourId, Colour (0xFF3D4248));
    playLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    playLabel->setColour (TextEditor::highlightColourId, Colour (0x00000000));
    
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
    
    addAndMakeVisible (saveTrainingLabel = new Label ("saveTrainingLabel", TRANS("Save")));
    saveTrainingLabel->setFont (Font ("Myriad Pro", 11.90f, Font::plain));
    saveTrainingLabel->setJustificationType (Justification::centred);
    saveTrainingLabel->setEditable (false, false, false);
    saveTrainingLabel->setColour (Label::textColourId, Colour (0xFF5C6266));
    saveTrainingLabel->setColour (TextEditor::textColourId, Colour (0xFF3D4248));
    saveTrainingLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    saveTrainingLabel->setColour (TextEditor::highlightColourId, Colour (0x00000000));
    
    
    addAndMakeVisible (loadTrainingLabel = new Label ("loadTrainingLabel", TRANS("Load")));
    loadTrainingLabel->setFont (Font ("Myriad Pro", 11.90f, Font::plain));
    loadTrainingLabel->setJustificationType (Justification::centred);
    loadTrainingLabel->setEditable (false, false, false);
    loadTrainingLabel->setColour (Label::textColourId, Colour (0xFF5C6266));
    loadTrainingLabel->setColour (TextEditor::textColourId, Colour (0xFF3D4248));
    loadTrainingLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    loadTrainingLabel->setColour (TextEditor::highlightColourId, Colour (0x00000000));
    
//    setSize (getParentWidth(), getParentHeight());
    
}


ToolBarControls::~ToolBarControls()
{
    audioStreamToggleButton         = nullptr;
    recordToggleButton              = nullptr;
    trainClassLabel                 = nullptr;
    playLabel                       = nullptr;
    
    saveTrainingButton              = nullptr;
    loadTrainingButton              = nullptr;
    saveTrainingLabel               = nullptr;
    loadTrainingLabel               = nullptr;
    
    addClassButton                  = nullptr;
    addClassLabel                   = nullptr;
    deleteClassButton               = nullptr;
    deleteClassLabel                = nullptr;
    
    deleteAllChildren();
}



void ToolBarControls::paint(juce::Graphics &g)
{
    
}


void ToolBarControls::resized()
{
#if JUCE_MAC || JUCE_LINUX || JUCE_WINDOWS
    
    audioStreamToggleButton->setBounds(88, 110, 64, 64);
    playLabel->setBounds(88, 110 + 65, 64, 20);
    
    recordToggleButton->setBounds(88, 220, 64, 64);
    trainClassLabel->setBounds(88, 220 + 65, 64, 20);
    
    recordToggleButton->setBounds(88, 220, 64, 64);
    trainClassLabel->setBounds(88, 220 + 65, 64, 20);
    
    
    addClassButton->setBounds(0, 0, 64, 64);
    addClassLabel-> setBounds(0, 65, 64, 20);
    
    deleteClassButton->setBounds(0, 110, 64, 64);
    deleteClassLabel->setBounds(-1, 110 + 65, 66, 20);
    
    saveTrainingButton->setBounds (0, 220, 64, 64);
    saveTrainingLabel->setBounds(0, 220 + 65, 64, 20);
    
    loadTrainingButton->setBounds (0, 330, 64, 64);
    loadTrainingLabel->setBounds(0, 330 + 65, 64, 20);
    
    
#elif JUCE_IOS || JUCE_ANDROID
    
    addClassButton->setBounds(0, 0, 32, 32);
    addClassLabel-> setBounds(0, 33, 32, 20);
    
    deleteClassButton->setBounds(0, 55, 32, 32);
    deleteClassLabel->setBounds(-1, 55 + 32, 33, 20);
    
    audioStreamToggleButton->setBounds(44, 55, 32, 32);
    playLabel->setBounds(44, 55 + 33, 32, 20);
    
    recordToggleButton->setBounds(44, 110, 32, 32);
    trainClassLabel->setBounds(44, 110 + 33, 32, 20);
    
    saveTrainingButton->setBounds (0, 110, 32, 32);
    saveTrainingLabel->setBounds(0, 110 + 33, 32, 20);
    
    loadTrainingButton->setBounds (0, 165, 32, 32);
    loadTrainingLabel->setBounds(0, 165 + 33, 32, 20);
    
#endif
}







//========================================================================================================================


PlayContentComponent::PlayContentComponent ()
{
    addAndMakeVisible(toolBarControls = new ToolBarControls());
    
    
    addAndMakeVisible(waveformLiveScroller = new LiveScrollDisplay(0));
    
    addAndMakeVisible(spectrumLiveScroller = new LiveScrollDisplay(1));
    
    
    addAndMakeVisible(shapeButtonArray = new ClassButtonArray);
    
    addAndMakeVisible(metroDisplay = new MetronomeComponent);

    
    setSize (getParentWidth(), getParentHeight());

}

PlayContentComponent::~PlayContentComponent()
{
    toolBarControls                 = nullptr;
    
    waveformLiveScroller            = nullptr;
    spectrumLiveScroller            = nullptr;
    
    shapeButtonArray                = nullptr;
    metroDisplay                    = nullptr;
    
    deleteAllChildren();
}



void PlayContentComponent::paint (Graphics& g)
{
    g.fillAll (Colour (0xff2b2b2b));
}

void PlayContentComponent::resized()
{
    waveformLiveScroller->setBounds(0, 0, proportionOfWidth (0.5000f), proportionOfHeight (0.2000f));
    spectrumLiveScroller->setBounds(proportionOfWidth (0.5000f), 0, proportionOfWidth (0.5000f), proportionOfHeight (0.2000f));
    
#if JUCE_MAC || JUCE_WINDOWS || JUCE_LINUX
    toolBarControls->setBounds(10, proportionOfHeight (0.2000f) + 44, 152, 420);
#elif JUCE_IOS || JUCE_ANDROID
    toolBarControls->setBounds(5, proportionOfHeight (0.2000f), 120, 210);
#endif
    
    shapeButtonArray->setBounds(getWidth()/2 - getHeight()/4, getHeight()/2 - getHeight()/4 + 30, getHeight()/2, getHeight()/2);
    metroDisplay->setBounds(getWidth()/2 - (getHeight()/4 + 10), getHeight()/2 - getHeight()/4 + 20, getHeight()/2 + 20, getHeight()/2 + 20);
}

