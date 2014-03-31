/*
  ==============================================================================

    SettingsContentComponent.cpp
    Created: 14 Feb 2014 2:41:44am
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#include "SettingsContentComponent.h"
#include "MainComponent.h"


class MidiComboBoxCustomComponent    :    public Component,
                                          public ComboBoxListener
{
public:
    
    MidiComboBoxCustomComponent (MidiAudioOutputComponent& owner_, bool isChannel) : owner (owner_), m_IsChannel(isChannel)
    {
        // just put a combo box inside this component
        addAndMakeVisible (comboBox);
        
        // when the combo is changed, we'll get a callback.
        comboBox.addListener (this);
        comboBox.setWantsKeyboardFocus (false);
        
        comboBox.clear();
        
        if (m_IsChannel)
        {
            for (int i=1; i < 17; i++)
            {
                comboBox.addItem(String(i), i);
            }
        }
        
        else
        {
            for (int i=0; i < 128; i++)
            {
                comboBox.addItem(String(i), i+1);
            }
        }
        
    }
    
    void resized() override
    {
        comboBox.setBoundsInset (BorderSize<int> (2));
    }
    
    void paint(Graphics& g) override
    {
        g.setColour (Colours::black.withAlpha (0.2f));
        g.drawRect(getWidth() - 1, 0, 1, getHeight());
    }
    
    // Our demo code will call this when we may need to update our contents
    void setRowAndColumn (const int newRow, const int newColumn)
    {
        row = newRow;
        columnId = newColumn;
        
        if (m_IsChannel)
        {
            comboBox.setSelectedId (owner.getMidiChannel (row), dontSendNotification);
        }
        
        else
        {
            comboBox.setSelectedId (owner.getMidiNote (row), dontSendNotification);
        }
        
    }
    
    void comboBoxChanged (ComboBox* /*comboBoxThatHasChanged*/) override
    {
        if (m_IsChannel)
        {
            owner.setMidiChannel (row, comboBox.getSelectedId());
        }
        
        else
        {
            owner.setMidiNote (row, comboBox.getSelectedId());
        }
        
    }
    
    
    
private:
    
    MidiAudioOutputComponent& owner;
    ComboBox comboBox;
    int row, columnId;
    bool m_IsChannel;     // 0: Note, 1: Channel
    
};



//==============================================================================



class ToggleButtonColumnCustomComponent    :        public Component,
                                                    public ButtonListener
{
public:
    
    ToggleButtonColumnCustomComponent (MidiAudioOutputComponent& owner_, bool isLooping) : owner (owner_)
    {
        m_bIsLooping = isLooping;
        
        addAndMakeVisible (toggleButton);
        
        toggleButton.addListener (this);
    }
    
    void resized() override
    {
        toggleButton.setBoundsInset (BorderSize<int> (2));
    }
    
    void paint(Graphics& g) override
    {
        g.setColour (Colours::black.withAlpha (0.2f));
        g.drawRect(getWidth() - 1, 0, 1, getHeight());
    }
    
    // Our demo code will call this when we may need to update our contents
    void setRowAndColumn (const int newRow, const int newColumn)
    {
        row = newRow;
        columnId = newColumn;
        
        if (m_bIsLooping)
        {
            toggleButton.setToggleState(owner.getLooping(row), dontSendNotification);
        }
        
        else
        {
            toggleButton.setToggleState(owner.getToggle(row), dontSendNotification);
        }
    }
    
    void buttonClicked (Button* buttonThatWasClicked) override
    {
        if (m_bIsLooping)
        {
            owner.setLooping (row, toggleButton.getToggleState());
        }
        
        else
        {
            owner.setToggle (row, toggleButton.getToggleState());
        }
        
    }
    
private:
    
    MidiAudioOutputComponent& owner;
    ToggleButton    toggleButton;
    int row, columnId;
    bool m_bIsLooping;
};



//==============================================================================



class TextButtonColumnCustomComponent    :          public Component,
                                                    public ButtonListener
{
public:
    
    TextButtonColumnCustomComponent (MidiAudioOutputComponent& owner_) : owner (owner_)
    {
        addAndMakeVisible (textButton);
        textButton.setClickingTogglesState(false);
        
        textButton.setButtonText (TRANS("Load"));
        textButton.setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
        textButton.setColour (TextButton::buttonColourId, Colour (0xFF5B5B5B));
        textButton.setColour (TextButton::buttonOnColourId, Colour (0xFF9A9A9A));
        textButton.addListener(this);
    }
    
    void resized() override
    {
        textButton.setBoundsInset (BorderSize<int> (2));
    }
    
    void paint(Graphics& g) override
    {
        g.setColour (Colours::black.withAlpha (0.2f));
        g.drawRect(getWidth() - 1, 0, 1, getHeight());
    }
    
    // Our demo code will call this when we may need to update our contents
//    void setRowAndColumn (const int newRow, const int newColumn)
//    {
//        row = newRow;
//        columnId = newColumn;
////        imageButton.setToggleState(owner.getToInclude(row), dontSendNotification);
//        //        comboBox.setSelectedId (owner.getActualClass (row), dontSendNotification);
//    }
    
    void buttonClicked (Button* buttonThatWasClicked) override
    {
        owner.getMainComponent()->loadAudioFileToPlay(row);
    }
    
    
    void setRowAndColumn (const int newRow, const int newColumn)
    {
        row = newRow;
        columnId = newColumn;
    }
    

private:
    
    MidiAudioOutputComponent& owner;
    TextButton    textButton;
    int row, columnId;
};



//==============================================================================



class TextEditorColumnCustomComponent    :      public Component,
                                                public LabelListener
{
public:
    
    TextEditorColumnCustomComponent (MidiAudioOutputComponent& owner_) : owner (owner_)
    {
        addAndMakeVisible (textLabel);
        
        textLabel.addListener (this);
        
        textLabel.setEditable (true, false, false);
        textLabel.setColour(Label::textColourId, Colour (0xFF000000));
        textLabel.setColour(Label::backgroundColourId, Colour(0xDD999999));
        textLabel.setColour(Label::outlineColourId, Colour(0xFF000000));
        textLabel.setColour(TextEditor::textColourId, Colour (0xFF000000));
        textLabel.setColour(TextEditor::backgroundColourId, Colour (0xDD999999));
        textLabel.setColour(TextEditor::highlightColourId, Colour (0xFFDDDDDD));
        textLabel.setBorderSize(0, 0);
    }
    
    void resized() override
    {
        textLabel.setBoundsInset (BorderSize<int> (2));
    }
    
    void paint(Graphics& g) override
    {
        g.setColour (Colours::black.withAlpha (0.2f));
        g.drawRect(getWidth() - 1, 0, 1, getHeight());
    }
    
    // Our demo code will call this when we may need to update our contents
    void setRowAndColumn (const int newRow, const int newColumn)
    {
        row = newRow;
        columnId = newColumn;
        textLabel.setText(String(owner.getMidiDuration(row)), dontSendNotification);
    }
    
    void labelTextChanged (Label *labelThatHasChanged) override
    {
        owner.setMidiDuration(row, textLabel.getText().getIntValue());
    }
    
    
private:
    
    MidiAudioOutputComponent& owner;
    Label    textLabel;
    int row, columnId;
};



//==============================================================================



class LabelColumnCustomComponent    :      public Component

{
public:
    
    LabelColumnCustomComponent (MidiAudioOutputComponent& owner_) : owner (owner_)
    {
        addAndMakeVisible (textLabel);
    }
    
    void resized() override
    {
        textLabel.setBoundsInset (BorderSize<int> (2));
    }
    
    void paint(Graphics& g) override
    {
        g.setColour (Colours::black.withAlpha (0.2f));
        g.drawRect(getWidth() - 1, 0, 1, getHeight());
    }
    
    // Our demo code will call this when we may need to update our contents
    void setRowAndColumn (const int newRow, const int newColumn)
    {
        row = newRow;
        columnId = newColumn;
        textLabel.setText(owner.getAudioFilename(newRow), dontSendNotification);
    }
    
    
private:
    
    MidiAudioOutputComponent& owner;
    Label    textLabel;
    int row, columnId;
};



//==============================================================================



MidiAudioOutputComponent::MidiAudioOutputComponent()
{
    
    m_pcMainComponent = nullptr;
    
    addAndMakeVisible(table);
    table.setModel(this);
    
    table.setColour (ListBox::outlineColourId, Colours::grey);
    table.setOutlineThickness (1);
    
    
    // Add Columns To Table
    table.getHeader().addColumn("Class", 1, 50, 50, 50, TableHeaderComponent::notResizableOrSortable, -1);
    table.getHeader().addColumn("Midi Channel", 2, 100, 100, 100, TableHeaderComponent::notResizableOrSortable, -1);
    table.getHeader().addColumn("Midi Note", 3, 100, 100, 100, TableHeaderComponent::notResizableOrSortable, -1);
    table.getHeader().addColumn("Midi Duration", 4, 100, 100, 100, TableHeaderComponent::notResizableOrSortable, -1);
    table.getHeader().addColumn("Load Audio File", 5, 100, 100, 100, TableHeaderComponent::notResizableOrSortable, -1);
    table.getHeader().addColumn("Audio File", 6, 250, 250, 250, TableHeaderComponent::notResizableOrSortable, -1);
    table.getHeader().addColumn("Looping", 7, 100, 100, 100, TableHeaderComponent::notResizableOrSortable, -1);
    table.getHeader().addColumn("Toggle", 8, 100, 100, 100, TableHeaderComponent::notResizableOrSortable, -1);
    
    
//    table.setColour(ListBox::backgroundColourId, Colour(playingColour));
    
    
    //    table.getHeader().setStretchToFitActive (true);
    
    
    clearRows();
    
}


MidiAudioOutputComponent::~MidiAudioOutputComponent()
{    
    clearRows();
}


void MidiAudioOutputComponent::resized()
{
    // position our table with a gap around its edge
    //    table.setBoundsInset (BorderSize<int> (8));
    table.setBounds(0, 0, getWidth(), getHeight());
}


void MidiAudioOutputComponent::paint(Graphics &g)
{
    
}


void MidiAudioOutputComponent::paintRowBackground(Graphics &g, int rowNumber, int width, int height, bool rowIsSelected)
{
    g.fillAll(Colour(float((numRows - rowNumber))/float(numRows), 0.75f, 0.75f, 0.75f));
}



void MidiAudioOutputComponent::paintCell (Graphics& g, int rowNumber, int columnId,
                                        int width, int height, bool /*rowIsSelected*/)
{
    g.setColour (Colours::black);
    g.setFont (Font(14.0f));
    
    //    const XmlElement* rowElement = dataList->getChildElement (rowNumber);
    
    //    if (rowElement != 0)
    //    {
    //        const String text (rowElement->getStringAttribute (getAttributeNameForColumnId (columnId)));
    //
    //        g.drawText (text, 2, 0, width - 4, height, Justification::centredLeft, true);
    //    }
    
    switch (columnId)
    {
        case 1:
            g.drawText (String(m_piClasses.getUnchecked(rowNumber)),
                        2, 0, width - 4, height, Justification::centredLeft, true);
            break;
            
            
        default:
            break;
    }
    
    g.setColour (Colours::black.withAlpha (0.2f));
    g.fillRect (width - 1, 0, 1, height);
}



// This is overloaded from TableListBoxModel, and must update any custom components that we're using
Component* MidiAudioOutputComponent::refreshComponentForCell (int rowNumber, int columnId, bool /*isRowSelected*/,
                                                            Component* existingComponentToUpdate)
{
    // MIDI Channel and Note Combo Boxes
    if ((columnId == 2) || (columnId == 3))
    {
        MidiComboBoxCustomComponent* midiBox = (MidiComboBoxCustomComponent*) existingComponentToUpdate;
        
        // If an existing component is being passed-in for updating, we'll re-use it, but
        // if not, we'll have to create one.
        if (midiBox == 0)
        {
            if (columnId == 2)
            {
                midiBox = new MidiComboBoxCustomComponent (*this, true);
            }
            
            else if (columnId == 3)
            {
                midiBox = new MidiComboBoxCustomComponent (*this, false);
            }
            
        }
        
        midiBox->setRowAndColumn (rowNumber, columnId);
        
        return midiBox;
    }
    
    
    else if (columnId == 4)
    {
        TextEditorColumnCustomComponent* durationLabel = (TextEditorColumnCustomComponent*) existingComponentToUpdate;
        
        if (durationLabel == 0)
            durationLabel = new TextEditorColumnCustomComponent (*this);
        
        durationLabel->setRowAndColumn (rowNumber, columnId);
        
        return durationLabel;
    }
    
    
    else if (columnId == 5)
    {
        TextButtonColumnCustomComponent* loadFileButton = (TextButtonColumnCustomComponent*) existingComponentToUpdate;
        
        // If an existing component is being passed-in for updating, we'll re-use it, but
        // if not, we'll have to create one.
        if (loadFileButton == 0)
        {
            loadFileButton = new TextButtonColumnCustomComponent (*this);
        }
        
        loadFileButton->setRowAndColumn (rowNumber, columnId);
        
        return loadFileButton;
    }
    
    
    else if (columnId == 6)
    {
        LabelColumnCustomComponent* audioFileName = (LabelColumnCustomComponent*) existingComponentToUpdate;
        
        // If an existing component is being passed-in for updating, we'll re-use it, but
        // if not, we'll have to create one.
        if (audioFileName == 0)
        {
            audioFileName = new LabelColumnCustomComponent (*this);
        }
        
        audioFileName->setRowAndColumn (rowNumber, columnId);
        
        return audioFileName;
    }
    
    
    
    else if (columnId == 7)
    {
        ToggleButtonColumnCustomComponent* loopingToggle = (ToggleButtonColumnCustomComponent*) existingComponentToUpdate;
        
        if (loopingToggle == 0)
            loopingToggle = new ToggleButtonColumnCustomComponent (*this, true);
        
        loopingToggle->setRowAndColumn (rowNumber, columnId);
        
        return loopingToggle;
    }
    
    
    
    else if (columnId == 8)
    {
        ToggleButtonColumnCustomComponent* toggleToggle = (ToggleButtonColumnCustomComponent*) existingComponentToUpdate;
        
        if (toggleToggle == 0)
            toggleToggle = new ToggleButtonColumnCustomComponent (*this, false);
        
        toggleToggle->setRowAndColumn (rowNumber, columnId);
        
        return toggleToggle;
    }

    
    
    else
    {
        // for any other column, just return 0, as we'll be painting these columns directly.
        jassert (existingComponentToUpdate == 0);
        return 0;
    }
    
}



int MidiAudioOutputComponent::getNumRows()
{
    return numRows;
}

void MidiAudioOutputComponent::setNumRows(int newNumRows)
{
    numRows = newNumRows;
}


int MidiAudioOutputComponent::getMidiChannel (const int rowNumber) const
{
    if (m_piMidiChannels.size() != 0)
    {
        return (m_piMidiChannels.getUnchecked(rowNumber));
    }
    
    else
    {
        return 0;
    }
}


void MidiAudioOutputComponent::setMidiChannel (const int rowNumber, const int newChannel)
{
    if (m_piMidiChannels.size() != 0)
    {
        m_piMidiChannels.set(rowNumber, newChannel);
        updateMidiData(rowNumber);
    }
}




int MidiAudioOutputComponent::getMidiNote (const int rowNumber) const
{
    if (m_piMidiNotes.size() != 0)
    {
        return (m_piMidiNotes.getUnchecked(rowNumber));
    }
    
    else
    {
        return 0;
    }
}


void MidiAudioOutputComponent::setMidiNote (const int rowNumber, const int newClass)
{
    if (m_piMidiNotes.size() != 0)
    {
        m_piMidiNotes.set(rowNumber, newClass);
        updateMidiData(rowNumber);
    }
}



int MidiAudioOutputComponent::getMidiDuration (const int rowNumber) const
{
    if (m_piMidiDuration.size() != 0)
    {
        return (m_piMidiDuration.getUnchecked(rowNumber));
    }
    
    else
    {
        return 0;
    }
}


void MidiAudioOutputComponent::setMidiDuration (const int rowNumber, const int newClass)
{
    if (m_piMidiDuration.size() != 0)
    {
        m_piMidiDuration.set(rowNumber, newClass);
        updateMidiData(rowNumber);
    }
}





bool MidiAudioOutputComponent::getLooping(const int rowNumber) const
{
    if (m_pbLooping.size() != 0)
    {
        return (m_pbLooping.getUnchecked(rowNumber));
    }
    
    else
    {
        return false;
    }
}


void MidiAudioOutputComponent::setLooping(const int rowNumber, const bool newToggle)
{
    if (m_pbLooping.size() != 0)
    {
        m_pbLooping.set(rowNumber, newToggle);
        m_pcMainComponent->setAudioFileParam(rowNumber, m_pbLooping.getUnchecked(rowNumber), m_pbToggle.getUnchecked(rowNumber));
    }
}


void MidiAudioOutputComponent::setAudioFilename(int rowNumber, String filename)
{
    if ((rowNumber >= 0) && (rowNumber < m_psAudioFilenames.size()))
    {
        m_psAudioFilenames.set(rowNumber, filename);
    }
    
    updateTable();
}


void MidiAudioOutputComponent::setToggle(const int rowNumber, const bool newToggle)
{
    if (m_pbToggle.size() != 0)
    {
        m_pbToggle.set(rowNumber, newToggle);
        m_pcMainComponent->setAudioFileParam(rowNumber, m_pbLooping.getUnchecked(rowNumber), m_pbToggle.getUnchecked(rowNumber));
    }
}


bool MidiAudioOutputComponent::getToggle(const int rowNumber) const
{
    if (m_pbToggle.size() != 0)
    {
        return (m_pbToggle.getUnchecked(rowNumber));
    }
    
    else
    {
        return false;
    }
}




String MidiAudioOutputComponent::getAudioFilename(const int rowNumber)
{
    if (m_psAudioFilenames.size() != 0)
    {
        return (m_psAudioFilenames.getReference(rowNumber));
    }
    
    else
    {
        return "";
    }
}



void MidiAudioOutputComponent::addRow(int classIndex, int midiChannel, int midiNote, int midiDuration,
                                      String filename, bool looping, bool toggle)
{
    numRows++;
    m_piClasses.add(classIndex);
    m_piMidiChannels.add(midiChannel);
    m_piMidiNotes.add(midiNote);
    m_piMidiDuration.add(midiDuration);
    m_psAudioFilenames.add(filename);
    m_pbLooping.add(looping);
    m_pbToggle.add(toggle);
    
    updateTable();
    
}


void MidiAudioOutputComponent::deleteRow(int rowIndex)
{
    numRows--;
    
    if (numRows < 0)
    {
        numRows = 0;
    }
    
    m_piClasses.remove(rowIndex);
    m_piMidiChannels.remove(rowIndex);
    m_piMidiNotes.remove(rowIndex);
    m_piMidiDuration.remove(rowIndex);
    m_psAudioFilenames.remove(rowIndex);
    m_pbLooping.remove(rowIndex);
    m_pbToggle.remove(rowIndex);
    
    updateTable();
}


void MidiAudioOutputComponent::clearRows()
{
    numRows = 0;
    
    m_piClasses.clear();
    m_piMidiChannels.clear();
    m_piMidiNotes.clear();
    m_piMidiDuration.clear();
    m_psAudioFilenames.clear();
    m_pbLooping.clear();
    m_pbToggle.clear();
}


void MidiAudioOutputComponent::updateTable()
{
    table.updateContent();
}



void MidiAudioOutputComponent::setMainComponent(MainComponent *component)
{
    m_pcMainComponent = component;
}

MainComponent* MidiAudioOutputComponent::getMainComponent()
{
    return m_pcMainComponent;
}


void MidiAudioOutputComponent::updateMidiData(int rowNumber)
{
    m_pcMainComponent->setMidiOutput(rowNumber, m_piMidiChannels.getUnchecked(rowNumber), m_piMidiNotes.getUnchecked(rowNumber), m_piMidiDuration.getUnchecked(rowNumber));
}

//==============================================================================



SettingsContentComponent::SettingsContentComponent ()
{
    addAndMakeVisible (audioSetupButton = new TextButton ("audioSetupButton"));
    audioSetupButton->setButtonText (TRANS("Audio Setup"));
    audioSetupButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    audioSetupButton->setColour (TextButton::buttonColourId, Colour (0xFF5B5B5B));
    audioSetupButton->setColour (TextButton::buttonOnColourId, Colour (0xFF9A9A9A));
    
    
    addAndMakeVisible (velocitySensitivitySlider = new Slider ("velocitySensitivitySlider"));
    velocitySensitivitySlider->setRange (0.001, 1, 0.001);
    velocitySensitivitySlider->setSliderStyle (Slider::Rotary);
    velocitySensitivitySlider->setTextBoxStyle (Slider::TextBoxBelow, false, 50, 20);
    velocitySensitivitySlider->setColour (Slider::thumbColourId, Colour (0xFF464E56));
    velocitySensitivitySlider->setColour (Slider::rotarySliderFillColourId, Colour (0x7F464E55));
    velocitySensitivitySlider->setColour (Slider::textBoxTextColourId, Colour (0xFF404952));
    velocitySensitivitySlider->setColour (Slider::textBoxBackgroundColourId, Colour (0x00000000));
    velocitySensitivitySlider->setColour (Slider::textBoxHighlightColourId, Colour (0x00000000));
    velocitySensitivitySlider->setColour (Slider::textBoxOutlineColourId, Colour (0x00808080));
    velocitySensitivitySlider->setVelocityBasedMode(true);
    velocitySensitivitySlider->setVelocityModeParameters(0.01,1,0.0,true);
    
    
    addAndMakeVisible (decayTimeSensitivitySlider = new Slider ("decayTimeSensitivitySlider"));
    decayTimeSensitivitySlider->setRange (0.025, 1, 0.025);
    decayTimeSensitivitySlider->setSliderStyle (Slider::Rotary);
    decayTimeSensitivitySlider->setTextBoxStyle (Slider::TextBoxBelow, false, 50, 20);
    decayTimeSensitivitySlider->setColour (Slider::thumbColourId, Colour (0xFF464E56));
    decayTimeSensitivitySlider->setColour (Slider::rotarySliderFillColourId, Colour (0x7F464E55));
    decayTimeSensitivitySlider->setColour (Slider::textBoxTextColourId, Colour (0xFF404952));
    decayTimeSensitivitySlider->setColour (Slider::textBoxBackgroundColourId, Colour (0x00000000));
    decayTimeSensitivitySlider->setColour (Slider::textBoxHighlightColourId, Colour (0x00000000));
    decayTimeSensitivitySlider->setColour (Slider::textBoxOutlineColourId, Colour (0x00808080));
    decayTimeSensitivitySlider->setVelocityBasedMode(true);
    decayTimeSensitivitySlider->setVelocityModeParameters(0.01,1,0.0,true);
    
    
    
    
    addAndMakeVisible (velocitySensitivityLabel = new Label ("velocitySensitivityLabel",
                                                             TRANS("Velocity Sensitivity\n")));
    velocitySensitivityLabel->setFont (Font ("Myriad Pro", 11.90f, Font::bold));
    velocitySensitivityLabel->setJustificationType (Justification::centred);
    velocitySensitivityLabel->setEditable (false, false, false);
    velocitySensitivityLabel->setColour (Label::textColourId, Colour (0xFF5C6266));
    velocitySensitivityLabel->setColour (TextEditor::textColourId, Colour (0xff3d4248));
    velocitySensitivityLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    velocitySensitivityLabel->setColour (TextEditor::highlightColourId, Colour (0x00000000));
    
    
    
    
    addAndMakeVisible (decayTimeSensitivityLabel = new Label ("decayTimeSensitivityLabel",
                                                              TRANS("Decay Time Sensitivity")));
    decayTimeSensitivityLabel->setFont (Font ("Myriad Pro", 12.00f, Font::bold));
    decayTimeSensitivityLabel->setJustificationType (Justification::centred);
    decayTimeSensitivityLabel->setEditable (false, false, false);
    decayTimeSensitivityLabel->setColour (Label::textColourId, Colour (0xFF5C6266));
    decayTimeSensitivityLabel->setColour (TextEditor::textColourId, Colour (0xff3d4248));
    decayTimeSensitivityLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    decayTimeSensitivityLabel->setColour (TextEditor::highlightColourId, Colour (0x00000000));
    
    

    
    addAndMakeVisible (tempoNumBox = new Label ("tempoNumBox", TRANS("120.00")));
    tempoNumBox->setFont (Font ("Myriad Pro", 16.00f, Font::bold));
    tempoNumBox->setJustificationType (Justification::centred);
    tempoNumBox->setEditable (true, false, false);
    tempoNumBox->setColour(Label::textColourId, Colour (0xFF000000));
    tempoNumBox->setColour(Label::backgroundColourId, Colour(0xDD999999));
    tempoNumBox->setColour(Label::outlineColourId, Colour(0xFF000000));
    tempoNumBox->setColour(TextEditor::textColourId, Colour (0xFF000000));
    tempoNumBox->setColour(TextEditor::backgroundColourId, Colour (0xDD999999));
    tempoNumBox->setColour(TextEditor::highlightColourId, Colour (0xFFDDDDDD));
    tempoNumBox->setBorderSize(0, 0);
    
    
    addAndMakeVisible (numeratorNumBox = new Label ("numeratorNumBox", TRANS("4")));
    numeratorNumBox->setFont (Font ("Myriad Pro", 15.00f, Font::bold));
    numeratorNumBox->setJustificationType (Justification::centred);
    numeratorNumBox->setEditable (true, false, false);
    numeratorNumBox->setColour(Label::textColourId, Colour (0xFF000000));
    numeratorNumBox->setColour(Label::backgroundColourId, Colour(0xDD999999));
    numeratorNumBox->setColour(Label::outlineColourId, Colour(0xFF000000));
    numeratorNumBox->setColour(TextEditor::textColourId, Colour (0xFF000000));
    numeratorNumBox->setColour(TextEditor::backgroundColourId, Colour (0xDD999999));
    numeratorNumBox->setColour(TextEditor::highlightColourId, Colour (0xFFDDDDDD));
    numeratorNumBox->setBorderSize(0, 0);
    
    
    addAndMakeVisible (denominatorNumBox = new Label ("denominatorNumBox", TRANS("4")));
    denominatorNumBox->setFont (Font ("Myriad Pro", 15.00f, Font::bold));
    denominatorNumBox->setJustificationType (Justification::centred);
    denominatorNumBox->setEditable (true, false, false);
    denominatorNumBox->setColour(Label::textColourId, Colour (0xFF000000));
    denominatorNumBox->setColour(Label::backgroundColourId, Colour(0xDD999999));
    denominatorNumBox->setColour(Label::outlineColourId, Colour(0xFF000000));
    denominatorNumBox->setColour(TextEditor::textColourId, Colour (0xFF000000));
    denominatorNumBox->setColour(TextEditor::backgroundColourId, Colour (0xDD999999));
    denominatorNumBox->setColour(TextEditor::highlightColourId, Colour (0xFFDDDDDD));
    denominatorNumBox->setBorderSize(0, 0);
    
    
    
    addAndMakeVisible (tempoLabel = new Label ("tempoLabel", TRANS("Tempo")));
    tempoLabel->setFont (Font ("Myriad Pro", 14.00f, Font::bold));
    tempoLabel->setJustificationType (Justification::right);
    tempoLabel->setEditable (false, false, false);
    tempoLabel->setColour (Label::textColourId, Colour (0xFF667788));
    tempoLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    tempoLabel->setColour (TextEditor::highlightColourId, Colour (0x00000000));
    
    
    addAndMakeVisible (meterLabel = new Label ("meterLabel", TRANS("Meter")));
    meterLabel->setFont (Font ("Myriad Pro", 14.00f, Font::bold));
    meterLabel->setJustificationType (Justification::right);
    meterLabel->setEditable (false, false, false);
    meterLabel->setColour (Label::textColourId, Colour (0xFF667788));
    meterLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    meterLabel->setColour (TextEditor::highlightColourId, Colour (0x00000000));
    
    
    addAndMakeVisible(midiAudioComponent = new MidiAudioOutputComponent());
    
    
    m_iNumClasses   =   0;
    
    

    setSize (getParentWidth(), getParentHeight());
}



SettingsContentComponent::~SettingsContentComponent()
{
    velocitySensitivitySlider   = nullptr;
    decayTimeSensitivitySlider  = nullptr;
    velocitySensitivityLabel    = nullptr;
    decayTimeSensitivityLabel   = nullptr;
    audioSetupButton            = nullptr;
    tempoNumBox                 = nullptr;
    numeratorNumBox             = nullptr;
    denominatorNumBox           = nullptr;
    tempoLabel                  = nullptr;
    meterLabel                  = nullptr;
    
    midiAudioComponent          = nullptr;
    
    deleteAllChildren();
}



void SettingsContentComponent::paint (Graphics& g)
{
    g.fillAll (Colour (0xff2b2b2b));
}


void SettingsContentComponent::resized()
{
    audioSetupButton->setBounds((getWidth() / 2) - ((120) / 2), (getHeight() / 2) - ((40) / 2) - 20, 120, 40);
    velocitySensitivitySlider->setBounds ((getWidth() / 2) + -90 - ((90) / 2), (getHeight() / 2) + 90 - ((90) / 2), 90, 90);
    decayTimeSensitivitySlider->setBounds ((getWidth() / 2) + 90 - ((90) / 2), (getHeight() / 2) + 90 - ((90) / 2), 90, 90);
    velocitySensitivityLabel->setBounds ((getWidth() / 2) + -90 - ((150) / 2), (getHeight() / 2) + 145 - ((20) / 2), 150, 20);
    decayTimeSensitivityLabel->setBounds ((getWidth() / 2) + 90 - ((150) / 2), (getHeight() / 2) + 145 - ((20) / 2), 150, 20);
    
    tempoNumBox          -> setBounds(getWidth() - 80, 90, 60, 20);
    numeratorNumBox      -> setBounds(getWidth() - 80, 110, 25, 20);
    denominatorNumBox    -> setBounds(getWidth() - 45, 110, 25, 20);
    
    tempoLabel           -> setBounds(getWidth() - 140, 90, 60, 20);
    meterLabel           -> setBounds(getWidth() - 140, 110, 60, 20);
    
    midiAudioComponent   -> setBounds(10, 10, 910, getHeight()/2 - 100);
}




void SettingsContentComponent::addClass()
{
    m_iNumClasses++;
    
    if (m_iNumClasses > 0)
    {
        midiAudioComponent->addRow(m_iNumClasses, 0, 0, 250, "", false, false);
    }
    
}



void SettingsContentComponent::deleteClass(int classIndex)
{
    m_iNumClasses--;
    
    if (m_iNumClasses < 0)
    {
        m_iNumClasses = 0;
    }
    
    if (index >= 0)
    {
        midiAudioComponent->deleteRow(classIndex - 1);
    }
    
}


void SettingsContentComponent::setAudioFileName(int index, String fileName)
{
    if (index >= 0)
    {
        midiAudioComponent->setAudioFilename(index, fileName);
    }
}

void SettingsContentComponent::setMainComponent(MainComponent *component)
{
    midiAudioComponent->setMainComponent(component);
}

MainComponent* SettingsContentComponent::getMainComponent()
{
    return (midiAudioComponent->getMainComponent());
}