/*
  ==============================================================================

    BeatSurfaceEngine.cpp
    Created: 12 Feb 2014 10:46:09pm
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#include "BeatSurfaceEngine.h"
#include <stdio.h>
#include <iostream>


BeatSurfaceEngine::BeatSurfaceEngine()
{
    m_pcLiveAudioStream         = new AudioStream();
    
    m_iTrainingTimeinBars       = 4;
    m_fTrainingTimeinMS         = globalClock->getTimeInterval() * globalClock->getNumerator() * m_iTrainingTimeinBars;
    m_bIsCurrentlyInTraining    = false;
    
    m_eCurrentMode              = BeatSurfaceBase::IdleMode;
}


BeatSurfaceEngine::~BeatSurfaceEngine()
{
    sharedAudioDeviceManager->removeAudioCallback(m_pcLiveAudioStream);
    m_pcLiveAudioStream = nullptr;
}


void BeatSurfaceEngine::addClass()
{
    m_pcLiveAudioStream->addClass();
}


void BeatSurfaceEngine::deleteClass(int classIndex)
{
    m_pcLiveAudioStream->deleteClass(classIndex);
}


void BeatSurfaceEngine::setSystemMode(BeatSurfaceBase::SystemMode newMode)
{
    m_eCurrentMode = newMode;
    m_pcLiveAudioStream->setSystemMode(newMode);
}

BeatSurfaceBase::SystemMode BeatSurfaceEngine::getSystemMode()
{
    return m_eCurrentMode;
}


void BeatSurfaceEngine::classButtonClicked(int classIndex)
{
    if (m_eCurrentMode == BeatSurfaceBase::TrainMode)
    {
        m_pcLiveAudioStream->setCurrentClassToTrain(classIndex);
        sharedAudioDeviceManager->addAudioCallback(m_pcLiveAudioStream);
        
        if (m_bIsCurrentlyInTraining)
        {
            m_bIsCurrentlyInTraining    = false;
            globalClock->stopClock();
            stopTimer();
        }
        
        else
        {
            m_bIsCurrentlyInTraining    =   true;
            globalClock->startClock();
            startTimer(m_fTrainingTimeinMS);
        }
    }
}


void BeatSurfaceEngine::playButtonClicked(bool toggleState)
{
    if (toggleState)
    {
        if (m_bIsCurrentlyInTraining)
        {
            performTraining();
        }
        
        m_eCurrentMode      = BeatSurfaceBase::PlayMode;
        m_pcLiveAudioStream->setSystemMode(BeatSurfaceBase::PlayMode);
        
        sharedAudioDeviceManager->addAudioCallback(m_pcLiveAudioStream);
    }
    
    else
    {
        m_eCurrentMode      = BeatSurfaceBase::IdleMode;
        sharedAudioDeviceManager->removeAudioCallback(m_pcLiveAudioStream);
        m_pcLiveAudioStream->setSystemMode(BeatSurfaceBase::IdleMode);
    }
}


void BeatSurfaceEngine::recordButtonClicked(bool toggleState)
{
    if (toggleState)
    {
        m_eCurrentMode = BeatSurfaceBase::TrainMode;
        m_pcLiveAudioStream->setSystemMode(m_eCurrentMode);
    }
    
    else
    {
        m_eCurrentMode = BeatSurfaceBase::IdleMode;
        m_pcLiveAudioStream->setSystemMode(m_eCurrentMode);
        
        if (m_bIsCurrentlyInTraining)
        {
            stopTimer();
            performTraining();
        }
    }
}


void BeatSurfaceEngine::audioDeviceSettingsChanged()
{
    m_pcLiveAudioStream->audioDeviceSettingsChanged();
}



void BeatSurfaceEngine::setParameter(BeatSurfaceBase::ParameterID parameterID, double parameterValue)
{
    m_pcLiveAudioStream->setParameter(parameterID, parameterValue);
}



void BeatSurfaceEngine::setMidiOutput(int classIndex, bool include, int channelNo, int noteNum, int duration_ms)
{
    m_pcLiveAudioStream->setMIDIOutput(classIndex, include, channelNo, noteNum, duration_ms);
}


void BeatSurfaceEngine::setAudioOutputFile(int index, File audioFile)
{
    m_pcLiveAudioStream->setAudioOutputFile(index, audioFile);
}


void BeatSurfaceEngine::setAudioFileParam(int index, bool include, bool looping, bool toggle)
{
    m_pcLiveAudioStream->setAudioOutputLooping(index, looping);
    m_pcLiveAudioStream->setAudioOutputToggle(index, toggle);
    m_pcLiveAudioStream->setAudioOutputInclude(index, include);
}



void BeatSurfaceEngine::timerCallback()
{
    performTraining();
}


void BeatSurfaceEngine::performTraining()
{
    if (m_bIsCurrentlyInTraining)
    {
        stopTimer();
        
        sharedAudioDeviceManager->removeAudioCallback(m_pcLiveAudioStream);
        guiUpdater->doneTraining();
        globalClock->stopClock();
        m_bIsCurrentlyInTraining = false;
        
        m_pcLiveAudioStream->performTraining();
    }
}



void BeatSurfaceEngine::saveTraining(File trainingFile)
{
    m_pcLiveAudioStream->saveTraining(trainingFile);
}


void BeatSurfaceEngine::loadTraining(File trainingFile)
{
    String fileExtension = trainingFile.getFileExtension();
    
    if (fileExtension == ".wav"     ||
        fileExtension == ".mp3"     ||
        fileExtension == ".flac"    ||
        fileExtension == ".aac"     ||
        fileExtension == ".aiff"    ||
        fileExtension == ".aif")
    {
        m_eCurrentMode = BeatSurfaceBase::FileTrainMode;
        m_pcLiveAudioStream->setSystemMode(m_eCurrentMode);
        
//        m_pcLiveAudioStream->loadAudioFileToTrain(trainingFile);
//        sharedAudioDeviceManager->addAudioCallback(m_pcLiveAudioStream);
//        m_pcLiveAudioStream->startPlayback();
    }
    
    else if (fileExtension == ".txt")
    {
        m_pcLiveAudioStream->loadTraining(trainingFile);
    }
    
}



void BeatSurfaceEngine::setTrainingTimeinBars(int bars)
{
    m_iTrainingTimeinBars       =   bars;
    m_fTrainingTimeinMS         =   globalClock->getTimeInterval() * globalClock->getNumerator() * m_iTrainingTimeinBars;
}


void BeatSurfaceEngine::actionListenerCallback(const juce::String &message)
{
    if (message == "UPDATE_TRANSPORT")
    {
        m_fTrainingTimeinMS         =   globalClock->getTimeInterval() * globalClock->getNumerator() * m_iTrainingTimeinBars;
    }
}



void BeatSurfaceEngine::updateDataset(Array<bool> includes, Array<int> classes)
{
    bool type;
    
    if (m_eCurrentMode == BeatSurfaceBase::TrainMode)
    {
        type = false;
    }
    
    else
    {
        type = true;
    }
    
    m_pcLiveAudioStream->updateDataset(includes, classes, type);
}


double BeatSurfaceEngine::getParameter(BeatSurfaceBase::ParameterID parameterID)
{
    return m_pcLiveAudioStream->getParameter(parameterID);
}


int BeatSurfaceEngine::getNumClasses()
{
    return m_pcLiveAudioStream->getNumClasses();
}

int BeatSurfaceEngine::getCurrentTrainingClass()
{
    return m_pcLiveAudioStream->getCurrentTrainingClass();
}

double BeatSurfaceEngine::getClassificationConfidence()
{
    return m_pcLiveAudioStream->getClassificationConfidence();
}

int BeatSurfaceEngine::getCurrentObservation()
{
    return m_pcLiveAudioStream->getCurrentObservation();
}

String BeatSurfaceEngine::getCurrentFeatureVector()
{
    return m_pcLiveAudioStream->getCurrentFeatureVector();
}

int BeatSurfaceEngine::getCurrentClassificationResult()
{
    return m_pcLiveAudioStream->getCurrentClassificationResult();
}