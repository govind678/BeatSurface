/*
  ==============================================================================

    TranscriptionComponent.cpp
    Created: 26 Mar 2014 7:20:23pm
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#include "TranscriptionComponent.h"



//==============================================================================



class ClassColumnCustomComponent    :   public Component,
                                        public ComboBoxListener
{
public:
    
    ClassColumnCustomComponent (TranscriptionComponent& owner_) : owner (owner_)
    {
        // just put a combo box inside this component
        addAndMakeVisible (comboBox);
        
        // when the combo is changed, we'll get a callback.
        comboBox.addListener (this);
        comboBox.setWantsKeyboardFocus (false);
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
        comboBox.setSelectedId (owner.getActualClass (row), dontSendNotification);
    }
    
    void comboBoxChanged (ComboBox* /*comboBoxThatHasChanged*/) override
    {
        owner.setActualClass (row, comboBox.getSelectedId());
    }
    
    void setNumClasses(int numClasses)
    {
        comboBox.clear();
        
        for (int i=1; i <= numClasses; i++)
        {
            comboBox.addItem(String(i), i);
        }
    }
    
    
private:
    
    TranscriptionComponent& owner;
    ComboBox comboBox;
    int row, columnId;

};



//==============================================================================



class IncludeColumnCustomComponent    :     public Component,
                                            public ButtonListener
{
public:
    
    IncludeColumnCustomComponent (TranscriptionComponent& owner_) : owner (owner_)
    {
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
        toggleButton.setToggleState(owner.getToInclude(row), dontSendNotification);
//        comboBox.setSelectedId (owner.getActualClass (row), dontSendNotification);
    }
    
    void buttonClicked (Button* buttonThatWasClicked) override
    {
        owner.setToInclude (row, toggleButton.getToggleState());
    }
    
private:
    
    TranscriptionComponent& owner;
    ToggleButton    toggleButton;
    int row, columnId;
};




//==============================================================================



TranscriptionComponent::TranscriptionComponent()
{
    trainingColour      = Colour(0xDDFECBCB);
    playingColour       = Colour(0xFFEDEDED);
    
    
    addAndMakeVisible(table);
    table.setModel(this);
    
    table.setColour (ListBox::outlineColourId, Colours::grey);
    table.setOutlineThickness (1);
    
    
    // Add Columns To Table
    table.getHeader().addColumn("Index", 1, 50, 50, 50, TableHeaderComponent::notResizableOrSortable, -1);
    table.getHeader().addColumn("Include", 2, 50, 50, 50, TableHeaderComponent::notResizableOrSortable, -1);
    table.getHeader().addColumn("Time", 3, 250, 250, 250, TableHeaderComponent::notResizableOrSortable, -1);
    table.getHeader().addColumn("Predicted Class", 4, 100, 100, 100, TableHeaderComponent::notResizableOrSortable, -1);
    table.getHeader().addColumn("Features", 5, 300, 300, 300, TableHeaderComponent::notResizableOrSortable, -1);
    table.getHeader().addColumn("Actual Class", 6, 100, 100, 100, TableHeaderComponent::notResizableOrSortable, -1);
    
    table.setColour(ListBox::backgroundColourId, Colour(playingColour));
    
    
//    table.getHeader().setStretchToFitActive (true);
    
    addAndMakeVisible (retrainButton = new TextButton ("retrainButton"));
    retrainButton->setButtonText (TRANS("Update Training"));
    retrainButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    retrainButton->setColour (TextButton::buttonColourId, Colour (0xFF5B5B5B));
    retrainButton->setColour (TextButton::buttonOnColourId, Colour (0xFF9A9A9A));
    
    addAndMakeVisible (clearButton = new TextButton ("clearButton"));
    clearButton->setButtonText (TRANS("Clear Button"));
    clearButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    clearButton->setColour (TextButton::buttonColourId, Colour (0xFF5B5B5B));
    clearButton->setColour (TextButton::buttonOnColourId, Colour (0xFF9A9A9A));
    
    clearRows();
    m_iNumClasses = 0;
    
}


TranscriptionComponent::~TranscriptionComponent()
{
    retrainButton = nullptr;
    clearButton   = nullptr;
    
    clearRows();
}


void TranscriptionComponent::resized()
{
    // position our table with a gap around its edge
//    table.setBoundsInset (BorderSize<int> (8));
    table.setBounds(50, 10, 910, getHeight() - 40);
    
    retrainButton->setBounds(1000, 100, 150, 40);
    clearButton->setBounds(1000, 180, 150, 40);
}


void TranscriptionComponent::paint(Graphics &g)
{
    
}


void TranscriptionComponent::paintRowBackground(Graphics &g, int rowNumber, int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
    {
        g.fillAll (Colours::lightblue);
    }
    
    else
    {
        if ((rowNumber % 2) == 0)
        {
            g.fillAll(Colour(0xAADDDDDD));
        }
        
        else
        {
            g.fillAll(Colour(0xAACCCCCC));
        }
    }
}



void TranscriptionComponent::paintCell (Graphics& g, int rowNumber, int columnId,
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
            g.drawText (String(m_piIndices.getUnchecked(rowNumber)),
                        2, 0, width - 4, height, Justification::centredLeft, true);
            break;
            
        case 3:
            g.drawText (m_psTime.getReference(rowNumber),
                        2, 0, width - 4, height, Justification::centredLeft, true);
            break;
            
        case 4:
            g.drawText (String(m_piPredictedClasses.getUnchecked(rowNumber)),
                        2, 0, width - 4, height, Justification::centredLeft, true);
            break;
            
        case 5:
            g.drawText (m_psFeatures.getReference(rowNumber),
                        2, 0, width - 4, height, Justification::centredLeft, true);
            break;
            
        default:
            break;
    }
    
    g.setColour (Colours::black.withAlpha (0.2f));
    g.fillRect (width - 1, 0, 1, height);
}



// This is overloaded from TableListBoxModel, and must update any custom components that we're using
Component* TranscriptionComponent::refreshComponentForCell (int rowNumber, int columnId, bool /*isRowSelected*/,
                                    Component* existingComponentToUpdate)
{
    if (columnId == 2) // If it's the includes column, we'll return our custom component..
    {
        IncludeColumnCustomComponent* includesToggle = (IncludeColumnCustomComponent*) existingComponentToUpdate;
        
        // If an existing component is being passed-in for updating, we'll re-use it, but
        // if not, we'll have to create one.
        if (includesToggle == 0)
            includesToggle = new IncludeColumnCustomComponent (*this);
        
        includesToggle->setRowAndColumn (rowNumber, columnId);
        
        return includesToggle;
    }
    
    if (columnId == 6) // If it's the actualClass column, we'll return our custom component..
    {
        ClassColumnCustomComponent* actualClassBox = (ClassColumnCustomComponent*) existingComponentToUpdate;
        
        // If an existing component is being passed-in for updating, we'll re-use it, but
        // if not, we'll have to create one.
        if (actualClassBox == 0)
        {
            actualClassBox = new ClassColumnCustomComponent (*this);
        }
        
        actualClassBox->setNumClasses(m_iNumClasses);
        actualClassBox->setRowAndColumn (rowNumber, columnId);
        
        return actualClassBox;
    }
    else
    {
        // for any other column, just return 0, as we'll be painting these columns directly.
        jassert (existingComponentToUpdate == 0);
        return 0;
    }
    
}



int TranscriptionComponent::getNumRows()
{
    return numRows;
}

void TranscriptionComponent::setNumRows(int newNumRows)
{
    numRows = newNumRows;
}


int TranscriptionComponent::getActualClass (const int rowNumber) const
{
    if (m_piActualClasses.size() != 0)
    {
        return (m_piActualClasses.getUnchecked(rowNumber));
    }
    
    else
    {
        return 0;
    }
}


void TranscriptionComponent::setActualClass (const int rowNumber, const int newClass)
{
    if (m_piActualClasses.size() != 0)
    {
        m_piActualClasses.set(rowNumber, newClass);
    }
}


bool TranscriptionComponent::getToInclude(const int rowNumber) const
{
    if (m_pbIncludes.size() != 0)
    {
        return (m_pbIncludes.getUnchecked(rowNumber));
    }
    
    else
    {
        return false;
    }
}


void TranscriptionComponent::setToInclude(const int rowNumber, const bool newToggle)
{
    if (m_pbIncludes.size() != 0)
    {
        m_pbIncludes.set(rowNumber, newToggle);
    }
}


void TranscriptionComponent::addRow(int index, String time, int predictedClass, String features)
{
    numRows++;
    
    m_piIndices.add(index);
    m_pbIncludes.add(true);
    m_piPredictedClasses.add(predictedClass);
    m_piActualClasses.add(predictedClass);
    m_psFeatures.add(features);
    m_psTime.add(time);
    
    table.updateContent();
    
}

void TranscriptionComponent::setNumClasses(int numClasses)
{
    m_iNumClasses = numClasses;
    table.updateContent();
}

void TranscriptionComponent::clearRows()
{
    numRows = 0;
    
    m_piIndices.clear();
    m_pbIncludes.clear();
    m_piPredictedClasses.clear();
    m_piActualClasses.clear();
    m_psFeatures.clear();
    m_psTime.clear();
    
    table.updateContent();
}

void TranscriptionComponent::setSystemMode(BeatSurfaceBase::SystemMode newMode)
{
    if (newMode == BeatSurfaceBase::TrainMode)
    {
        table.setColour(ListBox::backgroundColourId, Colour(trainingColour));
        table.updateContent();
    }
    
    else
    {
        table.setColour(ListBox::backgroundColourId, Colour(playingColour));
        table.updateContent();
    }
}

Array<bool> TranscriptionComponent::getCurrentIncludes()
{
    return m_pbIncludes;
}


Array<int> TranscriptionComponent::getCurrentClasses()
{
    return m_piActualClasses;
}

void TranscriptionComponent::updateTable()
{
    table.updateContent();
}