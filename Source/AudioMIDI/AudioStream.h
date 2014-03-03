/*
  ==============================================================================

    AudioStream.h
    Created: 16 Nov 2013 11:22:57am
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#ifndef AUDIOSTREAM_H_INCLUDED
#define AUDIOSTREAM_H_INCLUDED

#include "BeatSurfaceHeader.h"
#include "OnsetClassification.h"


class AudioStream : public AudioIODeviceCallback,
                    private Timer
{
    
public:
    
    AudioStream();
    ~AudioStream();
    
    void audioDeviceIOCallback(const float** inputChannelData,
							   int totalNumInputChannels,
							   float** outputChannelData,
							   int totalNumOutputChannels,
							   int blockSize);
	
	void audioDeviceAboutToStart (AudioIODevice* device);
    void audioDeviceStopped();
    
    void setMode(BeatSurfaceBase::SystemMode newMode);
    void setClassIndexToTrain(int newClassIndex);
    
    
    ScopedPointer<OnsetClassification> onsetClassifier;
    
    
private:
    
    AudioDeviceManager::AudioDeviceSetup    deviceSetup;
    
    TimeSliceThread mAudioStreamThread;
    
    static const int miStopThreadTimeOut_ms = 20;
    static const int miAudioThreadPriority  = 8;
    
    void timerCallback();
    
    BeatSurfaceBase::SystemMode currentMode;
    
    int m_iCurrentClassIndexToTrain;
    
};




#endif  // AUDIOSTREAM_H_INCLUDED
