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
    m_iCurrentTrainingOnsetClass        =   0;
    m_iNumClasses                       =   0;
    
    m_fCurrentWaveformSampleToDraw      =   0.0f;
    m_fCurrentSpectralCentroidToDraw    =   0.0f;
    DisplayTrainingOnset                =   false;
    DoneTraining                        =   false;
    DisplayOnsetProbabilities           =   false;
    DrawWaveform                        =   false;
    UpdateMetronome                     =   false;
    
    
//    m_pfOnsetProbabilities.clear();
//    m_pfCurrentSpectrum.clear();
}


GUIUpdater::~GUIUpdater()
{
//    m_pfOnsetProbabilities.clear();
    m_pfOnsetProbabilities.clear();
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

void GUIUpdater::displayTrainingOnset(int index)
{
    m_iCurrentTrainingOnsetClass    = index;
    
    //--- Set Flag to update GUI ---//
    DisplayTrainingOnset            = true;
}

int GUIUpdater::getCurrentTrainingClass()
{
    return m_iCurrentTrainingOnsetClass;
}
//==============================================================================




//==============================================================================
// Flash Class Probabilities

void GUIUpdater::displayOnsetProbabilities(vector<float> probabilities)
{
    for (int i = 0; i < m_iNumClasses; i++)
    {
        if (probabilities.size() == 0)
        {
//            m_pfOnsetProbabilities.set(i, probabilities[i]);
//            m_pfOnsetProbabilities.add(probabilities[i]);
            m_pfOnsetProbabilities[i] = float(probabilities[i]);
        }
        
        else
        {
//            m_pfOnsetProbabilities.set(i, 1.0f);
            m_pfOnsetProbabilities[i] = 1.0f;
        }
    }
    
    //--- Set Flag to update GUI ---//
    DisplayOnsetProbabilities = true;
}


vector<float> GUIUpdater::getOnsetProbabilities()
{
    return m_pfOnsetProbabilities;
}
//==============================================================================






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




//==============================================================================
// Set Current Number of Classes

void GUIUpdater::setNumClasses(int numClasses)
{
    m_iNumClasses = numClasses;
    
    m_pfOnsetProbabilities.resize(m_iNumClasses);
}
//==============================================================================