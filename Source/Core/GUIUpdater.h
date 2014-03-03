/*
  ==============================================================================

    GUIUpdater.h
    Created: 3 Mar 2014 3:17:00am
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#ifndef GUIUPDATER_H_INCLUDED
#define GUIUPDATER_H_INCLUDED

#include "JuceHeader.h"


class GUIUpdater    //:   public Thread
{
    
public:
    
    GUIUpdater();
    ~GUIUpdater();
    
    void doneTraining();
    void displayTrainingOnset(int index);
    void displayOnsetProbabilities(double* probabilities);
    
    int getCurrentTrainingClass();
    float* getOnsetProbabilities();
    
    void setNumClasses(int numClasses);
    
    
    //--- Flags ---//
    bool DisplayTrainingOnset       =   false;
    bool DoneTraining               =   false;
    bool DisplayOnsetProbabilities  =   false;
    
private:
    
    int m_iCurrentTrainingOnsetClass;
    
    int m_iNumClasses;

    ScopedPointer<float> onsetProbabilities;
    
};


#endif  // GUIUPDATER_H_INCLUDED
