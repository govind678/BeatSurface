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

OnsetClassification::OnsetClassification(int blockSize, int numChannels, float sampleRate)
{
    mfSampleRate = sampleRate;
    miBlockSize = blockSize;
    miNumChannels = numChannels;
    miBinSize = miBlockSize/2 + 1;
    

    
    //--- Allocating Memory ---//

    mpCurrentRealFFT    = new float[miBlockSize];
    mpCurrentImgFFT     = new float[miBinSize];
    mpPreviousRealFFT   = new float[miBinSize];
    
    stft = new ShortTermFourierTransform(miBlockSize);
    audioFeature = new AudioFeatureExtraction(miBlockSize);
    
    
    for (int bin = 0; bin < miBinSize; bin++) {
        mpCurrentRealFFT[bin]   = 0;
        mpCurrentImgFFT[bin]    = 0;
        mpPreviousRealFFT[bin]  = 0;
    }
    
    
//    mpTrainingMatrix = nullptr;
//    mpClassLabels    = nullptr;
//    knnClassifier    = nullptr;
    
    
    //--- Initializing Variables ---//
    
    miFrequencyBinHighLimit = 15;
    miFrequencyBinLowLimit = 5;
    miNumFeatures = miFrequencyBinHighLimit - miFrequencyBinLowLimit + 1;
    
    mfNoveltyFunction = 0;
    mfAdaptiveThreshold = 0;
    mfCurrentMaxDetectionValue = 0.3;
    miDecayBlockWindow = 2;
    mfDeltaThreshold = 0.15;
    
    mbDecayPeriod = false;
    miDecayBlockCounter = 0;
    
    
    
//    spTrainingFile1.open(sTrainingPath.append("Class1.txt"));
//    spTrainingFile2.open(sTrainingPath.append("Class2.txt"));
//    spTrainingFile3.open(sTrainingPath.append("Class3.txt"));
//    
//    spOutputFile.open(sOutputFilePath.append("Output.txt"));
    
    
//    mpTrainingMatrix1 = new float* [MAX_ONSETS_PER_TRAINING];
//    mpTrainingMatrix2 = new float* [MAX_ONSETS_PER_TRAINING];
//    mpTrainingMatrix3 = new float* [MAX_ONSETS_PER_TRAINING];
//    
//    mpTestVector = new float [miNumFeatures];
//    
//    for (int i=0; i < MAX_ONSETS_PER_TRAINING; i++) {
//        mpTrainingMatrix1[i] = new float [miNumFeatures];
//        mpTrainingMatrix2[i] = new float [miNumFeatures];
//        mpTrainingMatrix3[i] = new float [miNumFeatures];
//    }
//    
//
//    
//    miNumOnsets1 = 0;
//    miNumOnsets2 = 0;
//    miNumOnsets3 = 0;
    
    
//    miK = 1;
    
    
    //--- Train Parameters ---//
    m_iCurrentClassIndex = 0;
    m_iNumClasses = 0;
    m_iNumObservations = 0;
    m_ppfTrainingData.resize(miNumFeatures);
    
    probabilityEstimates    = nullptr;
    svmTrainer              = nullptr;
    svmClassifier           = nullptr;
    
    m_pfTestingVector = new float[miNumFeatures];
    
    svmClassifier = new SVMClassify();
    
    
}



OnsetClassification::~OnsetClassification()
{
//    spTrainingFile1.close();
//    spTrainingFile2.close();
//    spTrainingFile3.close();
//    spOutputFile.close();
    
    
    delete [] mpCurrentRealFFT;
    delete [] mpCurrentImgFFT;
    delete [] mpPreviousRealFFT;
    
    delete [] m_pfTestingVector;
    

//    delete [] mpTrainingMatrix1;
//    delete [] mpTrainingMatrix2;
//    delete [] mpTrainingMatrix3;
//
//    delete [] mpTrainingMatrix;
//    delete [] mpClassLabels;
//
//    delete [] mpTestVector;
//
//    delete knnClassifier;
    
    if (probabilityEstimates != nullptr) {
        delete [] probabilityEstimates;
    }
    probabilityEstimates = nullptr;
    
    if(svmClassifier != nullptr)
    {
        delete svmClassifier;
    }
    svmClassifier = nullptr;
    
    
    if (svmTrainer != nullptr) {
        delete svmTrainer;
    }
    svmTrainer = nullptr;
    
    
    delete stft;
    delete audioFeature;

}




//====================================== Process Methods ============================================


//==============================================================================
// Return True if Audio Block Consists of an Onset
// !!! Running on Audio Thread
//==============================================================================
bool OnsetClassification::detectOnsets(const float** input)
{
    
    //--- Compute STFT on 1st channel ---//
    stft->computeFFT(input[0], mpCurrentRealFFT, mpCurrentImgFFT);
    
    
    
    //--- Compute Spectral Flux ---//
    mfNoveltyFunction = audioFeature->spectralFlux(mpPreviousRealFFT, mpCurrentRealFFT);
    
    
    
    //--- Create Threshold ---//
    mfAdaptiveThreshold = mfDeltaThreshold + (mfNoveltyFunction + mfAdaptiveThreshold) / 2;
    
    

    //--- Store Current FFT ---//
    for (int bin = 0; bin < miBinSize; bin++) {
        mpPreviousRealFFT[bin] = mpCurrentRealFFT[bin];
    }
    
    
    
    //--- Check for Peaks ---//
    if (mfNoveltyFunction > mfAdaptiveThreshold)
    {
        if (miDecayBlockCounter == 0) {
            mbDecayPeriod = true;
            miDecayBlockCounter++;
            return true;
        }
    }
    
    
    //--- If Not Decay, Reset Counter ---//
    if (mbDecayPeriod) {
        if (miDecayBlockCounter == miDecayBlockWindow) {
            mbDecayPeriod = false;
            miDecayBlockCounter = 0;
        } else {
            miDecayBlockCounter++;
        }
    }
    
    
    return false;
}



//==============================================================================
// Classify Current Audio Block if Onset Detected
// !!! Running on Audio Thread
//==============================================================================
double* OnsetClassification::classify()
{
    for (int feature = 0; feature < miNumFeatures; feature++) {
        m_pfTestingVector[feature] = mpCurrentRealFFT[miFrequencyBinLowLimit + feature];
    }
    
    probabilityEstimates = svmClassifier->classify(m_pfTestingVector, miNumFeatures);
    
    std::cout << "Out: ";
    for (int i=0; i < m_iNumClasses; i++) {
        std::cout << probabilityEstimates[i] << "  ";
    }
    std::cout << std::endl;
    
    return probabilityEstimates;
    
}



//==============================================================================
// Add Training Sample if block consists of Onset
// !!! Running on Audio Thread
//==============================================================================
void OnsetClassification::train(int classLabel)
{
    //    testTraining.push_back(audioFeature->spectralCentroid(mpCurrentRealFFT));
    
    for (int feature = 0; feature < miNumFeatures; feature++) {
        m_ppfTrainingData[feature].push_back(mpCurrentRealFFT[miFrequencyBinLowLimit + feature]);
    }
    
    m_piClassLabels.push_back(classLabel);
    
    m_iNumObservations++;
    
}



//===================================================================================================









//====================================== Parameter Methods ============================================


//==============================================================================
// Set Audio Parameters - Not Running on Audio Thread
//==============================================================================

void OnsetClassification::setVelocitySensitivity(float sensitivity)
{
    mfDeltaThreshold = sensitivity * MAX_DELTA_THRESHOLD;
}


float OnsetClassification::getVelocitySensitivity()
{
    return (mfDeltaThreshold / MAX_DELTA_THRESHOLD);
}


void OnsetClassification::setDecayTimeSensitivity(float sensitivity)
{
    miDecayBlockWindow = (int) (sensitivity * MAX_DECAY_WINDOW_BLOCKS);
}


float OnsetClassification::getDecayTimeSensitivity()
{
    return (miDecayBlockWindow / MAX_DECAY_WINDOW_BLOCKS);
}





//==============================================================================
// Set Training Parameters - Not Running on Audio Thread
//==============================================================================

void OnsetClassification::addClass()
{
    m_iNumClasses++;
}

void OnsetClassification::deleteClass(int classIndex)
{
    m_iNumClasses--;
    
    if (m_iNumClasses < 0) {
        m_iNumClasses = 0;
    }
    
}

void OnsetClassification::setCurrentClassIndex(int classIndex)
{
    m_iCurrentClassIndex = classIndex;
}


void OnsetClassification::userFinishedTraining()
{
    if (svmTrainer != nullptr) {
        delete svmTrainer;
        svmTrainer = nullptr;
    }
    
    svmTrainer = new SVMTrain();
    
    
//    if (svmClassifier) {
//        delete svmClassifier;
//        svmClassifier = nullptr;
//    }
    
//    svmClassifier = new SVMClassify();

    
    // Recreate Probability Estimates
    if (probabilityEstimates != nullptr) {
        delete [] probabilityEstimates;
    }
    
    probabilityEstimates = new double [m_iNumClasses];
    
    float** ppfTrainingData     = new float* [m_iNumObservations];
    float*  pfTrainingLabels    = new float [m_iNumObservations];
    
    
    for (int i=0; i < m_iNumObservations; i++) {
        ppfTrainingData[i] = new float [miNumFeatures];
    }
    
    
//    std::cout << "Training Data for " << m_iNumObservations << " Observations: \n\n" << std::endl;
    
    for (int observation = 0; observation < m_iNumObservations; observation++)
    {
//        std::cout << "Class: " << m_piClassLabels[observation] << std::endl;
        
        pfTrainingLabels[observation] = m_piClassLabels[observation];
        
        for (int feature = 0; feature < miNumFeatures; feature++)
        {
            ppfTrainingData[observation][feature] = m_ppfTrainingData[feature][observation];
            
//            std::cout << m_ppfTrainingData[feature][observation] << "\t";
        }
        
//        std::cout << std::endl;
    }
    
    svmTrainer->setTrainingDataAndTrain(ppfTrainingData, pfTrainingLabels, miNumFeatures, m_iNumObservations);
    svmClassifier->setCurrentSVMModel(svmTrainer->getCurrentSVMModel());

    
    for (int i=0; i < m_iNumObservations; i++) {
        delete [] ppfTrainingData[i];
    }
    
    delete [] ppfTrainingData;
    delete [] pfTrainingLabels;
    
}



void OnsetClassification::saveTraining(std::string filePath)
{
    svmTrainer->saveModelToDisk(filePath);
}


void OnsetClassification::loadTraining(std::string filePath)
{
    svmClassifier->loadModelFromDisk(filePath);
}


//===================================================================================================

