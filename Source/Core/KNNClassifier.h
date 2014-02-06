/*
  ==============================================================================

    KNNClassifier.h
    Created: 4 Jan 2014 8:30:15pm
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#ifndef KNNCLASSIFIER_H_INCLUDED
#define KNNCLASSIFIER_H_INCLUDED


#include <math.h>

class KNNClassifier
{
    
public:
    
    KNNClassifier(float** featureSet, int* labels, int numObservations, int numFeatures, int K);
    ~KNNClassifier();
    
//    void loadTraining(float** featureSet, int* labels, int numObservations, int numFeatures, int K);
    
    int predict(float* testVector);
    
    int crossValidation(float** testDataset, int nPoint, int K);
    
private:
    
    float** mpTrainingSet;
    int* mpClassLabels;
    int* mpClasses;
    float* mpMinimumDistance;
    
    int miDistanceIndex;
    
    int miNumObservations;
    int miNumFeatures;
    int miK;
    
    float miDistance;
    float miSum;
    int   miSwapIndex;
    
    int miMaxIndex;
    float miMaxValue;
    
    void maxValueInArray(float* array, int length, int& index, float& value);
    int calculateMode(int* array, int length);
    
    
};


#endif  // KNNCLASSIFIER_H_INCLUDED
