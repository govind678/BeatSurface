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
    m_iLowBinLimit                   =   1;
    m_iHighBinLimit                  =   80;
    
    m_dFlux_SpectralSum              = 0.0f;
    m_dFlux_SpectralDifference       = 0.0f;
    
    m_dCentroid_Numerator            = 0.0f;
    m_dCentroid_Denominator          = 0.0f;
    m_dCentroid_Square               = 0.0f;
    
    m_dFlatness_Numerator            = 0.0f;
    m_dFlatness_Denominator          = 0.0f;
    
    m_dSlope_WeightedSum             = 0.0f;
    m_dSlope_UnweightedSum           = 0.0f;
    m_dSlope_BinSum                  = 0.0f;
    m_dSlope_BinSquareSum            = 0.0f;
    m_dSlope_Delta                   = 8.0f;
    
    m_dRMS_Sum                       = 0.0f;
    
    m_iBinSize                       = 0;
}


AudioFeatureExtraction::~AudioFeatureExtraction()
{
    
}



double AudioFeatureExtraction::spectralFlux(float *previousRealFFT, float *currentRealFFT, int blockSize)
{
    m_iBinSize = int(blockSize/2) + 1;
    
    m_dFlux_SpectralSum = 0.0f;
    
    for (int k = 0; k < m_iBinSize; k++)
    {
        m_dFlux_SpectralDifference = fabs(currentRealFFT[k]) - fabs(previousRealFFT[k]);
        
        if (m_dFlux_SpectralDifference > 0)
        {
            m_dFlux_SpectralSum += (k * m_dFlux_SpectralDifference) * (k * m_dFlux_SpectralDifference);
        }
    }
    
    return sqrt(m_dFlux_SpectralSum/m_iBinSize);
}




void AudioFeatureExtraction::extractFeatures(float *currentRealFFT, double *features, int blockSize)
{
    features[0] = spectralCentroid(currentRealFFT, blockSize);
    features[1] = spectralFlatness(currentRealFFT, blockSize);
    features[2] = spectralSlope(currentRealFFT, blockSize);
}



double AudioFeatureExtraction::spectralCentroid(float *currentRealFFT, int blockSize)
{
    m_iBinSize = int(blockSize/2) + 1;
    
    m_dCentroid_Numerator            = 0.0f;
    m_dCentroid_Denominator          = 0.0f;
    
    for (int k = 1; k < m_iBinSize; k++) {
        
        m_dCentroid_Square = fabs(currentRealFFT[k]) * fabs(currentRealFFT[k]);
        
        m_dCentroid_Numerator += log2f(k) * m_dCentroid_Square;
        m_dCentroid_Denominator += m_dCentroid_Square;
    }
    
    if (m_dCentroid_Denominator == 0.0f)
    {
        return 0.0f;
    }
    
    else
    {
        return (10.0f * (m_dCentroid_Numerator / m_dCentroid_Denominator));
    }
}




double AudioFeatureExtraction::rootMeanSquareIndB(const float **inputBuffer, int blockSize)
{
    m_dRMS_Sum = 0.0f;
    
    for (int sample=0; sample < blockSize; sample++)
    {
        m_dRMS_Sum += inputBuffer[0][sample] * inputBuffer[0][sample];
    }
    
    m_dRMS_dB = 20 * log10( (sqrt(m_dRMS_Sum / blockSize)) / REF_VOL_DB);
    
    return m_dRMS_dB;
}



double AudioFeatureExtraction::spectralFlatness(float *currentRealFFT, int blockSize)
{
    m_dFlatness_Numerator       = 0.0f;
    m_dFlatness_Denominator     = 0.0f;
    
    for (int k = 0; k < m_iHighBinLimit; k++)
    {
        m_dFlatness_Numerator   += log(fabs(currentRealFFT[k]));
        m_dFlatness_Denominator += fabs(currentRealFFT[k]);
    }

    if (m_dFlatness_Denominator == 0.0f)
    {
        return 0.0f;
    }
    
    else
    {
        return (100.0f * (expf(m_dFlatness_Numerator / m_iHighBinLimit)) / (m_dFlatness_Denominator / m_iHighBinLimit));
    }
}




double AudioFeatureExtraction::spectralSlope(float *currentRealFFT, int blockSize)
{
    m_iBinSize = int(blockSize/2) + 1;
    
    m_dSlope_WeightedSum             = 0.0f;
    m_dSlope_UnweightedSum           = 0.0f;
    m_dSlope_BinSum                  = 0.0f;
    m_dSlope_BinSquareSum            = 0.0f;
    
    for (int k=1; k < m_iBinSize; k++)
    {
        m_dSlope_WeightedSum    += (log2(k) * (fabs(currentRealFFT[k]) * fabs(currentRealFFT[k])));
        m_dSlope_UnweightedSum  += (fabs(currentRealFFT[k]) * fabs(currentRealFFT[k]));
        m_dSlope_BinSum         += log2(k);
        m_dSlope_BinSquareSum   += log2(k) * log2(k);
    }
    
    return (10.0f * (m_dSlope_Delta + ((blockSize * m_dSlope_WeightedSum) - (m_dSlope_BinSum * m_dSlope_UnweightedSum))
            / ((blockSize * m_dSlope_BinSquareSum) - (m_dSlope_BinSum * m_dSlope_BinSum))));
}

