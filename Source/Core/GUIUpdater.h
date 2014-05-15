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
    
    void displayTrainingOnset();
    //==============================================================================
    

    
    //==============================================================================
    // Display Classification Result  during Play Mode
    
    void displayPlayingOnset();
    //==============================================================================
    
    
    
    //==============================================================================
    // Draw Waveform
    void drawWaveformSample();
    //==============================================================================
    
    
    
    //==============================================================================
    // Draw Spectrum
    void drawSpectrum();
    //==============================================================================
    
    
    
    
    //==============================================================================
    // Update GUI on loading training
    void updateGUIOnLoadTraining();
    
    
    //==============================================================================
    // Flags to Update GUI
    bool DisplayTrainingOnset;
    bool DoneTraining;
    bool DisplayPlayingOnset;
    bool DrawWaveform;
    bool UpdateMetronome;
    bool DrawSpectrum;
    bool LoadTrainingUpdate;
    //==============================================================================

    
private:
    
    int             m_iNumClasses;
 
};


#endif  // GUIUPDATER_H_INCLUDED
