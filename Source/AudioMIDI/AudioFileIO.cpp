/*
  ==============================================================================

    AudioFileIO.cpp
    Created: 18 Nov 2013 2:56:30am
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#include "AudioFileIO.h"


AudioFileIO::AudioFileIO()  :   thread("Audio File IO")
{
    
    AudioFormatManager mcFormatManager;
    
    transportSource.setSource(nullptr);
    formatManager.registerBasicFormats();
    
    thread.startThread(3);
}


AudioFileIO::~AudioFileIO()
{

    transportSource.setSource(nullptr);
    currentAudioFileSource  =   nullptr;
    
    thread.stopThread(20);
    
    //delete onsetClassifier;
}




void AudioFileIO::loadAudioFile(String audioFilePath)
{
    m_sCurrentFilePath  =   audioFilePath;
    
    transportSource.stop();
    transportSource.setSource(nullptr);
    
    currentAudioFileSource  =   nullptr;
    
    AudioFormatReader* reader = formatManager.createReaderFor(File(m_sCurrentFilePath));
    
    
    if (reader != nullptr)
    {
        currentAudioFileSource = new AudioFormatReaderSource (reader, true);
        currentAudioFileSource->setLooping(false);
        transportSource.setSource(currentAudioFileSource, 32768, &thread, reader->sampleRate);
    }
}



//==============================================================================
// Process Functions - Audio Source Callbacks
// !!! Audio Thread
//==============================================================================

void AudioFileIO::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}


void AudioFileIO::releaseResources()
{
    stopPlayback();
    transportSource.releaseResources();
}


void AudioFileIO::getNextAudioBlock(const AudioSourceChannelInfo &audioSourceChannelInfo)
{
    transportSource.getNextAudioBlock(audioSourceChannelInfo);
    processAudioBlock(audioSourceChannelInfo.buffer->getArrayOfChannels(), audioSourceChannelInfo.numSamples);
}



void AudioFileIO::processAudioBlock(float **audioBuffer, int numSamples)
{
    
}



//==============================================================================
// Transport Control Methods
//==============================================================================

void AudioFileIO::startPlayback()
{
    transportSource.setPosition(0);
    transportSource.start();
}

void AudioFileIO::stopPlayback()
{
    transportSource.stop();
}