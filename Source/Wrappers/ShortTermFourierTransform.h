/*
  ==============================================================================

    ShortTermFourierTransform.h
    Created: 16 Nov 2013 5:53:53pm
    Author:  Govinda Ram Pingali

 
    Wrapper Class for AudioFFT by HiFi-LoFi : https://github.com/HiFi-LoFi/AudioFFT
  ==============================================================================
*/

#ifndef SHORTTERMFOURIERTRANSFORM_H_INCLUDED
#define SHORTTERMFOURIERTRANSFORM_H_INCLUDED


#include <cmath>
#include "AudioFFT.h"


class ShortTermFourierTransform
{
    
public:
    
    ShortTermFourierTransform(int blockSize);
    ~ShortTermFourierTransform();
    
    void computeFFT(const float* input, float* realFFT, float* imgFFT);
    void computeIFFT(float* realFFT, float* imgFFT, float* output);
    
    
private:
    
    audiofft::AudioFFT* audioFFT;
    
    void hannWindow(const float* input, int blockSize);
    
    int miBinSize;
    int miBlockSize;
    
    float* mpInputBuffer;
    
};




#endif  // SHORTTERMFOURIERTRANSFORM_H_INCLUDED
