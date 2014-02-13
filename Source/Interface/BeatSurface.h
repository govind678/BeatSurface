/*
  ==============================================================================

    BeatSurface.h
    Created: 12 Feb 2014 10:46:09pm
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#ifndef BEATSURFACE_H_INCLUDED
#define BEATSURFACE_H_INCLUDED

//#include "MainComponent.h"
#include "AudioStream.h"
#include "AudioFileIO.h"
#include "OnsetClassification.h"

class MainContentComponent;

class BeatSurface
{
    
public:
    
    BeatSurface(AudioDeviceManager& deviceManager, AudioDeviceManager::AudioDeviceSetup deviceSetup);
    ~BeatSurface();
    
private:
    
    OnsetClassification* onsetClassifier;
    
    
    
};


#endif  // BEATSURFACE_H_INCLUDED
