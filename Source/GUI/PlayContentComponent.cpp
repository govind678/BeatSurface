/*
  ==============================================================================

    PlayContentComponent.cpp
    Created: 14 Feb 2014 2:41:27am
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#include "PlayContentComponent.h"



PlayContentComponent::PlayContentComponent ()
{
    
    addAndMakeVisible (audioStreamToggleButton = new ImageButton ("playButton"));
    audioStreamToggleButton->setClickingTogglesState(true);
    audioStreamToggleButton->setImages (false, true, true,
                                ImageCache::getFromMemory (BinaryData::Play128_png, BinaryData::Play128_pngSize), 1.0f, Colour (0x00000000),
                                Image::null, 0.7f, Colour (0x00000000),
                                ImageCache::getFromMemory (BinaryData::Stop128_png, BinaryData::Stop128_pngSize), 1.0f, Colour (0x00000000));
    
    
    addAndMakeVisible(liveAudioScroller = new LiveScrollingAudioDisplay);
    
    
    addAndMakeVisible(shapeButtonArray = new ClassButtonArray);

    
    setSize (getParentWidth(), getParentHeight());

}

PlayContentComponent::~PlayContentComponent()
{
    audioStreamToggleButton     = nullptr;
    liveAudioScroller           = nullptr;
    shapeButtonArray            = nullptr;

}



void PlayContentComponent::paint (Graphics& g)
{
    g.fillAll (Colour (0xff2b2b2b));
}

void PlayContentComponent::resized()
{
    //liveStreamToggleButton->setBounds ((getWidth() / 2) - ((120) / 2), (getHeight() / 2) - ((40) / 2), 120, 40);
    liveAudioScroller->setBounds(0, 0, proportionOfWidth (0.5000f), proportionOfHeight (0.2000f));
    audioStreamToggleButton->setBounds(5, getHeight()/2 , 64, 64);
    shapeButtonArray->setBounds(getWidth()/2 - getHeight()/4, getHeight()/2 - getHeight()/4 + 30, getHeight()/2, getHeight()/2);
}

