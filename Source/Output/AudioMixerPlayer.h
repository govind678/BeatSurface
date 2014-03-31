/*
  ==============================================================================

    AudioMixerPlayer.h
    Created: 30 Mar 2014 4:31:08pm
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#ifndef AUDIOMIXERPLAYER_H_INCLUDED
#define AUDIOMIXERPLAYER_H_INCLUDED

#include "BeatSurfaceHeader.h"
#include "AudioFilePlayer.h"

class AudioMixerPlayer      :   public AudioIODeviceCallback
{
    
public:
    
    AudioMixerPlayer();
    ~AudioMixerPlayer();
    
    
    void audioDeviceIOCallback(const float** inputChannelData,
							   int totalNumInputChannels,
							   float** outputChannelData,
							   int totalNumOutputChannels,
							   int blockSize) override;
	
	void audioDeviceAboutToStart (AudioIODevice* device) override;
    void audioDeviceStopped() override;
    
    
    
    void loadAudioFile(int sampleID, File audioFile);
    
    void startPlayback(int sampleID, int velocity);
    void stopPlayback(int sampleID);
    void setLooping(int sampleID, bool looping);
    void setToggle(int sampleID, bool toggle);
    
    void addClass();
    void deleteClass(int index);
    
private:
    
    MixerAudioSource                    audioMixer;
    OwnedArray<AudioFilePlayer>         audioFileStream;
    AudioSourcePlayer                   audioSourcePlayer;
    
    Array<bool>                         m_pbAudioFileInitialized;
};





#endif  // AUDIOMIXERPLAYER_H_INCLUDED
