/*
  ==============================================================================

    BeatSurfaceEngine.h
    Created: 12 Feb 2014 10:46:09pm
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#ifndef BEATSURFACEENGINE_H_INCLUDED
#define BEATSURFACEENGINE_H_INCLUDED

#include "BeatSurfaceHeader.h"

#include "AudioStream.h"
#include "AudioFileIO.h"
#include "OnsetClassification.h"



class BeatSurfaceEngine
{
    
public:
    
    BeatSurfaceEngine();
    ~BeatSurfaceEngine();
    
    void liveAudioStreamButtonClicked(bool toggleState);
    void audioDeviceSettingsChanged();
    void parametersChanged(int parameterID, float parameterValue);
    
    
private:
    
    AudioDeviceManager::AudioDeviceSetup    deviceSetup;
    
    ScopedPointer<AudioStream>              liveAudioStream;
    ScopedPointer<OnsetClassification>      onsetClassifier;
    
    
    float   mfSamplingRate;
    int     miNumInputChannels;
    int     miNumOutputChannels;
    int     miBlockSize;
    
    
};


#endif  // BEATSURFACEENGINE_H_INCLUDED
