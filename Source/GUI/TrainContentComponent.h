/*
  ==============================================================================

    TrainContentComponent.h
    Created: 14 Feb 2014 2:43:50am
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#ifndef TRAINCONTENTCOMPONENT_H_INCLUDED
#define TRAINCONTENTCOMPONENT_H_INCLUDED


#include "BeatSurfaceHeader.h"

class TrainContentComponent  :      public Component
{
    
friend class MainComponent;

public:
   
    TrainContentComponent ();
    ~TrainContentComponent();
    

    
    void paint (Graphics& g);
    void resized();
    bool keyPressed (const KeyPress& key);
    bool keyStateChanged (const bool isKeyDown);
    
    
    
private:
    
    ScopedPointer<TextButton>       saveTrainingButton;
    ScopedPointer<TextButton>       loadTrainingButton;
    
    ScopedPointer<ImageButton>      addClassButton;
    ScopedPointer<ImageButton>      deleteClassButton;
    
    ScopedPointer<Label>            addClassLabel;
    ScopedPointer<Label>            deleteClassLabel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TrainContentComponent)
};


#endif  // TRAINCONTENTCOMPONENT_H_INCLUDED
