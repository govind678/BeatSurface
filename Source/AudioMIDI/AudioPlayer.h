/*
  ==============================================================================

    AudioPlayer.h
    Created: 18 Dec 2013 12:15:09pm
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#ifndef AUDIOPLAYER_H_INCLUDED
#define AUDIOPLAYER_H_INCLUDED

#include "BeatSurfaceHeader.h"


class AudioPlayer
{
    
public:
    
    AudioPlayer();
    ~AudioPlayer();
    
    void loadFileIntoTransport(const File& audioFile);
    void play(int velocity);
    
    
private:
    
    AudioFormatManager formatManager;
    
    AudioSourcePlayer                           sourcePlayer;
    AudioTransportSource                        transportSource;
    ScopedPointer<AudioFormatReaderSource>      currentAudioFileSource;
    
    TimeSliceThread                             thread;
    
};



#endif  // AUDIOPLAYER_H_INCLUDED
