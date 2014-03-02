/*
  ==============================================================================

    OnsetClassification.cpp
    Created: 16 Nov 2013 5:44:30pm
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#include "OnsetClassification.h"


OnsetClassification::OnsetClassification(int blockSize, int numChannels, float sampleRate)
{
    mfSampleRate = sampleRate;
    miBlockSize = blockSize;
    miNumChannels = numChannels;
    miBinSize = miBlockSize/2 + 1;
    

    
    //--- Allocating Memory ---//

    mpCurrentRealFFT    = new float[miBlockSize];
    mpCurrentImgFFT     = new float[miBinSize];
    mpPreviousRealFFT   = new float[miBinSize];
    
    stft = new ShortTermFourierTransform(miBlockSize);
    audioFeature = new AudioFeatureExtraction(miBlockSize);
    
    
    for (int bin = 0; bin < miBinSize; bin++) {
        mpCurrentRealFFT[bin]   = 0;
        mpCurrentImgFFT[bin]    = 0;
        mpPreviousRealFFT[bin]  = 0;
    }
    
    
//    mpTrainingMatrix = nullptr;
//    mpClassLabels    = nullptr;
//    knnClassifier    = nullptr;
    
    
    //--- Initializing Variables ---//
    
    miFrequencyBinHighLimit = 19;
    miFrequencyBinLowLimit = 1;
    miNumFeatures = miFrequencyBinHighLimit - miFrequencyBinLowLimit + 1;
    
    mfNoveltyFunction = 0;
    mfAdaptiveThreshold = 0;
    mfCurrentMaxDetectionValue = 0.3;
    miDecayBlockWindow = 2;
    mfDeltaThreshold = 0.15;
    
    mbDecayPeriod = false;
    miDecayBlockCounter = 0;
    
    
    
//    spTrainingFile1.open(sTrainingPath.append("Class1.txt"));
//    spTrainingFile2.open(sTrainingPath.append("Class2.txt"));
//    spTrainingFile3.open(sTrainingPath.append("Class3.txt"));
//    
//    spOutputFile.open(sOutputFilePath.append("Output.txt"));
    
    
//    mpTrainingMatrix1 = new float* [MAX_ONSETS_PER_TRAINING];
//    mpTrainingMatrix2 = new float* [MAX_ONSETS_PER_TRAINING];
//    mpTrainingMatrix3 = new float* [MAX_ONSETS_PER_TRAINING];
//    
//    mpTestVector = new float [miNumFeatures];
//    
//    for (int i=0; i < MAX_ONSETS_PER_TRAINING; i++) {
//        mpTrainingMatrix1[i] = new float [miNumFeatures];
//        mpTrainingMatrix2[i] = new float [miNumFeatures];
//        mpTrainingMatrix3[i] = new float [miNumFeatures];
//    }
//    
//
//    
//    miNumOnsets1 = 0;
//    miNumOnsets2 = 0;
//    miNumOnsets3 = 0;
    
    
//    miK = 1;
}



OnsetClassification::~OnsetClassification()
{
//    spTrainingFile1.close();
//    spTrainingFile2.close();
//    spTrainingFile3.close();
//    spOutputFile.close();
    
    
    delete [] mpCurrentRealFFT;
    delete [] mpCurrentImgFFT;
    delete [] mpPreviousRealFFT;
    

//    delete [] mpTrainingMatrix1;
//    delete [] mpTrainingMatrix2;
//    delete [] mpTrainingMatrix3;
//
//    delete [] mpTrainingMatrix;
//    delete [] mpClassLabels;
//
//    delete [] mpTestVector;
//
//    delete knnClassifier;
    
    
    delete stft;
//    delete audioFeature;

}



int OnsetClassification::process(const float **inputBuffer)
{
    
    // Write Input
//    for (int i=0; i < miBlockSize; i++) {
////        spInputTxtFile << inputBuffer[0][i] << std::endl;
//    }
    
//    int index = 0;
//    if (detectOnset(inputBuffer)) {
//        
//        for (int bin = miLowLimit; bin < miHighLimit; bin++, index++) {
//            mpTestVector[index] = mpCurrentRealFFT[bin];
//        }
//
//        return 1;
//        return (knnClassifier->predict(mpTestVector));
//    }
   
    return 0;
    
}




void OnsetClassification::train(const float** input, int classLabel)
{
//    if (detectOnset(input)) {
//        
//        if (classLabel == 1) {
//            
//            for (int bin = 0; bin < miNumFeatures; bin++) {
////                spTrainingFile1 << mpCurrentRealFFT[bin] << "\t";
//                mpTrainingMatrix1[miNumOnsets1][bin] = mpCurrentRealFFT[bin+miLowLimit];
//            }
//            miNumOnsets1++;
////            spTrainingFile1 << std::endl;
//            
//        } else if (classLabel == 2) {
//            
//            for (int bin = miLowLimit; bin <= miHighLimit; bin++) {
////                spTrainingFile2 << mpCurrentRealFFT[bin] << "\t";
//                mpTrainingMatrix1[miNumOnsets2][bin] = mpCurrentRealFFT[bin+miLowLimit];
//            }
////            spTrainingFile2 << std::endl;
//            miNumOnsets2++;
//            
//        } else if (classLabel == 3) {
//            
//            for (int bin = miLowLimit; bin <= miHighLimit; bin++) {
////                spTrainingFile3 << mpCurrentRealFFT[bin] << "\t";
//                mpTrainingMatrix3[miNumOnsets2][bin] = mpCurrentRealFFT[bin+miLowLimit];
//            }
////            spTrainingFile3 << std::endl;
//            miNumOnsets3++;
//        }
//    
//    }
    
}



//-- Return True if Audio Block Consists of an Onset --//

bool OnsetClassification::detectOnset(const float** input)
{
    
    //--- Compute STFT on 1st channel ---//
    stft->computeFFT(input[0], mpCurrentRealFFT, mpCurrentImgFFT);
    
    
    
    //--- Compute Spectral Flux ---//
    mfNoveltyFunction = audioFeature->spectralFlux(mpPreviousRealFFT, mpCurrentRealFFT);
    
    
    
    //--- Create Threshold ---//
    mfAdaptiveThreshold = mfDeltaThreshold + (mfNoveltyFunction + mfAdaptiveThreshold) / 2;
    
    
    
    //--- Store Current FFT ---//
    for (int bin = 0; bin < miBinSize; bin++) {
        mpPreviousRealFFT[bin] = mpCurrentRealFFT[bin];
    }
    
    
    
    //--- Check for Peaks ---//
    if (mfNoveltyFunction > mfAdaptiveThreshold)
    {
        if (miDecayBlockCounter == 0) {
            mbDecayPeriod = true;
            miDecayBlockCounter++;
            return true;
        }
    }
    
    
    //--- If Not Decay, Reset Counter ---//
    if (mbDecayPeriod) {
        if (miDecayBlockCounter == miDecayBlockWindow) {
            mbDecayPeriod = false;
            miDecayBlockCounter = 0;
        } else {
            miDecayBlockCounter++;
        }
    }
    
    
    return false;
}




void OnsetClassification::doneTraining()
{
//    int iTotalOnsets = miNumOnsets1 + miNumOnsets2 + miNumOnsets3;
//    mpTrainingMatrix = new float*[iTotalOnsets];
//    mpClassLabels   = new int [iTotalOnsets];
//    
//    for (int sample = 0; sample < miNumOnsets1; sample++) {
//        mpTrainingMatrix[sample] = mpTrainingMatrix1[sample];
//        mpClassLabels[sample] = 1;
//    }
//    
//    int index = 0;
//    for (int sample = miNumOnsets1; sample < miNumOnsets2; sample++, index++) {
//        mpTrainingMatrix[sample] = mpTrainingMatrix2[index];
//        mpClassLabels[sample] = 2;
//    }
//    
//    index = 0;
//    for (int sample = miNumOnsets2; sample < miNumOnsets3; sample++, index++) {
//        mpTrainingMatrix[sample] = mpTrainingMatrix3[index];
//        mpClassLabels[sample] = 3;
//    }
//    
//    
//    knnClassifier = new KNNClassifier(mpTrainingMatrix, mpClassLabels, iTotalOnsets, miNumFeatures, miK);
//    
//    
}




void OnsetClassification::setVelocitySensitivity(float sensitivity)
{
    mfDeltaThreshold = sensitivity * MAX_DELTA_THRESHOLD;
}


float OnsetClassification::getVelocitySensitivity()
{
    return (mfDeltaThreshold / MAX_DELTA_THRESHOLD);
}


void OnsetClassification::setDecayTimeSensitivity(float sensitivity)
{
    miDecayBlockWindow = (int) (sensitivity * MAX_DECAY_WINDOW_BLOCKS);
}


float OnsetClassification::getDecayTimeSensitivity()
{
    return (miDecayBlockWindow / MAX_DECAY_WINDOW_BLOCKS);
}
