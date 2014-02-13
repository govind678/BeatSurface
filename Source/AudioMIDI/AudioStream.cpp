/*
  ==============================================================================

    AudioStream.cpp
    Created: 16 Nov 2013 11:22:57am
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#include "AudioStream.h"


AudioStream::AudioStream(AudioDeviceManager& deviceManager, AudioDeviceManager::AudioDeviceSetup deviceSetup) :
                            mDeviceManager(deviceManager),
                            mAudioStreamThread("Audio IO")
{
    deviceManager.addAudioCallback(this);
    mAudioStreamThread.startThread(8);       // Priority : 8/10
    
//    onsetClassifier = new OnsetClassification(deviceSetup.bufferSize, deviceSetup.inputChannels.toInteger(), deviceSetup.sampleRate);
//    
//    miOnsetCounter = 0;
}



AudioStream::~AudioStream()
{
    mDeviceManager.removeAudioCallback(this);
    mAudioStreamThread.stopThread(20);
    
//    delete onsetClassifier;
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
    
    //AudioSampleBuffer sampleBuffer = AudioSampleBuffer(outputChannelData, totalNumOutputChannels, blockSize);
    
    
    /*** Audio Processing ***/

//    switch (miMode) {
//        
//        //--- Run ---//
//        case 0:
//            
//            miPredictedClass = onsetClassifier->process(inputChannelData);
//            
//            if (miPredictedClass != 0) {
//                std::cout << miPredictedClass << std::endl;
//            }
//            
//            break;
//            
//        
//        
//        //--- Train ---//
//            
//        case 1:
//            
//            onsetClassifier->train(inputChannelData, miMode);
//    
//            break;
//            
//        
//        case 2:
//            
//            onsetClassifier->train(inputChannelData, miMode);
//            
//            break;
//            
//            
//        case 3:
//            
//            onsetClassifier->train(inputChannelData, miMode);
//            
//            break;
//            
//            
//        default:
//        
//            break;
//    }
    
//    if (onsetClassifier->process(inputChannelData) != 0) {
//        std::cout << "Bang" << std::endl;
//    }
    
    
    
    for (int sample = 0; sample < blockSize; sample++) {
        
        for (int channel = 0; channel < totalNumOutputChannels; channel++) {
            
            outputChannelData[channel][sample] = inputChannelData[channel][sample];
        
        }
        
    }
    
    
}


void AudioStream::timerCallback()
{
    
}
