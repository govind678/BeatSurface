/*
  ==============================================================================

    OnsetClassification.h
    Created: 16 Nov 2013 5:44:30pm
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#ifndef ONSETCLASSIFICATION_H_INCLUDED
#define ONSETCLASSIFICATION_H_INCLUDED

#include "JuceHeader.h"

#include <vector>
#include <numeric>
#include <algorithm>
#include <stdio.h>
#include <iostream>


#include "ShortTermFourierTransform.h"
#include "AudioFeatureExtraction.h"
#include "SVMTrain.h"


#define MAX_DECAY_WINDOW_BLOCKS     25          // For Block Size of 512 ~ 250 ms
#define MIN_DECAY_WINDOW_BLOCKS     2
#define MAX_DELTA_THRESHOLD         4.0F
#define MIN_DELTA_THRESHOLD         0.6F
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
    // Audio Device Settings Structure
    
    struct AudioDeviceSettings
    {
        int         iBufferSize;
        int         iNumBins;
        int         iNumChannels;
        double      dSampleRate;
    };
    
    
    
    //==============================================================================
    // Detection Parameters Structure
    
    struct DetectionParameters
    {
        //--- Sensitivity Settings ---//
        double  dDeltaThreshold;
        int     iDecayBlockWindow;
    };
    
    
    
    //==============================================================================
    // Onset Detection Variables Structure
    
    struct DetectionVariables
    {
        //--- Moving Average for Threshold ---//
        double      dAdaptiveThreshold;
        bool        bDecayPeriod;
        int         iDecayBlockCounter;
    };
    
    
    
    //==============================================================================
    // Classification Parameters Structure
    
    struct ClassificationParameters
    {
        int iNumClasses;
        int iNumFeatures;
        int iNumObservations;
        double dCrossValidationAccuracy;
        bool bDidFinishTraining;
        int iCurrentObservation;
    };
    
    
    
    //==============================================================================
    // Audio Features Structure
    
    struct AudioFeatures
    {
        double   dSpectralCentroid;
        double   dSpectralFlatness;
        double   dSpectralSlope;
        
        double   dSpectralFlux;
        double   dRootMeanSquare;
    };
    
    
    
    //==============================================================================
    // Initialize - Set Buffer Sizes
    
    void initialize(int blockSize, double sampleRate);
    
    
    
    //==============================================================================
    // Process Methods
    
    bool detectOnset(const float** audioBuffer);
    bool detectOnset(float** audioBuffer);
    
    void trainAtClass(const float** audioBuffer, int classLabel);
    void trainAtClass(float** audioBuffer, int classLabel);
    
    int classify(const float** audioBuffer);
    int classify(float** audioBuffer);
    
    
    
    //==============================================================================
    // Set and Get Sensitivity Methods
    
    void setVelocitySensitivity (double sensitivity);
    void setDecayTimeSensitivity (double sensitivity);
    double getVelocitySensitivity();
    double getDecayTimeSensitivity();
    
    
    
    //==============================================================================
    // Training and Classification Parameter Methods
    
    void addClass();
    void deleteClass(int classIndex);
    void performTraining();
    
    int  getNumClasses();
    
    int saveTraining(File trainingFile);
    void loadTraining(File trainingFile);
    
    double getClassificationConfidence();
    int    getCurrentObservation();
    
    
    
    //==============================================================================
    // Are these needed?
    
    vector<float>   getCurrentSpectrum();
    vector<double>  getCurrentProbabilityEstimates();
    String          getCurrentFeatureVector();
    float           getCurrentSpectralCentroid();
    
    
    
private:
    
    
    //==============================================================================
    // Internal Utility Methods
    
    void normalizeVector(double* featureVector, double* normalizedVector);
    void findMaxAndMinVectors();
    int getTotalTrainedClasses();
    
    
    
    //==============================================================================
    // Internal Process Methods
    
    bool internal_detectOnset();
    int  internal_classify();
    void internal_trainAtClass(int classLabel);
    
    void evaluate();
    
    
    
    //==============================================================================
    // Class Declarations
    
    ScopedPointer<ShortTermFourierTransform>        m_pcDetectionSTFT;
    ScopedPointer<ShortTermFourierTransform>        m_pcClassificationSTFT;
    ScopedPointer<AudioFeatureExtraction>           m_pcAudioFeature;
    ScopedPointer<SVMTrain>                         m_pcSVMTrainer;
    
    
    
    //==============================================================================
    // Parameters
    
    AudioDeviceSettings         m_sDeviceSettings;
    DetectionParameters         m_sDetectionParameters;
    ClassificationParameters    m_sTrainingParameters;
    DetectionVariables          m_sDetectionVariables;
    AudioFeatures               m_sAudioFeatures;
    
    
    
    //==============================================================================
    // Buffers
    
    //--- Audio ---//
    vector<float>           m_pfDetectionAudioBlock;
    vector<float>           m_pfClassificationAudioBlock;
    
    //--- FFT ---//
    vector<float>           m_pfDetectionCurrentRealFFT;
    vector<float>           m_pfDetectionCurrentImgFFT;
    vector<float>           m_pfDetectionPreviousRealFFT;
    vector<float>           m_pfClassificationRealFFT;
    vector<float>           m_pfClassificationImgFFT;
    
    //--- Classification ---//
    vector<double>           m_pdFeatureVector;
    vector<double>           m_pdNormalizedVector;
    vector<double>           m_pdFeatureMaxVector;
    vector<double>           m_pdFeatureMinVector;
    
    vector<vector<double>>   m_ppdTrainingData;
    vector<vector<double>>   m_ppdNormalizedData;
    vector<int>              m_piTrainingClassLabels;
    
    vector<double>           m_pdProbabilityEstimates;
    
    
    //--- Evaluation and ReTraining ---//
    vector<vector<double>>   m_ppdEvaluationData;
    vector<int>              m_piEvaluationLabels;
    
    String                   m_psFeatureVector;

};

//========================================================================================================================

#endif  // ONSETCLASSIFICATION_H_INCLUDED
