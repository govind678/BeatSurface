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
    m_bTrainingState            = false;
}


BeatSurfaceEngine::~BeatSurfaceEngine()
{
    sharedAudioDeviceManager->removeAudioCallback(m_pcLiveAudioStream);
    
    m_pcLiveAudioStream = nullptr;
}




void BeatSurfaceEngine::setMode(BeatSurfaceBase::SystemMode newMode)
{
    m_eCurrentMode  =   newMode;
    m_pcLiveAudioStream->setMode(newMode);
    
    if (m_eCurrentMode == BeatSurfaceBase::PlayMode)
    {
        
    }
    
}



void BeatSurfaceEngine::liveAudioStreamButtonClicked(bool toggleState)
{
    if (toggleState)
    {
        m_pcLiveAudioStream->setMode(BeatSurfaceBase::PlayMode);
        m_bTrainingState    =   false;
        sharedAudioDeviceManager->addAudioCallback(m_pcLiveAudioStream);
    }
    
    else
    {
        m_pcLiveAudioStream->setMode(BeatSurfaceBase::IdleMode);
        sharedAudioDeviceManager->removeAudioCallback(m_pcLiveAudioStream);
    }
}


void BeatSurfaceEngine::trainClassButtonClicked(int classIndex)
{
    if (m_eCurrentMode == BeatSurfaceBase::TrainMode)
    {
        m_pcLiveAudioStream->setClassIndexToTrain(classIndex);
        sharedAudioDeviceManager->addAudioCallback(m_pcLiveAudioStream);
        m_bTrainingState    =   true;
        globalClock->startClock();
        startTimer(m_fTrainingTimeinMS);
    }
    
    else if (m_eCurrentMode == BeatSurfaceBase::PlayMode)
    {
       
    }
}

void BeatSurfaceEngine::idleModeClassButtonClicked(int classIndex)
{
    m_pcLiveAudioStream->setMode(BeatSurfaceBase::IdleMode);
    m_pcLiveAudioStream->playTrainingDataAtClass(classIndex);
}

void BeatSurfaceEngine::audioDeviceSettingsChanged()
{
    m_pcLiveAudioStream->audioDeviceSettingsChanged();
}



void BeatSurfaceEngine::parametersChanged(BeatSurfaceBase::ParameterID parameterID, float parameterValue)
{
    switch (parameterID)
    {
        case BeatSurfaceBase::VelocitySensitivity :
            m_pcLiveAudioStream->m_pcOnsetClassifier->setVelocitySensitivity(parameterValue);
            break;
            
        case BeatSurfaceBase::DecayTimeSensitivity :
            m_pcLiveAudioStream->m_pcOnsetClassifier->setDecayTimeSensitivity(parameterValue);
            break;
            
        default:
            break;
    }
}


BeatSurfaceBase::SystemMode BeatSurfaceEngine::getSystemMode()
{
    return m_pcLiveAudioStream->getMode();
}



void BeatSurfaceEngine::timerCallback()
{
    if (m_bTrainingState)
    {
        sharedAudioDeviceManager->removeAudioCallback(m_pcLiveAudioStream);
        guiUpdater->doneTraining();
        globalClock->stopClock();
        stopTimer();
        m_bTrainingState = false;
        
        m_pcLiveAudioStream->m_pcOnsetClassifier->doneTraining();
    }
}


void BeatSurfaceEngine::addClass()
{
    m_pcLiveAudioStream->m_pcOnsetClassifier->addClass();
}


void BeatSurfaceEngine::deleteClass(int classIndex)
{
    m_pcLiveAudioStream->m_pcOnsetClassifier->deleteClass(classIndex);
}


void BeatSurfaceEngine::saveTraining(juce::String filePath)
{
    m_pcLiveAudioStream->m_pcOnsetClassifier->saveTraining(filePath.toStdString());
}


void BeatSurfaceEngine::loadTraining(juce::String filePath)
{
    m_pcLiveAudioStream->m_pcOnsetClassifier->loadTraining(filePath.toStdString());
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