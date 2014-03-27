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


//==============================================================================
/**
    Core class to create an instance of the detection and classification system.
 
    Depends on the JUCE library to manage audio devices and buffers. Also depends on the libSVM for classification.
 */


class BeatSurfaceEngine     :   public Timer,
                                public ActionListener
{
    
public:
    
    //==============================================================================
    /** Creates an instance of the Beat Surface Engine */
    BeatSurfaceEngine();
    ~BeatSurfaceEngine();
    //==============================================================================
    
    
    
    
    //==============================================================================
    // Interface Methods for GUI -> Beat Surface Core

    void addClass();
    void deleteClass(int classIndex);
    
    void playButtonClicked(bool toggleState);
    void recordButtonClicked(bool toggleState);
    
    void setSystemMode(BeatSurfaceBase::SystemMode newMode);
    void classButtonClicked(int classIndex);
    
    void audioDeviceSettingsChanged();
    void setParameter(BeatSurfaceBase::ParameterID parameterID, double value);
    
    void saveTraining(File trainingFile);
    void loadTraining(File trainingFile);
    
    void setTrainingTimeinBars(int bars);
    
    
    
    
    
    //==============================================================================
    // Interface Get Methods for Beat Surface Core -> GUI
    
    BeatSurfaceBase::SystemMode getSystemMode();
    int getNumClasses();
    int getCurrentTrainingClass();
    
    double getParameter(BeatSurfaceBase::ParameterID parameterID);
    
    double getClassificationConfidence();
    int    getCurrentClassificationResult();
    int    getCurrentObservation();
    String getCurrentFeatureVector();
    
    
private:
    
    //==============================================================================
    // Internal Methods
    void actionListenerCallback (const String &message);
    void timerCallback();
    void performTraining();

    
    ScopedPointer<AudioStream>              m_pcLiveAudioStream;
    
    
    int                                     m_iTrainingTimeinBars;
    int                                     m_fTrainingTimeinMS;
    bool                                    m_bIsCurrentlyInTraining;
    
    BeatSurfaceBase::SystemMode             m_eCurrentMode;
    

    
};


#endif  // BEATSURFACEENGINE_H_INCLUDED
