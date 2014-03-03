/*
  ==============================================================================

    AudioStream.cpp
    Created: 16 Nov 2013 11:22:57am
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#include "AudioStream.h"

AudioStream::AudioStream() : mAudioStreamThread("Audio IO")
{
    mAudioStreamThread.startThread(miAudioThreadPriority);

    
    sharedAudioDeviceManager->getAudioDeviceSetup(deviceSetup);
    
    
    onsetClassifier = new OnsetClassification(deviceSetup.bufferSize,
                                              deviceSetup.inputChannels.toInteger(),
                                              deviceSetup.sampleRate);
    
    
    currentMode = BeatSurfaceBase::IdleMode;
    
    m_iCurrentClassIndexToTrain = 0;
}



AudioStream::~AudioStream()
{
    onsetClassifier     = nullptr;
    
    mAudioStreamThread.stopThread(miStopThreadTimeOut_ms);
}



void AudioStream::audioDeviceAboutToStart(AudioIODevice* device)
{
    
}



void AudioStream::audioDeviceStopped()
{
    
}




void AudioStream::audioDeviceIOCallback( const float** inputChannelData,
                                        int totalNumInputChannels,
                                        float** outputChannelData,
                                        int totalNumOutputChannels,
                                        int blockSize)
{
    //OnsetClassification::detectOnset(inputChannelData);
    
    
    if (currentMode == BeatSurfaceBase::TrainMode)
    {
        if(onsetClassifier->detectOnsets(inputChannelData))
        {
            onsetClassifier->train(m_iCurrentClassIndexToTrain);
            guiUpdater->displayTrainingOnset(m_iCurrentClassIndexToTrain);
        }
        
    }
    
    
    if (currentMode == BeatSurfaceBase::PlayMode)
    {
        
        if(onsetClassifier->detectOnsets(inputChannelData))
        {
            onsetClassifier->classify();
            guiUpdater->displayTrainingOnset(m_iCurrentClassIndexToTrain);
        }
    }
    
    
    
    for (int sample = 0; sample < blockSize; sample++)
    {
        
        for (int channel = 0; channel < totalNumOutputChannels; channel++)
        {
            outputChannelData[channel][sample] = inputChannelData[channel][sample];
        }
        
    }
    
    
}



void AudioStream::setMode(BeatSurfaceBase::SystemMode newMode)
{
    currentMode = newMode;
}


void AudioStream::setClassIndexToTrain(int newClassIndex)
{
    m_iCurrentClassIndexToTrain = newClassIndex;
}


void AudioStream::timerCallback()
{
    
}
