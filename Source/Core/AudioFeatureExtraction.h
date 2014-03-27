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
    
    AudioFeatureExtraction();
    ~AudioFeatureExtraction();
    
    double spectralFlux(float* previousRealFFT, float* currentRealFFT, int blockSize);
    double spectralCentroid(float* currentRealFFT, int blockSize);
    double spectralFlatness(float* currentRealFFT, int blockSize);
    double spectralSlope(float* currentRealFFT, int blockSize);
    
    void extractFeatures(float* currentRealFFT, double* features, int blockSize);
    
    double rootMeanSquare(const float** inputBuffer, int iNumChannels, int blockSize);

    
private:
    
    //--- Spectral Flux ---//
    double m_dFlux_SpectralSum;
    double m_dFlux_SpectralDifference;
    
    //--- Spectral Centroid ---//
    double m_dCentroid_Numerator;
    double m_dCentroid_Denominator;
    double m_dCentroid_Square;
    
    //--- Spectral Flatness ---//
    double m_dFlatness_Numerator;
    double m_dFlatness_Denominator;
    
    //--- Spectral Slope ---//
    double m_dSlope_WeightedSum;
    double m_dSlope_UnweightedSum;
    double m_dSlope_BinSum;
    double m_dSlope_BinSquareSum;
    double m_dSlope_Delta;
    
    //--- Root Mean Square ---//
    double m_dRMS_Sum;
    double m_dRMS_MixDown;
    
    
    int m_iLowBinLimit;
    int m_iHighBinLimit;
    int m_iBinSize;

    
};


#endif  // AUDIOFEATUREEXTRACTION_H_INCLUDED
