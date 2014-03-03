/*
  ==============================================================================

    AudioFileIO.cpp
    Created: 18 Nov 2013 2:56:30am
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#include "AudioFileIO.h"


AudioFileIO::AudioFileIO(File audioFile, int blockSize)
{
    miBlockSize = blockSize;
    
    AudioFormatManager mcFormatManager;
    mcFormatManager.registerBasicFormats();
    mpReader = mcFormatManager.createReaderFor(audioFile);
    
    
    if (mpReader != 0)
    {
        //--- Allocating Memory Based on File ---//
        
        //onsetClassifier = new OnsetClassification(miBlockSize, mpReader->numChannels, mpReader->sampleRate);
        
        inputBuffer = new const float*[mpReader->numChannels];
        
        for (int channel = 0; channel < mpReader->numChannels; channel++) {
            inputBuffer[channel] = new float[miBlockSize];
        }
        
        miNumBlocks = (int) (mpReader->lengthInSamples - (mpReader->lengthInSamples % miBlockSize)) / miBlockSize;
        
        mpOnsets = new bool [miNumBlocks];
        
        for (int i=0; i < miNumBlocks; i++) {
            mpOnsets[i] = false;
        }

    }
}


AudioFileIO::~AudioFileIO()
{
    delete [] inputBuffer;
    delete [] mpOnsets;
    //delete onsetClassifier;
}



void AudioFileIO::runDetectionAndClassification()
{
//    AudioFormatManager mcFormatManager;
//    mcFormatManager.registerBasicFormats();
//    ScopedPointer<AudioFormatReader> mpReader = mcFormatManager.createReaderFor(audioFile);
//    
//    
//    if (mpReader != 0)
//    {
//        AudioSampleBuffer buffer(mpReader->numChannels, mpReader->lengthInSamples);
//        mpReader->read(&buffer, 0, mpReader->lengthInSamples, 0, true, true);
//        
//        
//        
//        //--- Allocating Memory Based on File ---//
//        
//        onsetClassifier = new OnsetClassification(miBlockSize, mpReader->numChannels, mpReader->sampleRate);
//        
//        inputBuffer = new const float*[mpReader->numChannels];
//        
//        for (int channel = 0; channel < mpReader->numChannels; channel++) {
//            inputBuffer[channel] = new float[miBlockSize];
//        }
//        
//        miNumBlocks = (int) (mpReader->lengthInSamples - (mpReader->lengthInSamples % miBlockSize)) / miBlockSize;
//
//        mpOnsets = new bool [miNumBlocks];
//        
//        for (int i=0; i < miNumBlocks; i++) {
//            mpOnsets[i] = false;
//        }
    
    
    if (mpReader != 0) {
        
    
        AudioSampleBuffer buffer(mpReader->numChannels, mpReader->lengthInSamples);
        mpReader->read(&buffer, 0, mpReader->lengthInSamples, 0, true, true);
    
    
        //--- Run Block Wise Processing ---//
        
        int miCurrentBlock = 0;
        
        for (int64 sample = 0; sample < mpReader->lengthInSamples; sample += miBlockSize, miCurrentBlock++) {
            
            for (int channel = 0; channel < mpReader->numChannels; channel++) {
                inputBuffer[channel] = buffer.getSampleData(channel, sample);
            }
            
            //mpOnsets[miCurrentBlock] = onsetClassifier->process(inputBuffer);
            
        }
        
        
        
        
        //std::ofstream outputOnsetTxtFile;
        //outputOnsetTxtFile.open("/Users/govindarampingali/Documents/GaTech/Masters Project/MATLAB/outputOnsetsFile.txt");
        //for (int64 i=0; i < miNumBlocks; i++) {
        //    outputOnsetTxtFile << mpOnsets[i] << std::endl;
        //}
        //outputOnsetTxtFile.close();

        
    }
    
    
}