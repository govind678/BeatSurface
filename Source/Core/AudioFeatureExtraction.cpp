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
    
    mfSpectralSum = 0;
    mfSpectralDifference = 0;
    
    kfLowpassEnvelope = 0.5;
    
}


AudioFeatureExtraction::~AudioFeatureExtraction()
{
    
}


float AudioFeatureExtraction::spectralFlux(float *previousRealFFT, float *currentRealFFT)
{
    mfSpectralSum = 0;
    
    for (int k = 0; k < miBinSize; k++) {
        
        mfSpectralDifference = fabsf(currentRealFFT[k]) - fabsf(previousRealFFT[k]);
        
        if (mfSpectralDifference > 0) {
            mfSpectralSum = mfSpectralSum + ((k * mfSpectralDifference) * (k * mfSpectralDifference));
        }
    }
    
    return sqrt(mfSpectralSum/miBinSize);
}
