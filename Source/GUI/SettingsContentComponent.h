/*
  ==============================================================================

    SettingsContentComponent.h
    Created: 14 Feb 2014 2:41:44am
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#ifndef SETTINGSCONTENTCOMPONENT_H_INCLUDED
#define SETTINGSCONTENTCOMPONENT_H_INCLUDED

#include "BeatSurfaceHeader.h"


class MainComponent;

//==============================================================================

class MidiAudioOutputComponent    :     public Component,
                                        public TableListBoxModel
{
    
public:
    
    
    MidiAudioOutputComponent();
    ~MidiAudioOutputComponent();
    
    void setMainComponent(MainComponent* component);
    MainComponent* getMainComponent();
    
    void resized() override;
    void paint(Graphics& g) override;
    
    void paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) override;
    
    void paintCell (Graphics& g, int rowNumber, int columnId,
                    int width, int height, bool /*rowIsSelected*/) override;
    
    Component* refreshComponentForCell (int rowNumber, int columnId, bool /*isRowSelected*/,
                                        Component* existingComponentToUpdate) override;
    
    void setNumRows(int newNumRows);
    int getNumRows() override;
    
    void setIncludeMidi (const int rowNumber, const bool newInclude);
    bool getIncludeMidi (const int rowNumber);
    
    int getMidiChannel (const int rowNumber) const;
    void setMidiChannel (const int rowNumber, const int newMidiChannel);
    
    int getMidiNote (const int rowNumber) const;
    void setMidiNote (const int rowNumber, const int newMidiNote);
    
    bool getLooping(const int rowNumber) const;
    void setLooping(const int rowNumber, const bool newToggle);
    
    bool getToggle(const int rowNumber) const;
    void setToggle(const int rowNumber, const bool newToggle);
    
    void setIncludeAudio (const int rowNumber, const bool newInclude);
    bool getIncludeAudio (const int rowNumber);
    
    int getMidiDuration (const int rowNumber) const;
    void setMidiDuration (const int rowNumber, const int newDuration);
    
    void setAudioFilename(const int rowNumber, String filename);
    String getAudioFilename(const int rowNumber);
    
    void addRow(int classIndex, bool includeMIDI, int midiChannel, int midiNote, int midiDuration, bool includeAudio, String fileName, bool looping, bool toggle);
    void deleteRow(int rowIndex);
    
    void clearRows();
    
    void updateTable();
    
    

private:
    
    void updateMidiData(int rowNumber);
    
    TableListBox    table;
    int             numRows;
    int             m_iNumClasses;
    
    Array<int>      m_piClasses;
    
    Array<bool>     m_pbIncludeMIDI;
    Array<int>      m_piMidiChannels;
    Array<int>      m_piMidiNotes;
    Array<int>      m_piMidiDuration;
    
    Array<bool>     m_pbIncludeAudio;
    StringArray     m_psAudioFilenames;
    Array<bool>     m_pbLooping;
    Array<bool>     m_pbToggle;
    
    MainComponent*  m_pcMainComponent;
};


//==============================================================================



class SettingsContentComponent  :   public Component
{
    
friend class MainComponent;

public:
    
    SettingsContentComponent ();
    ~SettingsContentComponent();
    
    
    void setMainComponent(MainComponent* component);
    MainComponent* getMainComponent();
    
    
    void paint (Graphics& g);
    void resized();
    
    void addClass();
    void deleteClass(int classIndex);
    
    void setAudioFileName(int classIndex, String fileName);
    
    
private:

    
    ScopedPointer<Slider>           velocitySensitivitySlider;
    ScopedPointer<Slider>           decayTimeSensitivitySlider;
    ScopedPointer<Label>            velocitySensitivityLabel;
    ScopedPointer<Label>            decayTimeSensitivityLabel;
    ScopedPointer<TextButton>       audioSetupButton;
    
    ScopedPointer<Label>            tempoNumBox;
    ScopedPointer<Label>            numeratorNumBox;
    ScopedPointer<Label>            denominatorNumBox;
    
    ScopedPointer<Label>            tempoLabel;
    ScopedPointer<Label>            meterLabel;
    
    ScopedPointer<Label>            trainingTimeinBarsLabel;
    ScopedPointer<Slider>           trainingTimeinBarsSlider;
    
    ScopedPointer<MidiAudioOutputComponent> midiAudioComponent;
    
    
    int                             m_iNumClasses;
    
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SettingsContentComponent)
};


#endif  // SETTINGSCONTENTCOMPONENT_H_INCLUDED
