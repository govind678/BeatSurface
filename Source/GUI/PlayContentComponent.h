/*
  ==============================================================================

    PlayContentComponent.h
    Created: 14 Feb 2014 2:41:27am
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#ifndef PLAYCONTENTCOMPONENT_H_INCLUDED
#define PLAYCONTENTCOMPONENT_H_INCLUDED


#include "BeatSurfaceHeader.h"
#include "AudioLiveScrollDisplay.h"


class PlayContentComponent  :   public Component
{

friend class MainComponent;

public:
    
    PlayContentComponent ();
    ~PlayContentComponent();


    void paint (Graphics& g);
    void resized();
//    void buttonClicked (Button* buttonThatWasClicked);
    bool keyPressed (const KeyPress& key);
    bool keyStateChanged (const bool isKeyDown);
    
    
    
private:
    
    //ScopedPointer<TextButton>                   liveStreamToggleButton;
    ScopedPointer<ImageButton>                  audioStreamToggleButton;
    ScopedPointer<LiveScrollingAudioDisplay>    liveAudioScroller;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlayContentComponent)
};


#endif  // PLAYCONTENTCOMPONENT_H_INCLUDED
