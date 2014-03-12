/*
  ==============================================================================

    OnsetClassification.cpp
    Created: 16 Nov 2013 5:44:30pm
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#include "OnsetClassification.h"
#include <stdio.h>
#include <iostream>

OnsetClassification::OnsetClassification()
{
    
    //--- Initializing Device Settings ---//
    m_sDeviceSettings.iBufferSize       =   0;
    m_sDeviceSettings.fSampleRate       =   0;
    m_sDeviceSettings.iNumChannels      =   0;
    m_iBinSize                          =   0;
    
    
    
    //--- Initializing Detection Parameters ---//
    m_sDetectionParameters.iDecayBlockWindow    =   2;
    m_sDetectionParameters.fDeltaThreshold      =   0.15f;
    
    
    
    //--- Initializing Training Parameters ---//
    m_sTrainingParameters.iNumClasses           =   0;
    m_sTrainingParameters.iNumFeatures          =   0;
    m_sTrainingParameters.iNumObservations      =   0;
    m_sTrainingParameters.bDidFinishTraining    =   false;
    
    
    
    //--- Initializing Buffers ---//
    
    m_pfCurrentImgFFT.assign(0, 0.0f);
    m_pfCurrentRealFFT.assign(0, 0.0f);
    m_pfPreviousRealFFT.assign(0, 0.0f);
    
    m_pfTestingVector.assign(0, 0.0f);
    m_piTrainingClassLabels.assign(0, 0);
    m_pdProbabilityEstimates.assign(0, 0.0f);
    
    m_ppfTrainingData.assign(0, vector<float> (0,0));
    
    

    
    //--- Allocating Memory / Creating Instances for Utility Objects ---//
    m_pcStft            = new ShortTermFourierTransform();
    m_pcAudioFeature    = new AudioFeatureExtraction();
    m_pcSvmTrainer      = nullptr;



    
    //--- Initializing Variables ---//
    m_iFrequencyBinHighLimit            = 20;
    m_iFrequencyBinLowLimit             = 3;
    m_sTrainingParameters.iNumFeatures  = m_iFrequencyBinHighLimit - m_iFrequencyBinLowLimit + 1;
    

    m_fNoveltyFunction                  = 0.0f;
    m_fAdaptiveThreshold                = 0.0f;

    m_bDecayPeriod                      = false;
    m_iDecayBlockCounter                = 0;
    
    m_fCurrentRootMeanSquare            = 0.0f;
    m_fCurrentSpectralCentroid          = 0.0f;



    //--- Train Parameters ---//
    m_iCurrentClassIndex            =   0;
    
    
    //--- Resizing Buffers based on Number of Features ---//
    m_ppfTrainingData.assign(m_sTrainingParameters.iNumFeatures, vector<float> (0,0.0f));
    m_pfTestingVector.assign(m_sTrainingParameters.iNumFeatures, 0.0f);
    
}




//==============================================================================
// Set Audio Device Settings - Initializer Method
//==============================================================================

void OnsetClassification::setAudioDeviceSettings(OnsetClassification::AudioDeviceSettings newSettings)
{
    m_sDeviceSettings = newSettings;
    
    m_iBinSize = (m_sDeviceSettings.iBufferSize / 2) + 1;
    
    m_pfCurrentRealFFT.assign(m_iBinSize, 0.0f);
    m_pfCurrentImgFFT.assign(m_iBinSize, 0.0f);
    m_pfPreviousRealFFT.assign(m_iBinSize, 0.0f);
    
    m_pcStft->setBufferSize(m_sDeviceSettings.iBufferSize);
    m_pcAudioFeature->setBufferSize(m_sDeviceSettings.iBufferSize);
}




OnsetClassification::~OnsetClassification()
{
    m_pfCurrentRealFFT.clear();
    m_pfPreviousRealFFT.clear();
    m_pfCurrentImgFFT.clear();
    
    m_pfTestingVector.clear();
    m_ppfTrainingData.clear();
    m_piTrainingClassLabels.clear();
    m_pdProbabilityEstimates.clear();
    

    if (m_pcSvmTrainer != nullptr)
    {
        delete m_pcSvmTrainer;
    }
    m_pcSvmTrainer = nullptr;
    
    delete m_pcStft;
    delete m_pcAudioFeature;

}




//====================================== Process Methods ============================================


//==============================================================================
// Return True if Audio Block Consists of an Onset
// !!! Running on Audio Thread
//==============================================================================
bool OnsetClassification::detectOnsets(const float** input)
{
    
    //--- Compute STFT on 1st channel ---//
    m_pcStft->computeFFT(input[0], m_pfCurrentRealFFT.data(), m_pfCurrentImgFFT.data());
    
    
    
    //--- Compute Spectral Flux ---//
    m_fNoveltyFunction = m_pcAudioFeature->spectralFlux(m_pfPreviousRealFFT.data(), m_pfCurrentRealFFT.data());
    
    
    
    //--- Create Adaptive Threshold ---//
    m_fAdaptiveThreshold = m_sDetectionParameters.fDeltaThreshold + (m_fNoveltyFunction + m_fAdaptiveThreshold) / 2;
    
    

    //--- Store Current FFT ---//
    memcpy(m_pfPreviousRealFFT.data(), m_pfCurrentRealFFT.data(), m_iBinSize);
    
    
    
    //--- Compute Spectral Centroid ---//
    m_fCurrentSpectralCentroid = m_pcAudioFeature->spectralCentroid(m_pfCurrentRealFFT.data()) / m_iBinSize;
    
    
    //--- Check for Peaks ---//
    if (m_fNoveltyFunction > m_fAdaptiveThreshold)
    {
        if (m_iDecayBlockCounter == 0)
        {
            m_bDecayPeriod = true;
            m_iDecayBlockCounter++;
            return true;
        }
    }
    
    
    
    //--- If Not Decay, Reset Counter ---//
    if (m_bDecayPeriod)
    {
        if (m_iDecayBlockCounter == m_sDetectionParameters.iDecayBlockWindow)
        {
            m_bDecayPeriod = false;
            m_iDecayBlockCounter = 0;
        }
        
        else
        {
            m_iDecayBlockCounter++;
        }
    }
    
    
    return false;
}



//==============================================================================
// Classify Current Audio Block if Onset Detected
// !!! Running on Audio Thread
//==============================================================================
int OnsetClassification::classify()
{
    if (m_sTrainingParameters.bDidFinishTraining)
    {
        for (int feature = 0; feature < m_sTrainingParameters.iNumFeatures; feature++)
        {
            m_pfTestingVector[feature] = m_pfCurrentRealFFT[m_iFrequencyBinLowLimit + feature];
        }
        
//        m_pfTestingVector.assign(m_pfCurrentRealFFT.at(m_iFrequencyBinLowLimit), m_pfCurrentRealFFT.at(m_iFrequencyBinHighLimit));
//        double* probability = m_pcSvmTrainer->classify(m_pfTestingVector.data(), m_sTrainingParameters.iNumFeatures);
//        m_pdProbabilityEstimates.assign(std::begin(probability), std::end(probability));
        
//        if (probabilityEstimates != nullptr)
//        {
//            probabilityEstimates = m_pcSvmTrainer->classify(m_pfTestingVector.data(), m_sTrainingParameters.iNumFeatures);
//        }
//        
//        else
//        {
//            m_pcSvmTrainer->classify(m_pfTestingVector.data(), m_sTrainingParameters.iNumFeatures);
//        }
        
        m_pcSvmTrainer->classify(m_pfTestingVector.data(), m_sTrainingParameters.iNumFeatures);
        return int(m_pcSvmTrainer->getResult());
    }
    
    else
    {
        return 0;
    }
    
}


double* OnsetClassification::getProbabilityEstimates()
{
    return m_pdProbabilityEstimates.data();
}


//==============================================================================
// Add Training Sample if block consists of Onset
// !!! Running on Audio Thread
//==============================================================================
void OnsetClassification::train(int classLabel)
{
    for (int feature = 0; feature < m_sTrainingParameters.iNumFeatures; feature++)
    {
        m_ppfTrainingData.at(feature).push_back(m_pfCurrentRealFFT[m_iFrequencyBinLowLimit + feature]);
    }
    
    m_piTrainingClassLabels.push_back(classLabel);
    
    m_sTrainingParameters.iNumObservations++;
    
}


//==============================================================================
// Compute Root Mean Square of Time Domain Signal
// !!! Running on Audio Thread
//==============================================================================
float OnsetClassification::rootMeanSquare(const float **inputBuffer)
{
    m_fCurrentRootMeanSquare = m_pcAudioFeature->rootMeanSquare(inputBuffer, m_sDeviceSettings.iNumChannels);
    return m_fCurrentRootMeanSquare;
}

//===================================================================================================









//====================================== Parameter Methods ============================================


//==============================================================================
// Set Audio Parameters - Not Running on Audio Thread
//==============================================================================

void OnsetClassification::setVelocitySensitivity(float sensitivity)
{
    m_sDetectionParameters.fDeltaThreshold = sensitivity * MAX_DELTA_THRESHOLD;
}


float OnsetClassification::getVelocitySensitivity()
{
    return (m_sDetectionParameters.fDeltaThreshold / MAX_DELTA_THRESHOLD);
}


void OnsetClassification::setDecayTimeSensitivity(float sensitivity)
{
    m_sDetectionParameters.iDecayBlockWindow = (int) (sensitivity * MAX_DECAY_WINDOW_BLOCKS);
}


float OnsetClassification::getDecayTimeSensitivity()
{
    return (m_sDetectionParameters.iDecayBlockWindow / MAX_DECAY_WINDOW_BLOCKS);
}




//==============================================================================
// Set Training Parameters - Not Running on Audio Thread
//==============================================================================

void OnsetClassification::addClass()
{
    m_sTrainingParameters.iNumClasses++;
    
    m_pdProbabilityEstimates.resize(m_sTrainingParameters.iNumClasses, 0.0f);
}

void OnsetClassification::deleteClass(int classIndex)
{
    m_sTrainingParameters.iNumClasses--;
    
    if (m_sTrainingParameters.iNumClasses < 0)
    {
        m_sTrainingParameters.iNumClasses = 0;
    }
    
    
    m_pdProbabilityEstimates.resize(m_sTrainingParameters.iNumClasses, 0.0f);
    
}


void OnsetClassification::setCurrentClassIndex(int classIndex)
{
    m_iCurrentClassIndex = classIndex;
}


void OnsetClassification::doneTraining()
{
    if (m_sTrainingParameters.iNumClasses > 1)
    {
    
        //--- Delete Current Training ---//
        if (m_pcSvmTrainer != nullptr)
        {
            delete m_pcSvmTrainer;
            m_pcSvmTrainer = nullptr;
        }
    
    
        //--- Create New SVM Trainer and Classifier ---//
        m_pcSvmTrainer = new SVMTrain();
    
        
        SVMBase::Error_t error  = m_pcSvmTrainer->setTrainingDataAndTrain(m_ppfTrainingData,
                                                                      m_piTrainingClassLabels,
                                                                      m_sTrainingParameters.iNumFeatures,
                                                                      m_sTrainingParameters.iNumObservations);
    
        if (error == SVMBase::kNoError)
        {
            m_sTrainingParameters.bDidFinishTraining    = true;
        }
        
    }
    
}




int OnsetClassification::saveTraining(std::string filePath)
{
    if (m_pcSvmTrainer != nullptr)
    {
        if (m_sTrainingParameters.bDidFinishTraining)
        {
            m_pcSvmTrainer->saveModelToDisk(filePath);
            return 0;
        }
    }
    
    return 1;
}



void OnsetClassification::loadTraining(std::string filePath)
{
    m_pcSvmTrainer->loadModelFromDisk(filePath);
}


int OnsetClassification::getNumClasses()
{
    return m_sTrainingParameters.iNumClasses;
}

//===================================================================================================



vector<float> OnsetClassification::getCurrentSpectrum()
{
    return m_pfCurrentRealFFT;
}


float OnsetClassification::getCurrentSpectralCentroid()
{
    return m_fCurrentSpectralCentroid;
}

