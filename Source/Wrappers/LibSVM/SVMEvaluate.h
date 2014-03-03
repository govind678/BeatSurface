/*
  ==============================================================================

    SVMEvaluate.h
    Created: 13 Feb 2014 4:14:00pm
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#ifndef SVMEVALUATE_H_INCLUDED
#define SVMEVALUATE_H_INCLUDED


#include <iostream>
#include "SVMTrain.h"


class SVMEvaluate : public SVMTrain
{
    
public:
    
    SVMEvaluate();
    ~SVMEvaluate();
    
    Error_t crossValidationOnTrainingDataset();
    Error_t evaluationOnTestDataset(float** ppfTestDataset, float* pfTargetLabels, int iNumObservations, int iNumFeatures);
    Error_t setNumOfFolds (int iNumFolds = 10);
    
    
private:
    
    bool m_bIsInitialized;
    
};




#endif  // SVMEVALUATE_H_INCLUDED
