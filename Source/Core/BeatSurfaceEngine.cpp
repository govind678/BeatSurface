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
    
    sharedAudioDeviceManager->getAudioDeviceSetup(deviceSetup);
    
    mfSamplingRate          = deviceSetup.sampleRate;
    miNumInputChannels      = deviceSetup.inputChannels.toInteger();
    miNumOutputChannels     = deviceSetup.outputChannels.toInteger();
    miBlockSize             = deviceSetup.bufferSize;
    
    liveAudioStream         = new AudioStream();
    
    m_iTrainingTime         = 5000;    // 10 seconds
    m_bTrainingState        = false;
}


BeatSurfaceEngine::~BeatSurfaceEngine()
{
    sharedAudioDeviceManager->removeAudioCallback(liveAudioStream);
    
//    onsetClassifier = nullptr;
    liveAudioStream = nullptr;
}


void BeatSurfaceEngine::liveAudioStreamButtonClicked(bool toggleState)
{
    if (toggleState) {
        liveAudioStream->setMode(BeatSurfaceBase::PlayMode);
        m_bTrainingState    =   false;
        sharedAudioDeviceManager->addAudioCallback(liveAudioStream);
    }
    
    else {
        liveAudioStream->setMode(BeatSurfaceBase::IdleMode);
        sharedAudioDeviceManager->removeAudioCallback(liveAudioStream);
    }
}


void BeatSurfaceEngine::trainClassButtonClicked(int classIndex)
{
    liveAudioStream->setMode(BeatSurfaceBase::TrainMode);
    liveAudioStream->setClassIndexToTrain(classIndex);
    sharedAudioDeviceManager->addAudioCallback(liveAudioStream);
    m_bTrainingState    =   true;
    startTimer(m_iTrainingTime);
}


void BeatSurfaceEngine::audioDeviceSettingsChanged()
{
    sharedAudioDeviceManager->getAudioDeviceSetup(deviceSetup);
    
    mfSamplingRate          = deviceSetup.sampleRate;
    miNumInputChannels      = deviceSetup.inputChannels.toInteger();
    miNumOutputChannels     = deviceSetup.outputChannels.toInteger();
    miBlockSize             = deviceSetup.bufferSize;
}



void BeatSurfaceEngine::parametersChanged(BeatSurfaceBase::ParameterID parameterID, float parameterValue)
{
    switch (parameterID)
    {
            
        case BeatSurfaceBase::VelocitySensitivity :
            liveAudioStream->onsetClassifier->setVelocitySensitivity(parameterValue);
            break;
            
        case BeatSurfaceBase::DecayTimeSensitivity :
            liveAudioStream->onsetClassifier->setDecayTimeSensitivity(parameterValue);
            break;
            
        default:
            break;
    }
}



void BeatSurfaceEngine::timerCallback()
{
    if (m_bTrainingState) {
        sharedAudioDeviceManager->removeAudioCallback(liveAudioStream);
        guiUpdater->doneTraining();
        stopTimer();
        m_bTrainingState = false;
        
        liveAudioStream->onsetClassifier->userFinishedTraining();
    }
}


void BeatSurfaceEngine::addClass()
{
    liveAudioStream->onsetClassifier->addClass();
    
}


void BeatSurfaceEngine::deleteClass(int classIndex)
{
    liveAudioStream->onsetClassifier->deleteClass(classIndex);
}


void BeatSurfaceEngine::saveTraining(juce::String filePath)
{
    liveAudioStream->onsetClassifier->saveTraining(filePath.toStdString());
}

void BeatSurfaceEngine::loadTraining(juce::String filePath)
{
    liveAudioStream->onsetClassifier->loadTraining(filePath.toStdString());
}