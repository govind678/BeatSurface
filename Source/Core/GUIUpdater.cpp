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
    m_fCurrentWaveformSampleToDraw      =   0.0f;
    m_fCurrentSpectralCentroidToDraw    =   0.0f;
    
    
    DisplayTrainingOnset                =   false;
    DoneTraining                        =   false;
    DrawWaveform                        =   false;
    UpdateMetronome                     =   false;
    DisplayPlayingOnset                 =   false;
    LoadTrainingUpdate                  =   false;

    m_pfCurrentSpectrum.clear();
}


GUIUpdater::~GUIUpdater()
{
    m_pfCurrentSpectrum.clear();
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

void GUIUpdater::drawSpectrum(vector<float> spectrumArray)
{
    m_pfCurrentSpectrum.resize(spectrumArray.size());
    
    for (int i=0; i < spectrumArray.size(); i++)
    {
        m_pfCurrentSpectrum[i] = spectrumArray[i];
    }
    
    DrawSpectrum    =   true;
}

vector<float> GUIUpdater::getArrayToDrawSpectrum()
{
    return m_pfCurrentSpectrum;
}

void GUIUpdater::drawSpectralCentroid(float spectralCentroid)
{
    m_fCurrentSpectralCentroidToDraw = spectralCentroid;
}

float GUIUpdater::getSpectralCentroidToDraw()
{
    return m_fCurrentSpectralCentroidToDraw;
}
//==============================================================================





//==============================================================================
// Draw Waveforms

void GUIUpdater::drawWaveformWithSample(float sample)
{
    m_fCurrentWaveformSampleToDraw = sample;
    
    //--- Set Flag to update GUI ---//
    DrawWaveform = true;
}

float GUIUpdater::getSampleToDrawWaveform()
{
    return m_fCurrentWaveformSampleToDraw;
}

//==============================================================================




void GUIUpdater::updateGUIOnLoadTraining()
{
    LoadTrainingUpdate = true;
}
