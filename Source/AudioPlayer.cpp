/*
  ==============================================================================

    AudioPlayer.cpp
    Created: 18 Dec 2013 12:15:09pm
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#include "AudioPlayer.h"
#include <iostream>
#include <stdio.h>


AudioPlayer::AudioPlayer(AudioDeviceManager& deviceManager, AudioDeviceManager::AudioDeviceSetup deviceSetup) : deviceManager(deviceManager),  audioPlayerThread("AudioPlayer Thread")
{
    transportSource.setSource(0);
    
    formatManager.registerBasicFormats();
    
    deviceManager.addAudioCallback(&sourcePlayer);
    sourcePlayer.setSource(&transportSource);
    
    audioPlayerThread.startThread(5);
}


AudioPlayer::~AudioPlayer()
{
    transportSource.setSource(nullptr);
    sourcePlayer.setSource(nullptr);
    deviceManager.removeAudioCallback(&sourcePlayer);
}


void AudioPlayer::timerCallback()
{
    
}


void AudioPlayer::loadFileIntoTransport(const File& audioFile)
{
    transportSource.stop();
    transportSource.setSource(nullptr);
    
    currentAudioFileSource = nullptr;
    
    AudioFormatReader* reader = formatManager.createReaderFor(audioFile);
    
    if (reader != nullptr)
    {
        currentAudioFileSource = new AudioFormatReaderSource(reader, true);
        transportSource.setSource(currentAudioFileSource, 8192, &audioPlayerThread, reader->sampleRate);
    }
    
    else
    {
        std::cout<<"No File!"<<std::endl;
    }
    
}