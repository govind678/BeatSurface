/*
  ==============================================================================

    ShortTermFourierTransform.cpp
    Created: 16 Nov 2013 5:53:53pm
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#include "ShortTermFourierTransform.h"
#include <stdio.h>
#include <iostream>

ShortTermFourierTransform::ShortTermFourierTransform(int bufferSize)
{
    m_iBlockSize    =   bufferSize;
    m_iBinSize      =   (m_iBlockSize/2) + 1;
    
    audioFFT        = new audiofft::AudioFFT;
    audioFFT->init(m_iBlockSize);
    
    m_pfInputBuffer.assign(m_iBlockSize, 0.0f);
}




ShortTermFourierTransform::~ShortTermFourierTransform()
{
    m_pfInputBuffer.clear();
    delete audioFFT;
}


void ShortTermFourierTransform::computeFFT(const float *input, float *realFFT, float *imgFFT)
{
    hannWindow(input, m_iBlockSize);
    audioFFT->fft(m_pfInputBuffer.data(), realFFT, imgFFT);
}



void ShortTermFourierTransform::hannWindow(const float *input, int blockSize)
{
    for (int i=0; i<blockSize; i++) {
        m_pfInputBuffer.at(i) = input[i] * (0.5*(1-cos((2*M_PI*i)/(blockSize-1))));
    }
}


void ShortTermFourierTransform::computeIFFT(float *realFFT, float *imgFFT, float *output)
{
    audioFFT->ifft(output, realFFT, imgFFT);
}