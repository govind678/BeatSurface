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
    
    //liveAudioStream = new AudioStream();
}


BeatSurfaceEngine::~BeatSurfaceEngine()
{
    //deviceManager.removeAudioCallback(liveAudioStream);
    
    onsetClassifier = nullptr;
    //liveAudioStream = nullptr;
}


void BeatSurfaceEngine::liveAudioStreamButtonClicked(bool toggleState)
{
    if (toggleState) {
        sharedAudioDeviceManager->addAudioCallback(liveAudioStream);
    }
    
    else {
        sharedAudioDeviceManager->removeAudioCallback(liveAudioStream);
    }
    
}


void BeatSurfaceEngine::audioDeviceSettingsChanged()
{
    sharedAudioDeviceManager->getAudioDeviceSetup(deviceSetup);
    
    mfSamplingRate          = deviceSetup.sampleRate;
    miNumInputChannels      = deviceSetup.inputChannels.toInteger();
    miNumOutputChannels     = deviceSetup.outputChannels.toInteger();
    miBlockSize             = deviceSetup.bufferSize;
    
    std::cout << miBlockSize <<std::endl;
}


void BeatSurfaceEngine::parametersChanged(int parameterID, float parameterValue)
{
    switch (parameterID) {
            
        case 0:
            onsetClassifier->setVelocitySensitivity(parameterValue);
            break;
            
        case 1:
            onsetClassifier->setDecayTimeSensitivity(parameterValue);
            break;
            
        default:
            break;
    }
}


