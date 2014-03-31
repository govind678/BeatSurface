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

#include "LiveScrollDisplay.h"



class ToolBarControls       :   public Component
{
    
public:
    
    ToolBarControls();
    ~ToolBarControls();
    
    
    void paint (Graphics& g);
    void resized();
    
    
    ScopedPointer<ImageButton>                  audioStreamToggleButton;
    ScopedPointer<ImageButton>                  recordToggleButton;
    
    ScopedPointer<ImageButton>                  addClassButton;
    ScopedPointer<ImageButton>                  deleteClassButton;
    
    ScopedPointer<ImageButton>                  saveTrainingButton;
    ScopedPointer<ImageButton>                  loadTrainingButton;
    
private:
    
    ScopedPointer<Label>                        trainClassLabel;
    ScopedPointer<Label>                        playLabel;
    
    ScopedPointer<Label>                        addClassLabel;
    ScopedPointer<Label>                        deleteClassLabel;
    
    ScopedPointer<Label>                        saveTrainingLabel;
    ScopedPointer<Label>                        loadTrainingLabel;
};




//========================================================================================================================



class PlayContentComponent  :   public Component
{

friend class MainComponent;

public:
    
    PlayContentComponent ();
    ~PlayContentComponent();


    void paint (Graphics& g);
    void resized();
    
    
    
private:
    
    ScopedPointer<ClassButtonArray>             shapeButtonArray;
    ScopedPointer<MetronomeComponent>           metroDisplay;
    
    ScopedPointer<LiveScrollDisplay>            waveformLiveScroller;
    ScopedPointer<LiveScrollDisplay>            spectrumLiveScroller;
    
    ScopedPointer<ToolBarControls>              toolBarControls;
    
    ScopedPointer<Label>            trainingTimeinBarsLabel;
    ScopedPointer<Slider>           trainingTimeinBarsSlider;
    
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlayContentComponent)
};


#endif  // PLAYCONTENTCOMPONENT_H_INCLUDED
