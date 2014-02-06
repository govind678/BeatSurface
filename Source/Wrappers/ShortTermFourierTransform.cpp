/*
  ==============================================================================

    ShortTermFourierTransform.cpp
    Created: 16 Nov 2013 5:53:53pm
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#include "ShortTermFourierTransform.h"


ShortTermFourierTransform::ShortTermFourierTransform(int blockSize)
{
    miBlockSize = blockSize;
    miBinSize = (blockSize/2) + 1;
    
    audioFFT = new audiofft::AudioFFT;
    audioFFT->init(miBlockSize);
    
    mpInputBuffer = new float[miBlockSize];
    
    for (int i=0; i<miBlockSize; i++) {
        mpInputBuffer[i] = 0;
    }
}


ShortTermFourierTransform::~ShortTermFourierTransform()
{
    delete [] mpInputBuffer;
    delete audioFFT;
}


void ShortTermFourierTransform::computeFFT(const float *input, float *realFFT, float *imgFFT)
{
    hannWindow(input, miBlockSize);
    audioFFT->fft(mpInputBuffer, realFFT, imgFFT);
}



void ShortTermFourierTransform::hannWindow(const float *input, int blockSize)
{
    for (int i=0; i<blockSize; i++) {
        mpInputBuffer[i] = input[i] * (0.5*(1-cos((2*M_PI*i)/(blockSize-1))));
    }
}


void ShortTermFourierTransform::computeIFFT(float *realFFT, float *imgFFT, float *output)
{
    audioFFT->ifft(output, realFFT, imgFFT);
}