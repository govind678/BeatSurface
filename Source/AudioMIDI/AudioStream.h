/*
  ==============================================================================

    AudioStream.h
    Created: 16 Nov 2013 11:22:57am
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#ifndef AUDIOSTREAM_H_INCLUDED
#define AUDIOSTREAM_H_INCLUDED

#include "JuceHeader.h"
#include "OnsetClassification.h"



class AudioStream : public AudioIODeviceCallback,
                    private Timer
{
    
public:
    
    AudioStream(AudioDeviceManager& deviceManager, AudioDeviceManager::AudioDeviceSetup deviceSetup, int training);
    ~AudioStream();
    
    void audioDeviceIOCallback(const float** inputChannelData,
							   int totalNumInputChannels,
							   float** outputChannelData,
							   int totalNumOutputChannels,
							   int blockSize);
	
	void audioDeviceAboutToStart (AudioIODevice* device);
    void audioDeviceStopped();
    
    void setMode(int currentMode);
    
    OnsetClassification* onsetClassifier;
    
private:
    
    
    AudioDeviceManager& mDeviceManager;
    
    TimeSliceThread mAudioStreamThread;
    
    void timerCallback();
    
    int miPredictedClass;
    int miOnsetCounter;
    
    int miMode;
    
};




#endif  // AUDIOSTREAM_H_INCLUDED
