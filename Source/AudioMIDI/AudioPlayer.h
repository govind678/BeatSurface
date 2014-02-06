/*
  ==============================================================================

    AudioPlayer.h
    Created: 18 Dec 2013 12:15:09pm
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#ifndef AUDIOPLAYER_H_INCLUDED
#define AUDIOPLAYER_H_INCLUDED

#include "JuceHeader.h"


class AudioPlayer   : public Timer
{
    
public:
    
    AudioPlayer(AudioDeviceManager& deviceManager, AudioDeviceManager::AudioDeviceSetup deviceSetup);
    ~AudioPlayer();
    
    void loadFileIntoTransport(const File& audioFile);
    
    AudioTransportSource transportSource;
    
    
    
private:
    
    AudioFormatManager formatManager;
    AudioDeviceManager &deviceManager;
    
    AudioSourcePlayer sourcePlayer;
    ScopedPointer<AudioFormatReaderSource> currentAudioFileSource;
    
    
    TimeSliceThread audioPlayerThread;
    
    void timerCallback();
    
};



#endif  // AUDIOPLAYER_H_INCLUDED
