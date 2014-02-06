/*
  ==============================================================================

    AudioFileIO.h
    Created: 18 Nov 2013 2:56:30am
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#ifndef AUDIOFILEIO_H_INCLUDED
#define AUDIOFILEIO_H_INCLUDED

#include "JuceHeader.h"
#include "OnsetClassification.h"


class AudioFileIO
{
    
public:
    
    AudioFileIO (File audioFile, int blockSize);
    ~AudioFileIO();
    
    void runDetectionAndClassification();
    
    OnsetClassification* onsetClassifier;
    
    
private:
    
    int miBlockSize;
    float mfSampleRate;
    
    const float** inputBuffer;
    
    int miNumBlocks;
    bool* mpOnsets;
    
    ScopedPointer<AudioFormatReader> mpReader;
};



#endif  // AUDIOFILEIO_H_INCLUDED
