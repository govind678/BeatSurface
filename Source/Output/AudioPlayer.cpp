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


AudioPlayer::AudioPlayer()  :   thread("Audio File Playback")
{
    transportSource.setSource(0);
    
    formatManager.registerBasicFormats();
    
    sourcePlayer.setSource(&transportSource);
    
    sharedAudioDeviceManager->addAudioCallback(&sourcePlayer);

}


AudioPlayer::~AudioPlayer()
{
    transportSource.stop();
    transportSource.setSource(nullptr);
    sourcePlayer.setSource(nullptr);
    sharedAudioDeviceManager->removeAudioCallback(&sourcePlayer);
    currentAudioFileSource = nullptr;
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
        transportSource.setSource (currentAudioFileSource, 32768, &thread, reader->sampleRate);
    }
    
    else
    {
        std::cout<<"No File!"<<std::endl;
    }
    
}


void AudioPlayer::play(int velocity)
{
    transportSource.setGain(float(velocity / 127.0f));
    transportSource.setPosition(0.0f);
    transportSource.start();
}