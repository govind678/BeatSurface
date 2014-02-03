/*
  ==============================================================================

    KNNClassifier.cpp
    Created: 4 Jan 2014 8:30:15pm
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#include "KNNClassifier.h"

KNNClassifier::KNNClassifier(float **featureSet, int *labels, int numObservations, int numFeatures, int K)
{
    mpTrainingSet = featureSet;
    mpClassLabels = labels;
    
    miNumFeatures = numFeatures;
    miNumObservations = numObservations;
    miK = K;
    
    mpClasses = new int[K];
    mpMinimumDistance = new float[K];
    
    for (int i=0; i < K; i++) {
        mpMinimumDistance[i] = 100;
    }

}



KNNClassifier::~KNNClassifier()
{
    delete [] mpClassLabels;
    delete [] mpTrainingSet;
    delete [] mpClasses;
    delete [] mpMinimumDistance;
}






int KNNClassifier::predict(float *testVector)
{
    miDistanceIndex = 0;
    
    for (int observation = 0; observation < miNumObservations; observation++) {
        
        miSum = 0;
        for (int feature = 0 ; feature < miNumFeatures; feature++) {
            
            miSum = miSum + (mpTrainingSet[observation][feature] - testVector[feature]) * (mpTrainingSet[observation][feature] - testVector[feature]);
            
        }
        
        miDistance = sqrtf(miSum);
        
        maxValueInArray(mpMinimumDistance, miK, miMaxIndex, miMaxValue);
        
        if (miDistanceIndex < miK )
        {
            mpMinimumDistance[miDistanceIndex] = miDistance;
            mpClasses[miDistanceIndex] = mpClassLabels[observation];
            miDistanceIndex++;
        }
        
        else if (miDistance < miMaxValue)
        {
            miSwapIndex = miMaxIndex;
            mpMinimumDistance[miSwapIndex] = miDistance;
            mpClasses[miSwapIndex] = mpClassLabels[observation];
        }
    }
    
    
    
    return (calculateMode(mpClasses, miK));
    
}



int KNNClassifier::crossValidation(float **testDataset, int nPoint, int K)
{
 
    return 0;
}



void KNNClassifier::maxValueInArray(float *array, int length, int& index, float& value)
{
    index = 0;
    value = 0;
    
    for(int i=0 ; i< length; i++) {
        if (array[i] > value) {
            value=array[i];
            index = i;
        }
    }
    
}




int KNNClassifier::calculateMode(int *array, int length)
{
    
    int num = 0;
    int count = 0;
    int max = 0;
    int mode = 0;
    
    for (int i=0; i < length; i++) {
        
        if (num != array[i]) {
            num = array[i];
            count = 1;
        } else {
            count++;
        }
        
        if (count > max) {
            max = count;
            mode = num;
        }
    }
    
    return mode;
}



