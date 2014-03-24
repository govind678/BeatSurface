/*
  ==============================================================================

    GUIUpdater.h
    Created: 3 Mar 2014 3:17:00am
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#ifndef GUIUPDATER_H_INCLUDED
#define GUIUPDATER_H_INCLUDED

#include <stdio.h>
#include <vector>

using std::vector;

class GUIUpdater
{
    
public:
    
    //==============================================================================
    // Constructor and Destructor
    GUIUpdater();
    ~GUIUpdater();
    //==============================================================================
    
    
    
    //==============================================================================
    // Indicate end of training
    void doneTraining();
    //==============================================================================
    
    
    
    //==============================================================================
    // Display Onsets during training
    void displayTrainingOnset(int index);
    int getCurrentTrainingClass();
    //==============================================================================
    

    
    //==============================================================================
    // Display Onset Class Probabilities during Play Mode
    void displayOnsetProbabilities(vector<double> probabilities);
    vector<float> getOnsetProbabilities();
    
    void displayPlayingOnset(int index);
    int getCurrentPlayingOnset();
    //==============================================================================
    
    
    
    //==============================================================================
    // Draw Waveform
    void drawWaveformWithSample(float sample);
    float getSampleToDrawWaveform();
    //==============================================================================
    
    
    
    //==============================================================================
    // Draw Spectrum
    void drawSpectrum(vector<float> spectrumArray);
    vector<float> getArrayToDrawSpectrum();
    
    void drawSpectralCentroid(float spectralCentroid);
    float getSpectralCentroidToDraw();
    //==============================================================================
    
    
    
    //==============================================================================
    // Set Current Number of Classes to Resize Probability Array
    void setNumClasses(int numClasses);
    //==============================================================================
    
    
    
    //==============================================================================
    // Flags to Update GUI
    bool DisplayTrainingOnset;
    bool DoneTraining;
    bool DisplayOnsetProbabilities;
    bool DisplayPlayingOnset;
    bool DrawWaveform;
    bool UpdateMetronome;
    bool DrawSpectrum;
    //==============================================================================

    
private:
    
    int             m_iCurrentTrainingOnsetClass;
    int             m_iCurrentPlayingOnsetClass;
    float           m_fCurrentWaveformSampleToDraw;
    float           m_fCurrentSpectralCentroidToDraw;
    int             m_iNumClasses;
    
//    HeapBlock<float>        m_pfOnsetProbabilities;
//    HeapBlock<float>        m_pfCurrentSpectrum;
    vector<float>   m_pfOnsetProbabilities;
    vector<float>   m_pfCurrentSpectrum;
 
};


#endif  // GUIUPDATER_H_INCLUDED
