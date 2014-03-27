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


class AudioStream : public AudioIODeviceCallback,
                    public AudioSource,
                    private Timer
{
    
public:
    
    //==============================================================================
    // Constructor & Destructor
    AudioStream();
    ~AudioStream();
    
    
    
    //==============================================================================
    // Audio Device IO Callback Methods
    
    void audioDeviceAboutToStart (AudioIODevice* device);
    
    void audioDeviceIOCallback(const float** inputChannelData,
							   int totalNumInputChannels,
							   float** outputChannelData,
							   int totalNumOutputChannels,
							   int blockSize);
    void audioDeviceStopped();
    
    
    
    //==============================================================================
    // Audio Source Player Callback Methods
    
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& audioSourceChannelInfo) override;
    void releaseResources() override;
    
    
    
    //==============================================================================
    // Audio Stream Training Methods
    
    void loadAudioFileToTrain(File audioTrainingFile);
    void setCurrentClassToTrain(int newClass);
    void playTrainingDataAtClass(int classIndex);
    
    
    
    //==============================================================================
    // Transport Control Methods
    
    void startPlayback();
    void stopPlayback();
    bool isPlaying();
    
    
    
    //==============================================================================
    // Generic Audio Stream Methods
    
    void audioDeviceSettingsChanged();
    void setSystemMode(BeatSurfaceBase::SystemMode newMode);
    
    
    
    //==============================================================================
    // Onset Classification Methods
    
    void processAudioBlock(const float** audioBuffer, int numSamples);
    
    void setParameter(BeatSurfaceBase::ParameterID parameterID, double value);
    
    void addClass();
    void deleteClass(int index);
    void performTraining();
    
    void saveTraining(File trainingFile);
    void loadTraining(File trainingFile);
    
    
    
    //==============================================================================
    // Get Interface Methods
    
    double getParameter(BeatSurfaceBase::ParameterID parameterID);
    int    getNumClasses();
    int    getCurrentTrainingClass();
    int    getCurrentClassificationResult();
    double getClassificationConfidence();
    String getCurrentFeatureVector();
    int    getCurrentObservation();
    
private:
    
    //==============================================================================
    // Internal Timer Methods
    
    void timerCallback();
    
    
    
    //==============================================================================
    // Class Declarations
    
    ScopedPointer<OnsetClassification>          m_pcOnsetClassifier;
    ScopedPointer<MidiOut>                      m_pcMidiOut;
    
    
    
    //==============================================================================
    // Member Variables
    
    int                                         m_iCurrentClassIndexToTrain;
    int                                         m_iCurrentClassificationResult;
    float                                       m_fCurrentRMS;
    int                                         m_iCurrentIndexToPlay;
    bool                                        m_bOnsetDetected;
    
    AudioDeviceManager::AudioDeviceSetup        deviceSetup;
    TimeSliceThread                             m_AudioStreamThread;
    BeatSurfaceBase::SystemMode                 m_eCurrentMode;
    
    
    
    //==============================================================================
    // Audio File Reader and Playback
    
    AudioSourcePlayer                           audioSourcePlayer;
    AudioFormatManager                          formatManager;
    ScopedPointer<AudioFormatReaderSource>      currentAudioFileSource;
    AudioTransportSource                        transportSource;
    
    
    
    //==============================================================================
    // Onset Classification Buffers
    
    Array<float>                                m_pfOnsetProbabilities;
    Array<int>                                  m_piClassLabels;
    OwnedArray<AudioSampleBuffer>               m_ppfOnsetAudio;
    
    
    
    //==============================================================================
    // Thread Parameters
    
    static const int m_iStopThreadTimeOut_ms    = 20;
    static const int m_iAudioThreadPriority     = 8;

};




#endif  // AUDIOSTREAM_H_INCLUDED
