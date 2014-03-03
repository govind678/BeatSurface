/*
  ==============================================================================

    SVMTrain.h
    Created: 13 Feb 2014 3:16:11am
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#ifndef SVMTRAIN_H_INCLUDED
#define SVMTRAIN_H_INCLUDED


#include <iostream>
#include "SVMBase.h"


class SVMTrain : public SVMBase
{
    
public:
    
    SVMTrain();
    ~SVMTrain();
    
    Error_t setTrainingDataAndTrain(float **ppfFeatureData, float *pfLabels, int iNumFeatures, int iNumObservations);
    Error_t setParameters(const SvmParameter_t &stSvmParam);
    Error_t saveModelToDisk(std::string modelFilePath);
    svm_model*    getCurrentSVMModel();
    
    void clearModel();
    
    
private:
    
    void setDefaultParameters();
    
    bool m_bIsInitialized;
    
};





#endif  // SVMTRAIN_H_INCLUDED
