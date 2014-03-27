/*
  ==============================================================================

    SVMTrain.h
    Created: 13 Feb 2014 3:16:11am
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#ifndef SVMTRAIN_H_INCLUDED
#define SVMTRAIN_H_INCLUDED


#include <vector>
#include <iostream>
#include <iterator>
#include "SVMBase.h"

using std::vector;

class SVMTrain : public SVMBase
{
    
public:
    
    SVMTrain();
    ~SVMTrain();
    
    Error_t setTrainingDataAndTrain(vector<vector<double>> ppdTrainingData,
                                    vector<int> piTrainingClassLabels,
                                    int iNumFeatures,
                                    int iNumObservations);
    
    Error_t setParameters(const SvmParameter_t &stSvmParam);
    
    
    double classify(double* pfFeatures, double* pdProbability = 0);
    
    int getNumSvElements( std::string strSvString );
    
    Error_t saveModelToDisk(std::string modelFilePath);
    Error_t loadModelFromDisk(std::string modelFilePath);
    
    
    Error_t crossValidationOnTrainingDataset(int numFolds, double& accuracy);
    Error_t evaluationOnTestDataset(double** ppfTestDataset, double* pfTargetLabels, int iNumObservations, int iNumFeatures);
    Error_t setNumOfFolds (int iNumFolds = 10);
    
    
private:
    
    void setDefaultParameters();
    
    bool m_bIsInitialized;
    
    vector<double> m_pdProbabilityEstimate;
    
    int     m_iNumFeatures;
    
    svm_node    *m_pSvmFeatures;
    vector<svm_node>    m_pcSvmFeatures;
    
};





#endif  // SVMTRAIN_H_INCLUDED
