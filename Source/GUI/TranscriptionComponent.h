/*
  ==============================================================================

    TranscriptionComponent.h
    Created: 26 Mar 2014 7:20:23pm
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#ifndef TRANSCRIPTIONCOMPONENT_H_INCLUDED
#define TRANSCRIPTIONCOMPONENT_H_INCLUDED

#include "BeatSurfaceHeader.h"



//==============================================================================


class TranscriptionComponent    :   public Component,
                                    public TableListBoxModel
{
    
friend class MainComponent;
    
public:
    
    
    TranscriptionComponent();
    ~TranscriptionComponent();
    
    void resized() override;
    void paint(Graphics& g) override;
    
    void paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) override;
    
    void paintCell (Graphics& g, int rowNumber, int columnId,
                    int width, int height, bool /*rowIsSelected*/) override;
    
    Component* refreshComponentForCell (int rowNumber, int columnId, bool /*isRowSelected*/,
                                        Component* existingComponentToUpdate) override;
    
    void setNumRows(int newNumRows);
    int getNumRows() override;
    
    int getActualClass (const int rowNumber) const;
    void setActualClass (const int rowNumber, const int newClass);
    
    bool getToInclude(const int rowNumber) const;
    void setToInclude(const int rowNumber, const bool newToggle);
    
    void addRow(int index, String time, int predictedClass, String features);
    void setNumClasses(int numClasses);
    void clearRows();
    void setSystemMode(BeatSurfaceBase::SystemMode newMode);
    
private:
    
    ScopedPointer<TextButton>   retrainButton;
    
    TableListBox table;
    int          numRows;
    int          m_iNumClasses;
    
    Array<int>      m_piIndices;
    Array<bool>     m_pbIncludes;
    Array<int>      m_piPredictedClasses;
    Array<int>      m_piActualClasses;
    StringArray     m_psFeatures;
    StringArray     m_psTime;
    
    Colour          trainingColour;
    Colour          playingColour;

};


#endif  // TRANSCRIPTIONCOMPONENT_H_INCLUDED
