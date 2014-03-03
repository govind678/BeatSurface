/*
  ==============================================================================

    OnsetClassification.h
    Created: 16 Nov 2013 5:44:30pm
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#ifndef ONSETCLASSIFICATION_H_INCLUDED
#define ONSETCLASSIFICATION_H_INCLUDED

#define MAX_ONSETS_PER_TRAINING     30


#include <algorithm>
#include <fstream>
#include <vector>
#include <stdio.h>


#include "ShortTermFourierTransform.h"
#include "AudioFeatureExtraction.h"
#include "SVMTrain.h"
#include "SVMClassify.h"

//#include "KNNClassifier.h"


#define MAX_DECAY_WINDOW_BLOCKS     25          // For Block Size of 512 ~ 250 ms
#define MAX_DELTA_THRESHOLD         4.0F



//========================================================================================================================

using std::vector;

class OnsetClassification
{
    
public:
    
    //==============================================================================
    // Constructor and Destructor
    
    OnsetClassification(int blockSize, int numChannels, float sampleRate);
    ~OnsetClassification();
    //==============================================================================
    
    
    
    //==============================================================================
    // Process Methods
    
    bool detectOnsets(const float** inputBuffer);
    double* classify();
    void train(int classLabel);
    //==============================================================================


    
    
    //==============================================================================
    // Set and Get Sensitivity Methods
    
    void setVelocitySensitivity (float sensitivity);
    void setDecayTimeSensitivity (float sensitivity);
    float getVelocitySensitivity();
    float getDecayTimeSensitivity();
    //==============================================================================
    
    
    
    //==============================================================================
    // Set Training Params
    
    void addClass();
    void deleteClass(int classIndex);
    void setCurrentClassIndex(int classIndex);
    void userFinishedTraining();
    
    void saveTraining(std::string filePath);
    void loadTraining(std::string filePath);
    //==============================================================================
    
    
private:
    
    
    float* mpCurrentRealFFT;
    float* mpCurrentImgFFT;
    float* mpPreviousRealFFT;
    
    bool onsetDetected;
    
    //--- Low Pass Filter for Detection Function ---//
    float mfNoveltyFunction;
    
    
    //--- Moving Average for Threshold ---//
    float mfAdaptiveThreshold;
    bool mbDecayPeriod;
    int  miDecayBlockCounter;
    
    
    //--- Max Amplitude for Normalization ---//
    float mfCurrentMaxDetectionValue;
    
    
    //--- Spectrum Bandwidth ---//
    int miFrequencyBinLowLimit;
    int miFrequencyBinHighLimit;
    int miNumFeatures;
    
    
    //--- Sensitivity Settings ---//
    float mfDeltaThreshold;
    int   miDecayBlockWindow;
    
    float kfMaxDeltaThreshold;
    int   kiMaxDecayBlockWindow;
    
    
    
    
    ShortTermFourierTransform* stft;
    AudioFeatureExtraction* audioFeature;
    SVMTrain*   svmTrainer;
    SVMClassify* svmClassifier;
    
    svm_model* currentModel;
    
//    KNNClassifier* knnClassifier;
    
    

    
    
    //--- Audio Parameters ---//
    int miBinSize;
    int miBlockSize;
    int miNumChannels;
    float mfSampleRate;
    
    
    
    //--- Training Parameters ---//
    int m_iNumClasses;
    int m_iNumObservations;
    int m_iCurrentClassIndex;
    
    float* m_pfTestingVector;
    vector<vector<float>> m_ppfTrainingData;
    vector<int> m_piClassLabels;
    double* probabilityEstimates;
    
    
    
    
//    std::ofstream spTrainingFile1;
//    std::ofstream spTrainingFile2;
//    std::ofstream spTrainingFile3;
    
//    std::ofstream spOutputFile;
    
//    std::string sTrainingPath;
//    std::string sOutputFilePath;

    
//    float** mpTrainingMatrix1;
//    float** mpTrainingMatrix2;
//    float** mpTrainingMatrix3;

    
//    int miNumOnsets1;
//    int miNumOnsets2;
//    int miNumOnsets3;
    
//    float** mpTrainingMatrix;
//    int* mpClassLabels;
//    float* mpTestVector;
//    int miK;
    

};

//========================================================================================================================

#endif  // ONSETCLASSIFICATION_H_INCLUDED
