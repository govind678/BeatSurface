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
#include "MidiOut.h"
//#include "AudioPlayer.h"
#include "AudioFileIO.h"


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
    
    void setClassIndexToTrain(int newClassIndex);
    void audioDeviceSettingsChanged();
    
    void playTrainingDataAtClass(int classIndex);
    
    void loadAudioFileToTrain(String audioFilePath);
    
    ScopedPointer<OnsetClassification> m_pcOnsetClassifier;
    
    
private:
    
    void stopCurrentClassPlayback();
    
    AudioDeviceManager::AudioDeviceSetup        deviceSetup;
    OnsetClassification::AudioDeviceSettings    deviceSettings;
    
    ScopedPointer<AudioFileIO>                  m_pcAudioFileIO;
    
    
    TimeSliceThread                             m_AudioStreamThread;
    
    AudioSourcePlayer                           audioSourcePlayer;
    
    ScopedPointer<MidiOut>                      m_pcMidiOut;
    
//    OwnedArray<AudioPlayer> m_acAudioPlayer;
    
    static const int m_iStopThreadTimeOut_ms = 20;
    static const int m_iAudioThreadPriority  = 8;
    
    void timerCallback();
    
    int m_iCurrentClassIndexToTrain;
    int m_iCurrentClassificationResult;
    float m_fCurrentRMS;
    
    Array<float>                                m_pfOnsetProbabilities;
    
    OwnedArray<AudioSampleBuffer>               m_ppfOnsetAudio;

    Array<int>                                  m_piClassLabels;
    
    int                                         m_iCurrentIndexToPlay;
};




#endif  // AUDIOSTREAM_H_INCLUDED
