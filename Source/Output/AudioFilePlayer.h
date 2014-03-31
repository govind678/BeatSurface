/*
  ==============================================================================

    AudioFilePlayer.h
    Created: 18 Dec 2013 12:15:09pm
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#ifndef AUDIOFILEPLAYER_H_INCLUDED
#define AUDIOFILEPLAYER_H_INCLUDED

#include "BeatSurfaceHeader.h"


class AudioFilePlayer   :   public AudioSource
{
    
public:
    
    AudioFilePlayer();
    ~AudioFilePlayer();
    
    
    //==============================================================================
    // Transport Methods
    
    bool loadFileIntoTransport(const File& audioFile);
    void play(int velocity);
    void stop();
    void setLooping(bool looping);
    void setToggle(bool toggle);
    
    
    
    
    //==============================================================================
    // Audio Source Methods
    
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& audioSourceChannelInfo) override;
    void releaseResources() override;
    
    
    
    
private:
    
    AudioFormatManager                          formatManager;
    AudioTransportSource                        transportSource;
    ScopedPointer<AudioFormatReaderSource>      currentAudioFileSource;
    
    bool                                        m_bShouldToggle;
    bool                                        m_bIsCurrentlyPlaying;
    
    TimeSliceThread                             thread;
};



#endif  // AudioFilePlayer_H_INCLUDED
