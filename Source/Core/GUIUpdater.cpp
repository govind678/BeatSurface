/*
  ==============================================================================

    GUIUpdater.cpp
    Created: 3 Mar 2014 3:17:00am
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#include "GUIUpdater.h"


GUIUpdater::GUIUpdater()    //:   Thread("GUI Updater")
{
    m_iCurrentTrainingOnsetClass = 0;
    m_iNumClasses = 0;
}


GUIUpdater::~GUIUpdater()
{
    onsetProbabilities = nullptr;
}


void GUIUpdater::doneTraining()
{
    DoneTraining = true;
}


void GUIUpdater::displayTrainingOnset(int index)
{
    m_iCurrentTrainingOnsetClass = index;
    DisplayTrainingOnset = true;
}

int GUIUpdater::getCurrentTrainingClass()
{
    return m_iCurrentTrainingOnsetClass;
}


void GUIUpdater::displayOnsetProbabilities(double *probabilities)
{
    for (int i=0; i < m_iNumClasses; i++) {
        onsetProbabilities[i] = float(probabilities[i]);
    }
    DisplayOnsetProbabilities = true;
}

float* GUIUpdater::getOnsetProbabilities()
{
    return onsetProbabilities;
}




void GUIUpdater::setNumClasses(int numClasses)
{
    onsetProbabilities = nullptr;
    
    m_iNumClasses = numClasses;
    
    onsetProbabilities = new float [m_iNumClasses];
}

