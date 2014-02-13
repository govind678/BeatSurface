/*
  ==============================================================================

    AudioFeatureExtraction.h
    Created: 16 Nov 2013 6:21:54pm
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#ifndef AUDIOFEATUREEXTRACTION_H_INCLUDED
#define AUDIOFEATUREEXTRACTION_H_INCLUDED

#include <cmath>
#include "OnsetClassification.h"


class AudioFeatureExtraction 
{
    
public:
    
    AudioFeatureExtraction(int blockSize);
    ~AudioFeatureExtraction();
    
    float spectralFlux(float* previousRealFFT, float* currentRealFFT);
    
    
private:
    
    float mfSpectralSum;
    float mfSpectralDifference;
    
    float kfLowpassEnvelope;
    
    int miBlockSize;
    int miBinSize;
    
};




#endif  // AUDIOFEATUREEXTRACTION_H_INCLUDED
