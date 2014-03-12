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


class BeatSurfaceEngine     :   public Timer,
                                public ActionListener
{
    
public:
    
    //==============================================================================
    // Constructor and Destructor
    BeatSurfaceEngine();
    ~BeatSurfaceEngine();
    //==============================================================================
    
    
    
    
    //==============================================================================
    // Interface Methods for GUI -> Beat Surface Core
    void liveAudioStreamButtonClicked(bool toggleState);
    void setMode(BeatSurfaceBase::SystemMode newMode);
    
    void trainClassButtonClicked(int classIndex);
    void idleModeClassButtonClicked(int classIndex);
    void audioDeviceSettingsChanged();
    void parametersChanged(BeatSurfaceBase::ParameterID parameterID, float parameterValue);
    
    void addClass();
    void deleteClass(int classIndex);
    
    void saveTraining(String filePath);
    void loadTraining(String filePath);
    
    void setTrainingTimeinBars(int bars);
    
    BeatSurfaceBase::SystemMode getSystemMode();
    //==============================================================================
    
    
    
private:
    
    //==============================================================================
    // Action Listener Callback
    void actionListenerCallback (const String &message);
    //==============================================================================

    
    ScopedPointer<AudioStream>              m_pcLiveAudioStream;

    
    int m_iTrainingTimeinBars;
    int m_fTrainingTimeinMS;
    bool m_bTrainingState;
    
    BeatSurfaceBase::SystemMode m_eCurrentMode;
    
    //==============================================================================
    // Timer Callback
    void timerCallback();
    //==============================================================================

    
};


#endif  // BEATSURFACEENGINE_H_INCLUDED
