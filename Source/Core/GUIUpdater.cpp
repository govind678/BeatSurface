/*
  ==============================================================================

    GUIUpdater.cpp
    Created: 3 Mar 2014 3:17:00am
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#include "GUIUpdater.h"


//==============================================================================
// Constructor and Destructor

GUIUpdater::GUIUpdater()
{
    //--- Initialize ---//
    DisplayTrainingOnset                =   false;
    DoneTraining                        =   false;
    DrawWaveform                        =   false;
    UpdateMetronome                     =   false;
    DisplayPlayingOnset                 =   false;
    LoadTrainingUpdate                  =   false;
}


GUIUpdater::~GUIUpdater()
{
    
}
//==============================================================================




//==============================================================================
// Reset button alphas to indicate end of training

void GUIUpdater::doneTraining()
{
    DoneTraining = true;
}
//==============================================================================




//==============================================================================
// Flash current training onset

void GUIUpdater::displayTrainingOnset()
{
    DisplayTrainingOnset    = true;
}
//==============================================================================





//==============================================================================
// Flash Playing Onset

void GUIUpdater::displayPlayingOnset()
{
    DisplayPlayingOnset =   true;
}




//==============================================================================
// Draw Spectrogram

void GUIUpdater::drawSpectrum()
{
    DrawSpectrum    =   true;
}
//==============================================================================





//==============================================================================
// Draw Waveforms

void GUIUpdater::drawWaveformSample()
{
    //--- Set Flag to update GUI ---//
    DrawWaveform = true;
}

//==============================================================================




void GUIUpdater::updateGUIOnLoadTraining()
{
    LoadTrainingUpdate = true;
}
