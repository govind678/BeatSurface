/*
  ==============================================================================

    BeatSurfaceEngine.h
    Created: 12 Feb 2014 10:46:09pm
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#ifndef BEATSURFACEENGINE_H_INCLUDED
#define BEATSURFACEENGINE_H_INCLUDED


#include "AudioStream.h"
//#include "AudioFileIO.h"
#include "BeatSurfaceHeader.h"


class BeatSurfaceEngine     :   public Timer
{
    
public:
    
    BeatSurfaceEngine();
    ~BeatSurfaceEngine();
    
    void liveAudioStreamButtonClicked(bool toggleState);
    void trainClassButtonClicked(int classIndex);
    void audioDeviceSettingsChanged();
    void parametersChanged(BeatSurfaceBase::ParameterID parameterID, float parameterValue);
    
    void addClass();
    void deleteClass(int classIndex);
    
    void saveTraining(String filePath);
    void loadTraining(String filePath);
    
    
private:
    
    AudioDeviceManager::AudioDeviceSetup    deviceSetup;
    
    ScopedPointer<AudioStream>              liveAudioStream;

    
    
    float   mfSamplingRate;
    int     miNumInputChannels;
    int     miNumOutputChannels;
    int     miBlockSize;
    
    int m_iTrainingTime;
    bool m_bTrainingState;
    
    
    void timerCallback();
    
    
};


#endif  // BEATSURFACEENGINE_H_INCLUDED
