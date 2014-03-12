/*
  ==============================================================================

    AudioFeatureExtraction.cpp
    Created: 16 Nov 2013 6:21:54pm
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#include "AudioFeatureExtraction.h"


AudioFeatureExtraction::AudioFeatureExtraction()
{
    m_iBlockSize    =   0;
    m_iBinSize      =   0;
    reset();
}


AudioFeatureExtraction::~AudioFeatureExtraction()
{
    
}



void AudioFeatureExtraction::setBufferSize(int bufferSize)
{
    m_iBlockSize    =   bufferSize;
    m_iBinSize      =   (m_iBlockSize / 2) + 1;
    reset();
}


float AudioFeatureExtraction::spectralFlux(float *previousRealFFT, float *currentRealFFT)
{
    m_fFlux_SpectralSum = 0.0f;
    
    for (int k = 0; k < m_iBinSize; k++) {
        
        m_fFlux_SpectralDifference = fabsf(currentRealFFT[k]) - fabsf(previousRealFFT[k]);
        
        if (m_fFlux_SpectralDifference > 0) {
            m_fFlux_SpectralSum = m_fFlux_SpectralSum + ((k * m_fFlux_SpectralDifference) * (k * m_fFlux_SpectralDifference));
        }
    }
    
    return sqrt(m_fFlux_SpectralSum/m_iBinSize);
}




float AudioFeatureExtraction::spectralCentroid(float *currentRealFFT)
{
    m_fCentroid_Numerator            = 0.0f;
    m_fCentroid_Denominator          = 0.0f;
    
    for (int k = 0; k < m_iBinSize; k++) {
        
        m_fCentroid_Square = currentRealFFT[k] * currentRealFFT[k];
        
        m_fCentroid_Numerator += k * m_fCentroid_Square;
        m_fCentroid_Denominator += m_fCentroid_Square;
    }
    
    if (m_fCentroid_Denominator == 0.0f) {
        return 0.0f;
    }
    
    else {
        return (m_fCentroid_Numerator / m_fCentroid_Denominator);
    }
}




float AudioFeatureExtraction::rootMeanSquare(const float **inputBuffer, int iNumChannels)
{
    m_fRMS_Sum = 0.0f;
    
    for (int sample=0; sample < m_iBlockSize; sample++)
    {
//        m_fRMS_MixDown = 0.0f;
        
//        for (int channel = 0; channel < iNumChannels; channel++) {
//            m_fRMS_MixDown += inputBuffer[sample][channel] / iNumChannels;
//        }
        m_fRMS_Sum += inputBuffer[0][sample] * inputBuffer[0][sample];
    }
    return(sqrtf(m_fRMS_Sum / m_iBlockSize));
}



void AudioFeatureExtraction::reset()
{
    m_fFlux_SpectralSum              = 0.0f;
    m_fFlux_SpectralDifference       = 0.0f;
    
    m_fCentroid_Numerator            = 0.0f;
    m_fCentroid_Denominator          = 0.0f;
    m_fCentroid_Square               = 0.0f;
    
    m_fRMS_MixDown                   = 0.0f;
    m_fRMS_Sum                       = 0.0f;
}

