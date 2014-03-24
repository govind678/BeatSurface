/*
  ==============================================================================

    AudioFileIO.h
    Created: 18 Nov 2013 2:56:30am
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#ifndef AUDIOFILEIO_H_INCLUDED
#define AUDIOFILEIO_H_INCLUDED

#include "BeatSurfaceHeader.h"
//#include "OnsetClassification.h"


class AudioFileIO   :   public AudioSource
{
    
public:
    
    AudioFileIO ();
    ~AudioFileIO();
    
    void runDetectionAndClassification();
    
    void loadAudioFile(String audioFilePath);
    
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& audioSourceChannelInfo) override;
    void releaseResources() override;
    
    void processAudioBlock(float** audioBuffer, int numSamples);
    
    
    void startPlayback();
    void stopPlayback();
    
    
    //OnsetClassification* onsetClassifier;
    
    
private:
    
    
    AudioFormatManager      formatManager;
    ScopedPointer<AudioFormatReaderSource> currentAudioFileSource;
    AudioTransportSource    transportSource;
    
    int miBlockSize;
    float mfSampleRate;

    TimeSliceThread thread;
    String  m_sCurrentFilePath;

};



#endif  // AUDIOFILEIO_H_INCLUDED
