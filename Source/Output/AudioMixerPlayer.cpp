/*
  ==============================================================================

    AudioMixerPlayer.cpp
    Created: 30 Mar 2014 4:31:08pm
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#include "AudioMixerPlayer.h"


AudioMixerPlayer::AudioMixerPlayer()
{
    audioFileStream.clear(true);
    
    m_pbAudioFileInitialized.ensureStorageAllocated(16);
    
    for (int i=0; i < 16; i++)
    {
        m_pbAudioFileInitialized.add(false);
    }
    
    audioSourcePlayer.setSource(&audioMixer);
    
    sharedAudioDeviceManager->addAudioCallback(this);
}


AudioMixerPlayer::~AudioMixerPlayer()
{
    sharedAudioDeviceManager->removeAudioCallback(this);
    
    audioSourcePlayer.setSource(0); 
    m_pbAudioFileInitialized.clear();
    audioMixer.removeAllInputs();

    audioFileStream.clear(true);
    
}




void AudioMixerPlayer::loadAudioFile(int sampleID, File audioFile)
{    
    if (sampleID >= 0)
    {
        bool result = audioFileStream.getUnchecked(sampleID)->loadFileIntoTransport(audioFile);
        m_pbAudioFileInitialized.set(sampleID, result);
    }
}



void AudioMixerPlayer::startPlayback(int sampleID, int velocity)
{
    if (m_pbAudioFileInitialized.getUnchecked(sampleID))
    {
        audioFileStream.getUnchecked(sampleID)->play(velocity);
    }
}



void AudioMixerPlayer::stopPlayback(int sampleID)
{
    if (m_pbAudioFileInitialized.getUnchecked(sampleID))
    {
        audioFileStream.getUnchecked(sampleID)->stop();
    }
    
}


void AudioMixerPlayer::setLooping(int sampleID, bool looping)
{
    if (m_pbAudioFileInitialized.getUnchecked(sampleID))
    {
        audioFileStream.getUnchecked(sampleID)->setLooping(looping);
    }
}


void AudioMixerPlayer::setToggle(int sampleID, bool toggle)
{
    if (m_pbAudioFileInitialized.getUnchecked(sampleID))
    {
        audioFileStream.getUnchecked(sampleID)->setToggle(toggle);
    }
}


void AudioMixerPlayer::addClass()
{
    audioFileStream.add(new AudioFilePlayer());
    audioMixer.addInputSource(audioFileStream.getLast(), false);
}

void AudioMixerPlayer::deleteClass(int index)
{
    audioMixer.removeInputSource(audioFileStream.getUnchecked(index));
    audioFileStream.remove(index);
    m_pbAudioFileInitialized.set(index, false);
}



//==============================================================================
// Device Callback Methods
// !!! Audio Thread
//==============================================================================

void AudioMixerPlayer::audioDeviceIOCallback(const float** inputChannelData,
                                             int totalNumInputChannels,
                                             float** outputChannelData,
                                             int totalNumOutputChannels,
                                             int numSamples)

{
	audioSourcePlayer.audioDeviceIOCallback (inputChannelData,
                                             totalNumInputChannels,
                                             outputChannelData,
                                             totalNumOutputChannels,
                                             numSamples);
    
}


void AudioMixerPlayer::audioDeviceAboutToStart (AudioIODevice* device)
{
	audioSourcePlayer.audioDeviceAboutToStart (device);
}


void AudioMixerPlayer::audioDeviceStopped()
{
	audioSourcePlayer.audioDeviceStopped();
}

