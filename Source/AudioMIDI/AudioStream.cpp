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
}



AudioStream::~AudioStream()
{
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
    
    for (int sample = 0; sample < blockSize; sample++)
    {
        
        for (int channel = 0; channel < totalNumOutputChannels; channel++)
        {
            outputChannelData[channel][sample] = inputChannelData[channel][sample];
        }
        
    }
    
    
}


void AudioStream::timerCallback()
{
    
}
