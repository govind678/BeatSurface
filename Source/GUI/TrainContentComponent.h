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


//========================================================================================================================

class TrainContentComponent  :      public Component
{
    
friend class MainComponent;

public:
   
    TrainContentComponent ();
    ~TrainContentComponent();
    

    
    void paint (Graphics& g);
    void resized();
    
    
private:
    
    ScopedPointer<ClassButtonArray> shapeButtonArray;
    
    ScopedPointer<Component>        toolbarButtons;
    
    ScopedPointer<ImageButton>      addClassButton;
    ScopedPointer<ImageButton>      deleteClassButton;
    
    ScopedPointer<ImageButton>      saveTrainingButton;
    ScopedPointer<ImageButton>      loadTrainingButton;
    
    ScopedPointer<Label>            addClassLabel;
    ScopedPointer<Label>            deleteClassLabel;
    
    ScopedPointer<Label>            saveTrainingLabel;
    ScopedPointer<Label>            loadTrainingLabel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TrainContentComponent)
};


#endif  // TRAINCONTENTCOMPONENT_H_INCLUDED
