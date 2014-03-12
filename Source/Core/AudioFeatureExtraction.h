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
    
    void setBufferSize(int bufferSize);
    
    
    float spectralFlux(float* previousRealFFT, float* currentRealFFT);
    float spectralCentroid(float* currentRealFFT);
    float rootMeanSquare(const float** inputBuffer, int iNumChannels);
    
    
private:
    
    void reset();
    
    //--- Spectral Flux ---//
    float m_fFlux_SpectralSum;
    float m_fFlux_SpectralDifference;
    
    //--- Spectral Centroid ---//
    float m_fCentroid_Numerator;
    float m_fCentroid_Denominator;
    float m_fCentroid_Square;
    
    //--- Root Mean Square ---//
    float m_fRMS_Sum;
    float m_fRMS_MixDown;
    
    int m_iBlockSize;
    int m_iBinSize;

    
};


#endif  // AUDIOFEATUREEXTRACTION_H_INCLUDED
