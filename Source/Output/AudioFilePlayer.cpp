/*
  ==============================================================================

    AudioFilePlayer.cpp
    Created: 18 Dec 2013 12:15:09pm
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#include "AudioFilePlayer.h"
#include <iostream>
#include <stdio.h>


AudioFilePlayer::AudioFilePlayer()  :   thread("Audio File Playback")
{
    transportSource.setSource(nullptr);
    
    formatManager.registerBasicFormats();
    
    m_bShouldToggle = false;
    m_bIsCurrentlyPlaying = false;
    
    thread.startThread(3);

}


AudioFilePlayer::~AudioFilePlayer()
{
    transportSource.stop();
    transportSource.setSource(nullptr);
    currentAudioFileSource = nullptr;
    thread.stopThread(20);
}


bool AudioFilePlayer::loadFileIntoTransport(const File& audioFile)
{
    
    transportSource.stop();
    transportSource.setSource(nullptr);
    
    currentAudioFileSource = nullptr;
    
    std::cout << audioFile.getFullPathName() << std::endl;
    AudioFormatReader* reader = formatManager.createReaderFor(audioFile);
    
    if (reader != nullptr)
    {
        currentAudioFileSource = new AudioFormatReaderSource(reader, true);
        currentAudioFileSource->setLooping(false);
        transportSource.setSource (currentAudioFileSource, 32768, &thread, reader->sampleRate);
        
        return true;
    }
    
    else
    {
        return false;
    }
}


void AudioFilePlayer::play(int velocity)
{
    if (m_bShouldToggle)
    {
        if (m_bIsCurrentlyPlaying)
        {
            transportSource.setGain(0.0f);
            m_bIsCurrentlyPlaying = false;
        }
        
        else
        {
            transportSource.setGain(float(velocity) / 127.0F);
            transportSource.setPosition(0.0F);
            transportSource.start();
            m_bIsCurrentlyPlaying = true;
        }
    }
    
    
    
    else
    {
        transportSource.setGain(float(velocity) / 127.0F);
        transportSource.setPosition(0.0F);
        transportSource.start();
        m_bIsCurrentlyPlaying = true;
    }
    
}

void AudioFilePlayer::stop()
{
    transportSource.stop();
}

void AudioFilePlayer::setLooping(bool looping)
{
    currentAudioFileSource->setLooping(looping);
}

void AudioFilePlayer::setToggle(bool toggle)
{
    m_bShouldToggle = toggle;
}


//==============================================================================
// Process Functions - Audio Source Callbacks
// !!! Audio Thread
//==============================================================================

void AudioFilePlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}


void AudioFilePlayer::releaseResources()
{
//    transportSource.releaseResources();
}


void AudioFilePlayer::getNextAudioBlock(const AudioSourceChannelInfo &audioSourceChannelInfo)
{
    transportSource.getNextAudioBlock(audioSourceChannelInfo);
}