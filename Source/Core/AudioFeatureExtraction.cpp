/*
  ==============================================================================

    AudioFeatureExtraction.cpp
    Created: 16 Nov 2013 6:21:54pm
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#include "AudioFeatureExtraction.h"


AudioFeatureExtraction::AudioFeatureExtraction(int blockSize)
{
    miBlockSize = blockSize;
    miBinSize = (miBlockSize/2) + 1;
    
    mfFlux_SpectralSum              = 0.0f;
    mfFlux_SpectralDifference       = 0.0f;
    
    mfCentroid_Numerator            = 0.0f;
    mfCentroid_Denominator          = 0.0f;
    mfCentroid_Square               = 0.0f;

    
}


AudioFeatureExtraction::~AudioFeatureExtraction()
{
    
}


float AudioFeatureExtraction::spectralFlux(float *previousRealFFT, float *currentRealFFT)
{
    mfFlux_SpectralSum = 0.0f;
    
    for (int k = 0; k < miBinSize; k++) {
        
        mfFlux_SpectralDifference = fabsf(currentRealFFT[k]) - fabsf(previousRealFFT[k]);
        
        if (mfFlux_SpectralDifference > 0) {
            mfFlux_SpectralSum = mfFlux_SpectralSum + ((k * mfFlux_SpectralDifference) * (k * mfFlux_SpectralDifference));
        }
    }
    
    return sqrt(mfFlux_SpectralSum/miBinSize);
}




float AudioFeatureExtraction::spectralCentroid(float *currentRealFFT)
{
    mfCentroid_Numerator            = 0.0f;
    mfCentroid_Denominator          = 0.0f;
    
    for (int k = 0; k < miBinSize; k++) {
        
        mfCentroid_Square = currentRealFFT[k] * currentRealFFT[k];
        
        mfCentroid_Numerator += k * mfCentroid_Square;
        mfCentroid_Denominator += mfCentroid_Square;
    }
    
    if (mfCentroid_Denominator == 0.0f) {
        return 0.0f;
    }
    
    else {
        return (mfCentroid_Numerator / mfCentroid_Denominator);
    }
}