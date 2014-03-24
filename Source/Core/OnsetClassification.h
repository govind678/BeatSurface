/*
  ==============================================================================

    OnsetClassification.h
    Created: 16 Nov 2013 5:44:30pm
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#ifndef ONSETCLASSIFICATION_H_INCLUDED
#define ONSETCLASSIFICATION_H_INCLUDED


#include <algorithm>
#include <fstream>
#include <vector>
#include <stdio.h>


#include "ShortTermFourierTransform.h"
#include "AudioFeatureExtraction.h"
#include "SVMTrain.h"

//#include "KNNClassifier.h"


#define MAX_DECAY_WINDOW_BLOCKS     25          // For Block Size of 512 ~ 250 ms
#define MAX_DELTA_THRESHOLD         4.0F
#define MAX_BUFFER_SIZE             2048


//========================================================================================================================

using std::vector;

class OnsetClassification
{
   

public:
    
    
    //==============================================================================
    // Constructor and Destructor
    
    OnsetClassification();
    ~OnsetClassification();
    //==============================================================================
    
    
    
    
    //==============================================================================
    // Audio Device Settings Structure
    
    struct AudioDeviceSettings
    {
        int iBufferSize;
        int iNumChannels;
        float fSampleRate;
    };
    //==============================================================================
    
    
    
    
    //==============================================================================
    // Detection Parameters Structure
    
    struct DetectionParameters
    {
        //--- Sensitivity Settings ---//
        float fDeltaThreshold;
        int   iDecayBlockWindow;
    };
    //==============================================================================
    
    
    
    
    //==============================================================================
    // Classification Parameters Structure
    
    struct ClassificationParameters
    {
        int iNumClasses;
        int iNumFeatures;
        int iNumObservations;
        bool bDidFinishTraining;
    };
    //==============================================================================
    
    
    
    
    //==============================================================================
    // Process Methods
    
    bool detectOnsets(const float** inputBuffer);
    int classify();
    void train(int classLabel);
    float rootMeanSquare(const float** inputBuffer);
    //==============================================================================
    
    
    
    
    //==============================================================================
    // Set and Get Sensitivity Methods
    
    void setVelocitySensitivity (float sensitivity);
    void setDecayTimeSensitivity (float sensitivity);
    float getVelocitySensitivity();
    float getDecayTimeSensitivity();
    //==============================================================================
    
    
    
    
    //==============================================================================
    // Set Audio Device Settings
    
    void setAudioDeviceSettings(AudioDeviceSettings newSettings);
    //==============================================================================
    
    
    
    
    //==============================================================================
    // Set Training Params
    
    void addClass();
    void deleteClass(int classIndex);
    void setCurrentClassIndex(int classIndex);
    int  getNumClasses();
    void doneTraining();
    
    int saveTraining(std::string filePath);
    void loadTraining(std::string filePath);
    //==============================================================================
    
    
    vector<float> getCurrentSpectrum();
    vector<double> getCurrentProbabilityEstimates();
    
    float getCurrentSpectralCentroid();
    
    
private:
    
    //--- Low Pass Filter for Detection Function ---//
    float m_fNoveltyFunction;
    
    
    //--- Moving Average for Threshold ---//
    float m_fAdaptiveThreshold;
    bool m_bDecayPeriod;
    int  m_iDecayBlockCounter;
    
    
    float k_fMaxDeltaThreshold;
    int   k_iMaxDecayBlockWindow;
    
    
    //--- Input Amplitude ---//
    float m_fCurrentRootMeanSquare;
    
    
    //--- Current Features ---//
    float m_fCurrentSpectralCentroid;
    
    
    //--- Classes ---//
    ShortTermFourierTransform*      m_pcStft;
    AudioFeatureExtraction*         m_pcAudioFeature;
    SVMTrain*                       m_pcSvmTrainer;
    

    
    
    //--- Audio Parameters ---//
    int m_iBinSize;
    AudioDeviceSettings m_sDeviceSettings;
    
    
    //--- Detection Parameters ---//
    DetectionParameters m_sDetectionParameters;
    
    
    //--- Training Parameters ---//
    int m_iCurrentClassIndex;
    ClassificationParameters m_sTrainingParameters;
    
    
    
    //--- Spectrum Bandwidth ---//
    int m_iFrequencyBinLowLimit;
    int m_iFrequencyBinHighLimit;
    
    
    
    
    //--- Buffers ---//
    vector<float> m_pfCurrentRealFFT;
    vector<float> m_pfCurrentImgFFT;
    vector<float> m_pfPreviousRealFFT;
    
    
    //--- Classification ---//
    vector<vector<float>> m_ppfTrainingData;
    vector<int> m_piTrainingClassLabels;
    
    vector<float> m_pfTestingVector;
    vector<double> m_pdProbabilityEstimates;
    

};

//========================================================================================================================

#endif  // ONSETCLASSIFICATION_H_INCLUDED
