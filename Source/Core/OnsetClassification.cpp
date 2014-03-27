/*
  ==============================================================================

    OnsetClassification.cpp
    Created: 16 Nov 2013 5:44:30pm
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#include "OnsetClassification.h"

OnsetClassification::OnsetClassification()
{
    
    //--- Initializing Device Settings ---//
    m_sDeviceSettings.iBufferSize                   =   0;
    m_sDeviceSettings.dSampleRate                   =   0;
    m_sDeviceSettings.iNumChannels                  =   0;
    m_sDeviceSettings.iNumBins                      =   0;

    
    
    
    //--- Initializing Detection Parameters ---//
    m_sDetectionParameters.iDecayBlockWindow    =   MIN_DECAY_WINDOW_BLOCKS;
    m_sDetectionParameters.dDeltaThreshold      =   MIN_DELTA_THRESHOLD;
    
    
    
    //--- Initializing Training Parameters ---//
    m_sTrainingParameters.iNumFeatures          =   3;
    m_sTrainingParameters.iNumClasses           =   0;
    m_sTrainingParameters.iNumObservations      =   0;
    m_sTrainingParameters.dCrossValidationAccuracy = 0.0f;
    m_sTrainingParameters.bDidFinishTraining    =   false;
    m_sTrainingParameters.iCurrentObservation   =   0;
    
    
    
    //--- Initializing Buffers ---//
    m_pfDetectionCurrentImgFFT.assign(0, 0.0f);
    m_pfDetectionCurrentRealFFT.assign(0, 0.0f);
    m_pfDetectionPreviousRealFFT.assign(0, 0.0f);
    m_pfClassificationRealFFT.assign(0, 0.0f);
    m_pfClassificationImgFFT.assign(0, 0.0f);
    m_pfClassificationAudioBlock.assign(0, 0.0f);
    m_pfDetectionAudioBlock.assign(0, 0.0f);
    
    //--- Initialize Classification Vectors ---//
    m_pdFeatureVector.assign(m_sTrainingParameters.iNumFeatures, 0.0f);
    m_pdFeatureMaxVector.assign(m_sTrainingParameters.iNumFeatures, 0.0f);
    m_pdFeatureMinVector.assign(m_sTrainingParameters.iNumFeatures, 0.0f);
    m_pdNormalizedVector.assign(0, 0.0f);
    m_piTrainingClassLabels.assign(0, 0);
    m_ppdTrainingData.assign(0, vector<double> (0,0.0f));
    m_ppdNormalizedData.assign(0, vector<double> (0,0.0f));
    m_pdProbabilityEstimates.assign(1, 0.0f);
    m_ppdEvaluationData.assign(0, vector<double> (0,0.0f));
    m_piEvaluationLabels.assign(0, 0);

    m_psFeatureVector.clear();
    
    
    //--- Allocating Memory / Creating Instances for Utility Objects ---//
    m_pcAudioFeature            = new AudioFeatureExtraction();
    m_pcDetectionSTFT           = nullptr;
    m_pcClassificationSTFT      = nullptr;
    m_pcSVMTrainer              = nullptr;



    
    //--- Initializing Variables ---//
    m_sDetectionVariables.dAdaptiveThreshold    =   0.0f;
    m_sDetectionVariables.iDecayBlockCounter    =   0;
    m_sDetectionVariables.bDecayPeriod          =   false;
    
    m_sAudioFeatures.dSpectralCentroid          =   0.0f;
    m_sAudioFeatures.dSpectralFlatness          =   0.0f;
    m_sAudioFeatures.dSpectralSlope             =   0.0f;
    
    m_sAudioFeatures.dSpectralFlux              =   0.0f;
    m_sAudioFeatures.dRootMeanSquare            =   0.0f;
    
}




//==============================================================================
// Set Audio Device Settings - Initializer Method (Called in Prepare To Play)
//==============================================================================

void OnsetClassification::initialize(int blockSize, double sampleRate)
{
    m_sDeviceSettings.iBufferSize          = blockSize;
    m_sDeviceSettings.dSampleRate          = sampleRate;
    m_sDeviceSettings.iNumBins             = (m_sDeviceSettings.iBufferSize / 2) + 1;
    
    
    //--- Resize Buffers based on new blockSize ---//
    m_pfDetectionAudioBlock.assign(m_sDeviceSettings.iBufferSize, 0.0f);
    m_pfClassificationAudioBlock.assign((2 * m_sDeviceSettings.iBufferSize), 0.0f);
    
    m_pfDetectionCurrentRealFFT.assign(m_sDeviceSettings.iNumBins, 0.0f);
    m_pfDetectionCurrentImgFFT.assign(m_sDeviceSettings.iNumBins, 0.0f);
    m_pfDetectionPreviousRealFFT.assign(m_sDeviceSettings.iNumBins, 0.0f);
    
    m_pfClassificationRealFFT.assign(blockSize + 1, 0.0f);
    m_pfClassificationImgFFT.assign(blockSize + 1, 0.0f);
    
    
    //--- Re-initalize STFT based on block size ---//
    m_pcDetectionSTFT       =   nullptr;
    m_pcClassificationSTFT  =   nullptr;
    
    m_pcDetectionSTFT       =   new ShortTermFourierTransform(blockSize);
    m_pcClassificationSTFT  =   new ShortTermFourierTransform(2 * blockSize);
}




//==============================================================================
// Destructor
//==============================================================================
OnsetClassification::~OnsetClassification()
{
    m_pfDetectionCurrentRealFFT.clear();
    m_pfDetectionPreviousRealFFT.clear();
    m_pfDetectionCurrentImgFFT.clear();
    
    m_pfClassificationRealFFT.clear();
    m_pfClassificationImgFFT.clear();
    
    m_pfClassificationAudioBlock.clear();
    
    m_pdFeatureVector.clear();
    m_pdFeatureMaxVector.clear();
    m_pdFeatureMinVector.clear();
    m_pdNormalizedVector.clear();
    m_ppdTrainingData.clear();
    m_piTrainingClassLabels.clear();
    m_ppdNormalizedData.clear();
    m_pdProbabilityEstimates.clear();
    m_ppdEvaluationData.clear();
    m_piEvaluationLabels.clear();
    
    m_psFeatureVector.clear();
    
    m_pcDetectionSTFT       = nullptr;
    m_pcClassificationSTFT  = nullptr;
    m_pcAudioFeature        = nullptr;
    m_pcSVMTrainer          = nullptr;
}






//====================================== Process Methods ============================================


//==============================================================================
// Return True if Audio Block Consists of an Onset
// !!! Running on Audio Thread
//==============================================================================
bool OnsetClassification::detectOnset(const float** audioBuffer)
{
    m_pfDetectionAudioBlock.insert(m_pfDetectionAudioBlock.begin(), audioBuffer[0], audioBuffer[0] + m_sDeviceSettings.iBufferSize);
    return internal_detectOnset();

}

bool OnsetClassification::detectOnset(float** audioBuffer)
{
    m_pfDetectionAudioBlock.insert(m_pfDetectionAudioBlock.begin(), audioBuffer[0], audioBuffer[0] + m_sDeviceSettings.iBufferSize);
    return internal_detectOnset();
}



//==============================================================================
// Internal Onset Detection
// !!! Running on Audio Thread
//==============================================================================

bool OnsetClassification::internal_detectOnset()
{
    //--- Compute STFT on 1st channel ---//
    m_pcDetectionSTFT->computeFFT(m_pfDetectionAudioBlock.data(),
                                  m_pfDetectionCurrentRealFFT.data(),
                                  m_pfDetectionCurrentImgFFT.data());
    
    
    
    //--- Compute Spectral Flux ---//
    m_sAudioFeatures.dSpectralFlux = m_pcAudioFeature->spectralFlux(m_pfDetectionPreviousRealFFT.data(),
                                                                    m_pfDetectionCurrentRealFFT.data(),
                                                                    m_sDeviceSettings.iBufferSize);
    
    
    
    //--- Create Adaptive Threshold ---//
    m_sDetectionVariables.dAdaptiveThreshold =  m_sDetectionParameters.dDeltaThreshold
    +   (m_sAudioFeatures.dSpectralFlux
         + m_sDetectionVariables.dAdaptiveThreshold) / 2.0f;
    
    
    
    //--- Store Current FFT ---//
    m_pfDetectionPreviousRealFFT = m_pfDetectionCurrentRealFFT;

    
    
    //--- Store Current Audio Buffer ---//
    for (int i=0; i < m_sDeviceSettings.iBufferSize; i++)
    {
        m_pfClassificationAudioBlock[i] = m_pfDetectionAudioBlock[i];
    }
    
    
    
    
    //--- Check for Peaks ---//
    if (m_sAudioFeatures.dSpectralFlux > m_sDetectionVariables.dAdaptiveThreshold)
    {
        if (m_sDetectionVariables.iDecayBlockCounter == 0)
        {
            m_sDetectionVariables.bDecayPeriod = true;
            m_sDetectionVariables.iDecayBlockCounter++;
            m_sTrainingParameters.iCurrentObservation++;
            return true;
        }
    }
    
    
    
    //--- If Not Decay, Reset Counter ---//
    if (m_sDetectionVariables.bDecayPeriod)
    {
        if (m_sDetectionVariables.iDecayBlockCounter >= m_sDetectionParameters.iDecayBlockWindow)
        {
            m_sDetectionVariables.bDecayPeriod = false;
            m_sDetectionVariables.iDecayBlockCounter = 0;
        }
        
        else
        {
            m_sDetectionVariables.iDecayBlockCounter++;
        }
    }
    
    
    return false;
}



//==============================================================================
// Classify Current Audio Block if Onset Detected
// !!! Running on Audio Thread
//==============================================================================
int OnsetClassification::classify(const float** audioBuffer)
{
    m_pfDetectionAudioBlock.insert(m_pfDetectionAudioBlock.begin(), audioBuffer[0], audioBuffer[0] + m_sDeviceSettings.iBufferSize);
    return internal_classify();
}


int OnsetClassification::classify(float** audioBuffer)
{
    m_pfDetectionAudioBlock.insert(m_pfDetectionAudioBlock.begin(), audioBuffer[0], audioBuffer[0] + m_sDeviceSettings.iBufferSize);
    return internal_classify();
}


//==============================================================================
// Internal Onset Detection
// !!! Running on Audio Thread
//==============================================================================

int OnsetClassification::internal_classify()
{
    //--- If only one class or no classes is/are trained, return ---//
    if (m_sTrainingParameters.iNumClasses < 2)
    {
        return 1;
    }
    
    else
    {
        if (!m_sTrainingParameters.bDidFinishTraining)
        {
            return 0;
        }
        
        else
        {
            //--- Concatenate Previous and Current Audio Blocks ---//
            for (int sample = 0; sample < m_sDeviceSettings.iBufferSize; sample++)
            {
                m_pfClassificationAudioBlock[m_sDeviceSettings.iBufferSize + sample] = m_pfDetectionAudioBlock[sample];
            }
            
            
            //--- Re-compute FFT for twice blocksize ---//
            m_pcClassificationSTFT->computeFFT(m_pfClassificationAudioBlock.data(),
                                               m_pfClassificationRealFFT.data(),
                                               m_pfClassificationImgFFT.data());
            
            
            
            //--- Extract Audio Features ---//
            m_pcAudioFeature->extractFeatures(m_pfClassificationRealFFT.data(),
                                              m_pdFeatureVector.data(),
                                              (2 * m_sDeviceSettings.iBufferSize));
            
            
            
            //--- Normalize data for Classification ---//
            normalizeVector(m_pdFeatureVector.data(), m_pdNormalizedVector.data());
            
            int result = m_pcSVMTrainer->classify(m_pdNormalizedVector.data(), m_pdProbabilityEstimates.data());
            
            
            //--- Add Result to Vector for Evaluation ---//
            m_ppdEvaluationData.push_back(m_pdFeatureVector);
            m_piEvaluationLabels.push_back(result);
            
            
            
            //--- Printout Probability Estimates ---//
            std::cout << result << ":\t";
            for (int c = 0; c < m_sTrainingParameters.iNumClasses; c++)
            {
                std::cout << m_pdProbabilityEstimates[c] << "\t";
            }
            std::cout << std::endl;
            
            
            //--- Return Resulting Class ---//
            return result;
            
        }
    }
}


std::vector<double> OnsetClassification::getCurrentProbabilityEstimates()
{
    return m_pdProbabilityEstimates;
}


//==============================================================================
// Add Training sample if block consists of Onset
// !!! Running on Audio Thread
//==============================================================================
void OnsetClassification::trainAtClass(float** audioBuffer, int classLabel)
{
    m_pfDetectionAudioBlock.insert(m_pfDetectionAudioBlock.begin(), audioBuffer[0], audioBuffer[0] + m_sDeviceSettings.iBufferSize);
    internal_trainAtClass(classLabel);
}


void OnsetClassification::trainAtClass(const float** audioBuffer, int classLabel)
{
    m_pfDetectionAudioBlock.insert(m_pfDetectionAudioBlock.begin(), audioBuffer[0], audioBuffer[0] + m_sDeviceSettings.iBufferSize);
    internal_trainAtClass(classLabel);
}


void OnsetClassification::internal_trainAtClass(int classLabel)
{
    for (int sample = 0; sample < m_sDeviceSettings.iBufferSize; sample++)
    {
        m_pfClassificationAudioBlock[m_sDeviceSettings.iBufferSize + sample] = m_pfDetectionAudioBlock[sample];
    }
    
    
    m_pcClassificationSTFT->computeFFT(m_pfClassificationAudioBlock.data(),
                                       m_pfClassificationRealFFT.data(),
                                       m_pfClassificationImgFFT.data());
    
    
    m_pcAudioFeature->extractFeatures(m_pfClassificationRealFFT.data(),
                                      m_pdFeatureVector.data(),
                                      (2 * m_sDeviceSettings.iBufferSize));
    
    
    
    //--- Printout Features ---//
    std::cout << classLabel << ": ";
    for (int i=0; i < m_sTrainingParameters.iNumFeatures; i++)
    {
        std::cout << m_pdFeatureVector[i] << "\t ";
    }
    std::cout << std::endl;
    
    
    
    m_ppdTrainingData.push_back(m_pdFeatureVector);
    m_piTrainingClassLabels.push_back(classLabel);
    
    m_sTrainingParameters.iNumObservations++;
    
}




//==============================================================================
// Run Normalization and Train SVM
// Not running on Audio Thread
//==============================================================================
void OnsetClassification::performTraining()
{
    if (m_sTrainingParameters.iNumObservations >= 1)
    {
        //--- Find Max and Min Values for Normalization ---//
        findMaxAndMinVectors();
        
        
        //--- Normalize Data ---//
        m_ppdNormalizedData.clear();
        m_ppdNormalizedData.assign(m_sTrainingParameters.iNumObservations, vector<double>(m_sTrainingParameters.iNumFeatures, 0.0f));
        
        m_pdNormalizedVector.clear();
        m_pdNormalizedVector.assign(m_sTrainingParameters.iNumFeatures, 0.0f);
        
        for (int observation = 0; observation < m_sTrainingParameters.iNumObservations; observation++)
        {
            normalizeVector(m_ppdTrainingData.at(observation).data(), m_ppdNormalizedData.at(observation).data());
        }
        
        
        
        //--- Train SVM ---//
        
        if (getTotalTrainedClasses() > 1)
        {
            //--- Delete Current Training ---//
            m_pcSVMTrainer  = nullptr;
            
            
            //--- Create New SVM Trainer and Classifier ---//
            m_pcSVMTrainer = new SVMTrain();
            
            SVMBase::Error_t error  = m_pcSVMTrainer->setTrainingDataAndTrain(m_ppdNormalizedData,
                                                                              m_piTrainingClassLabels,
                                                                              m_sTrainingParameters.iNumFeatures,
                                                                              m_sTrainingParameters.iNumObservations);
            
            if (error == SVMBase::kNoError)
            {
                evaluate();
                m_sTrainingParameters.bDidFinishTraining    = true;
            }
            
            else
            {
                m_sTrainingParameters.bDidFinishTraining    = false;
            }
        }
        
        
        else
        {
            m_sTrainingParameters.dCrossValidationAccuracy = 0.0f;
        }
        
    }
}



//==============================================================================
// Utility Methods for Data Normalization
//==============================================================================

void OnsetClassification::findMaxAndMinVectors()
{
    m_pdFeatureMaxVector.assign(m_sTrainingParameters.iNumFeatures, 0.0f);
    m_pdFeatureMinVector.assign(m_sTrainingParameters.iNumFeatures, 0.0f);
    
    
    for (int feature = 0; feature < m_sTrainingParameters.iNumFeatures; feature++)
    {
        double maxVal = 0.0f;
        double minVal = 0.0f;
        
        for (int observation = 0; observation < m_sTrainingParameters.iNumObservations; observation++)
        {
            if (m_ppdTrainingData[observation][feature] > maxVal)
            {
                maxVal = m_ppdTrainingData[observation][feature];
            }
            
            if (m_ppdTrainingData[observation][feature] < minVal)
            {
                minVal = m_ppdTrainingData[observation][feature];
            }
        }
        
        m_pdFeatureMaxVector[feature]   = maxVal;
        m_pdFeatureMinVector[feature]   = minVal;
    }
}


void OnsetClassification::normalizeVector(double* featureVector, double* normalizedVector)
{
    for (int feature = 0; feature < m_sTrainingParameters.iNumFeatures; feature++)
    {
        normalizedVector[feature] = (featureVector[feature] - m_pdFeatureMinVector[feature])
        / (m_pdFeatureMaxVector[feature] - m_pdFeatureMinVector[feature]);
    }
}



int OnsetClassification::getTotalTrainedClasses()
{
    vector<int> uniqueVector = m_piTrainingClassLabels;
    
    sort(uniqueVector.begin(), uniqueVector.end());
    
    vector<int>::iterator it;
    it = unique (uniqueVector.begin(), uniqueVector.end());
    
    uniqueVector.resize(distance(uniqueVector.begin(),it));
    
    return uniqueVector.size();

}



//==============================================================================
// Run SVM Cross-Validation
//==============================================================================
void OnsetClassification::evaluate()
{
    m_pcSVMTrainer->crossValidationOnTrainingDataset(10, m_sTrainingParameters.dCrossValidationAccuracy);
}





//====================================== Parameter Methods ============================================


//==============================================================================
// Set Audio Parameters
//==============================================================================

void OnsetClassification::setVelocitySensitivity(double sensitivity)
{
    m_sDetectionParameters.dDeltaThreshold = MIN_DELTA_THRESHOLD + (sensitivity * (MAX_DELTA_THRESHOLD - MIN_DELTA_THRESHOLD));
}


void OnsetClassification::setDecayTimeSensitivity(double sensitivity)
{
    m_sDetectionParameters.iDecayBlockWindow = MIN_DECAY_WINDOW_BLOCKS + int(sensitivity * (MAX_DECAY_WINDOW_BLOCKS - MIN_DECAY_WINDOW_BLOCKS));
}


double OnsetClassification::getVelocitySensitivity()
{
    return ((m_sDetectionParameters.dDeltaThreshold - MIN_DELTA_THRESHOLD)
            / (MAX_DELTA_THRESHOLD - MIN_DELTA_THRESHOLD));
}


double OnsetClassification::getDecayTimeSensitivity()
{
    return (double(m_sDetectionParameters.iDecayBlockWindow - MIN_DECAY_WINDOW_BLOCKS)
            / (MAX_DECAY_WINDOW_BLOCKS - MIN_DECAY_WINDOW_BLOCKS));
}




//==============================================================================
// Add and Delete Training Classes from Dataset
//==============================================================================

void OnsetClassification::addClass()
{
    m_sTrainingParameters.iNumClasses++;
    
    m_pdProbabilityEstimates.resize(m_sTrainingParameters.iNumClasses, 0.0f);
}

void OnsetClassification::deleteClass(int classIndex)
{
    if (m_sTrainingParameters.iNumClasses > 0)
    {
        m_sTrainingParameters.iNumClasses--;
        
        vector<int>::iterator itClass               = m_piTrainingClassLabels.begin();
        vector<vector<double>>::iterator itTrain    = m_ppdTrainingData.begin();
        
        while (itClass != m_piTrainingClassLabels.end())
        {
            if (*itClass == classIndex)
            {
                itClass = m_piTrainingClassLabels.erase(itClass);
                itTrain = m_ppdTrainingData.erase(itTrain);
            }
            
            else
            {
                ++itClass;
                ++itTrain;
            }
        }
        
        m_sTrainingParameters.iNumObservations = m_piTrainingClassLabels.size();
        
        
        //--------------------------------------------------
        // Iterate through class labels to restructure
        // all classes as continously ascending from 1.
        // Ex: if numClasses = 3, and class 2 was deleted,
        // make class 3 = 2
        for (int i=0; i < m_sTrainingParameters.iNumObservations; i++)
        {
            if (m_piTrainingClassLabels[i] > classIndex)
            {
                m_piTrainingClassLabels[i]--;
            }
        }
        
        
        
        //--- Redo Training for New Num Classes ---//
        performTraining();
        
        
        //--- Resize Probability Estimates to fit New Num Classes ---//
        m_pdProbabilityEstimates.resize(m_sTrainingParameters.iNumClasses, 0.0f);
    }
    
    
    
    
}




//==============================================================================
// Save and Load Training to Disk
//==============================================================================

int OnsetClassification::saveTraining(File trainingFile)
{
    if (m_sTrainingParameters.bDidFinishTraining)
    {
        String fileName = trainingFile.getFileNameWithoutExtension();
        Time currentTime = Time::getCurrentTime();
        
        String dataToWrite = "Beat Surface Training: \t " + fileName + "\n"
        + currentTime.toString(true, true, false, false) + "\n"
        + "Observations: " + String(m_sTrainingParameters.iNumObservations) + "\n"
        + "Classes: " + String(m_sTrainingParameters.iNumClasses) + "\n"
        + "Features: " + String(m_sTrainingParameters.iNumFeatures) + "\n"
        + "VelocityThresholdSetting: " + String(m_sDetectionParameters.dDeltaThreshold) + "\n"
        + "DecayTimeSetting: " + String(m_sDetectionParameters.iDecayBlockWindow) + "\n"
        + "\nClass: \t Audio Features: \n";
        
        for (int observation = 0; observation < m_sTrainingParameters.iNumObservations; observation++)
        {
            dataToWrite += String(m_piTrainingClassLabels[observation]) + ";";
            
            for (int feature = 0; feature < m_sTrainingParameters.iNumFeatures; feature++)
            {
                dataToWrite +=  String(m_ppdTrainingData[observation][feature]) + ";";
            }
            
            dataToWrite += "\n";
        }
        
        trainingFile.replaceWithText(dataToWrite, false, false);
        
        return 0;
    }
    
    return 1;
}



void OnsetClassification::loadTraining(File trainingFile)
{
    if (trainingFile.getFileExtension() == ".txt")
    {
        if(trainingFile.existsAsFile())
        {
            StringArray trainingDataString;
            trainingFile.readLines(trainingDataString);
            
            m_sTrainingParameters.iNumObservations      = trainingDataString[2].fromFirstOccurrenceOf("Observations: ",
                                                                                                      false, false).getIntValue();
            
            m_sTrainingParameters.iNumClasses           = trainingDataString[3].fromFirstOccurrenceOf("Classes: ",
                                                                                                      false, false).getIntValue();
            
            m_sTrainingParameters.iNumFeatures          = trainingDataString[4].fromFirstOccurrenceOf("Features: ",
                                                                                                      false, false).getIntValue();
            
            m_sDetectionParameters.dDeltaThreshold      = trainingDataString[5].fromFirstOccurrenceOf("VelocityThresholdSetting: ",
                                                                                                      false, false).getDoubleValue();
            
            m_sDetectionParameters.iDecayBlockWindow    = trainingDataString[6].fromFirstOccurrenceOf("DecayTimeSetting: ",
                                                                                                      false, false).getIntValue();
            
            
            m_ppdTrainingData.clear();
            m_ppdTrainingData.assign(m_sTrainingParameters.iNumObservations, vector<double> (m_sTrainingParameters.iNumFeatures,0.0f));
            
            m_piTrainingClassLabels.clear();
            m_piTrainingClassLabels.assign(m_sTrainingParameters.iNumObservations, 0);
            
            
            for (int observation = 0; observation < m_sTrainingParameters.iNumObservations; observation++)
            {
                StringArray data;
                data.addTokens(trainingDataString[observation + 9].trim(), ";", "\"");
                
                m_piTrainingClassLabels[observation] = data[0].getIntValue();
                
                for (int feature=0; feature< m_sTrainingParameters.iNumFeatures; feature++)
                {
                    m_ppdTrainingData[observation][feature] = data[feature + 1].getDoubleValue();
                }
            }
            
            
            //--- Normalize Data and Train SVM ---//
            performTraining();
            
        }
        
    }
}


int OnsetClassification::getNumClasses()
{
    return m_sTrainingParameters.iNumClasses;
}


double OnsetClassification::getClassificationConfidence()
{
    return m_sTrainingParameters.dCrossValidationAccuracy;
}


String OnsetClassification::getCurrentFeatureVector()
{
    m_psFeatureVector.clear();
    
    for (int i=0; i < m_sTrainingParameters.iNumFeatures; i++)
    {
        m_psFeatureVector += String(m_pdFeatureVector[i]) + "\t\t";
    }
    
    return m_psFeatureVector;
}

int OnsetClassification::getCurrentObservation()
{
    return m_sTrainingParameters.iCurrentObservation;
}


//==============================================================================
// Are these required?
//==============================================================================

vector<float> OnsetClassification::getCurrentSpectrum()
{
    return m_pfClassificationRealFFT;
}


float OnsetClassification::getCurrentSpectralCentroid()
{
    return m_sAudioFeatures.dSpectralCentroid;
}

