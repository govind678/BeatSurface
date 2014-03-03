/*
  ==============================================================================

    SVMClassify.h
    Created: 13 Feb 2014 3:16:19am
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#ifndef SVMCLASSIFY_H_INCLUDED
#define SVMCLASSIFY_H_INCLUDED


#include <iostream>
#include "SVMBase.h"


class SVMClassify   : public SVMBase
{
    
public:
    
    SVMClassify();
    ~SVMClassify();
    
    double* classify(float *pfFeatures, int iNumFeatures);
    double getResult (double *pdProbability = 0);
    double* getProbability();
    
    int getNumSvElements( std::string strSvString );
    
    Error_t loadModelFromDisk(std::string modelFilePath);
    void setCurrentSVMModel(svm_model* model);
    
private:
    
    bool    m_bIsInitialized;
    
    
};




#endif  // SVMCLASSIFY_H_INCLUDED
