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
    
    Error_t setTrainingDataAndTrain(vector<vector<float>> ppfTrainingData,
                                    vector<int> piTrainingClassLabels,
                                    int iNumFeatures,
                                    int iNumObservations);
    
    Error_t setParameters(const SvmParameter_t &stSvmParam);
    
    
    double* classify(float *pfFeatures, int iNumFeatures);
    double getResult (double *pdProbability = 0);
    vector<double> getProbability();
    
    int getNumSvElements( std::string strSvString );
    
    Error_t saveModelToDisk(std::string modelFilePath);
    Error_t loadModelFromDisk(std::string modelFilePath);
    
    
private:
    
    void setDefaultParameters();
    
    bool m_bIsInitialized;
    
    vector<double> m_pdProbabilityEstimate;
    
};





#endif  // SVMTRAIN_H_INCLUDED
