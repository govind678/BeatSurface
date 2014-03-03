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


class AudioFeatureExtraction
{
public:
    
    AudioFeatureExtraction(int blockSize);
    ~AudioFeatureExtraction();
    
    
    float spectralFlux(float* previousRealFFT, float* currentRealFFT);
    float spectralCentroid(float* currentRealFFT);
    
    
private:
    
    float mfFlux_SpectralSum;
    float mfFlux_SpectralDifference;
    
    float mfCentroid_Numerator;
    float mfCentroid_Denominator;
    float mfCentroid_Square;
    
    int miBlockSize;
    int miBinSize;

    
};


#endif  // AUDIOFEATUREEXTRACTION_H_INCLUDED
