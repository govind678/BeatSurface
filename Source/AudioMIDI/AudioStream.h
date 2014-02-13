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



class AudioStream : public AudioIODeviceCallback,
                    private Timer
{
    
public:
    
    AudioStream(AudioDeviceManager& deviceManager, AudioDeviceManager::AudioDeviceSetup deviceSetup);
    ~AudioStream();
    
    void audioDeviceIOCallback(const float** inputChannelData,
							   int totalNumInputChannels,
							   float** outputChannelData,
							   int totalNumOutputChannels,
							   int blockSize);
	
	void audioDeviceAboutToStart (AudioIODevice* device);
    void audioDeviceStopped();
    
    void setMode(int currentMode);
    
    
private:
    
    
    AudioDeviceManager& mDeviceManager;
    
    TimeSliceThread mAudioStreamThread;
    
    void timerCallback();
    
};




#endif  // AUDIOSTREAM_H_INCLUDED
