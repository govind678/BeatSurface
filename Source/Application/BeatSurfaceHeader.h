/*
  ==============================================================================

    BeatSurfaceHeader.h
    Created: 13 Feb 2014 6:08:10pm
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#ifndef BEATSURFACEHEADER_H_INCLUDED
#define BEATSURFACEHEADER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "CommandIDs.h"
#include "GlobalClock.h"
#include "CustomShapeButton.h"
#include "GUIUpdater.h"



/**
    Base class that is included in all the back end classes.
 
    Includes initialization of audio device parameters and application mode enums.
 */


extern ScopedPointer<ApplicationCommandManager>     commandManager;
extern ScopedPointer<AudioDeviceManager>            sharedAudioDeviceManager;
extern ScopedPointer<GUIUpdater>                    guiUpdater;
extern ScopedPointer<GlobalClock>                   globalClock;


//=========================================== BeatSurfaceBase Class ======================================================
// Initializations and Includes
//========================================================================================================================

class BeatSurfaceBase
{
    
public:
    
    
    //--- Audio Device Selector Component ---//
    static const int iMinNoInputChannelsRequired        = 0;
    static const int iMinNoOutputChannelsRequired       = 0;
    static const int iMaxNoInputChannelsRequired        = 4;
    static const int iMaxNoOutputChannelssRequired      = 4;
    static const bool bShowMidiInputOptions             = true;
    static const bool bShowMidiOutputSelector           = true;
    static const bool bShowChannelsAsStereoPairs        = true;
    static const bool bHideAdvancedOptionsWithButton    = false;
    
    
    //--- Shared Audio Device Manager ---//
    static const int iNumInputChannels                  = 2;
    static const int iNumOutputChannels                 = 2;
    
    
    //--- Look and Feel ---//
    
    
    
    //--- GUI Timer Interval ---//
    static const int iGUITimerInterval                  = 5;    // 5 ms
    
    
    
    //--- System Modes ---//
    enum SystemMode
    {
        TrainMode,
        PlayMode,
        FileTrainMode,
        IdleMode
    };
    
    
    //--- Parameter IDs ---//
    enum ParameterID
    {
        VelocitySensitivity,
        DecayTimeSensitivity
    };
    
    
private:
    
    
    JUCE_DECLARE_NON_COPYABLE (BeatSurfaceBase);
};

//========================================================================================================================








//============================================= Class Button Array =======================================================
// Use extended Shape Button to dynamically create pie-shaped buttons based on number of classes
//========================================================================================================================

class ClassButtonArray  :   public Component,
                            public Timer
{
public:
    
    
    //==============================================================================
    // Constructor and Destructor
    
    ClassButtonArray()
    {
        m_iNumButtons = 0;
        m_iButtonFlashTime_ms = 40;    // Default 20 ms
        m_fFinalAlpha   =   0.2f;
        m_bFlashArray = false;
        m_pcClassButton.clear(true);
    }
    
    
    ~ClassButtonArray()
    {
        
    }
    //==============================================================================

    
    
    
    
    
    //==============================================================================
    // Repaint Component (Create stroke lines indicating number of classes)
    
    void paint (Graphics& g)
    {
        if (m_iNumButtons > 0) {
            
            
            for (int side = 1; side <= m_iNumButtons; side++)
            {
                Path pie;
                
                float fromRadians   = float((side - 1.0f) * 2.0f * M_PI) / float(m_iNumButtons);
                float toRadians     = float(side * 2.0f * M_PI) / float(m_iNumButtons);
                
                pie.addPieSegment(0.0f, 0.0f, getWidth(), getHeight(), fromRadians, toRadians, 0.0f);
                pie.closeSubPath();
                g.setColour(Colours::black);
                g.strokePath(pie, PathStrokeType(1.0f));
            }
            
            
        }
    }
    //==============================================================================
    
    
    
    
    
    
    //==============================================================================
    // Parent Component Resized
    
    void resized()
    {
        if (m_iNumButtons > 0)
        {
            for (int side = 1; side <= m_iNumButtons; side++)
            {
                Path pie;
                
                float fromRadians   = float((side - 1.0f) * 2.0f * M_PI) / float(m_iNumButtons);
                float toRadians     = float(side * 2.0f * M_PI) / float(m_iNumButtons);
                
                pie.addPieSegment(0.0f, 0.0f, getWidth(), getHeight(), fromRadians, toRadians, 0.0f);
                pie.closeSubPath();
                
                m_pcClassButton.getUnchecked(side-1)->setShape(pie, true, true);
                m_pcClassButton.getUnchecked(side-1)->setBounds(0, 0, getWidth(), getHeight());
            }
        }
    }
    //==============================================================================
    
    
    
    
    
    //==============================================================================
    // Set Number of Buttons and Recreate Button Array
    
    void setNumClasses(int numButtons)
    {
        m_iNumButtons = numButtons;
        
        if(m_pcClassButton.size() > 0)
        {
            m_pcClassButton.clear(true);
        }
        
        
        
        if (m_iNumButtons > 0) {
            
            
            for (int side = 1; side <= m_iNumButtons; side++) {
                
                m_pcClassButton.add(new CustomShapeButton(String(side),
                                                          Colour(float((m_iNumButtons - side) + 1)/float(m_iNumButtons), 0.75f, 0.9f, 1.0f),
                                                          Colour(float((m_iNumButtons - side) + 1)/float(m_iNumButtons), 0.75f, 0.7f, 1.0f),
                                                          Colour(float((m_iNumButtons - side) + 1)/float(m_iNumButtons), 0.75f, 0.5f, 1.0f)));
                
                Path pie;
                
                float fromRadians   = float((side - 1.0f) * 2.0f * M_PI) / float(m_iNumButtons);
                float toRadians     = float(side * 2.0f * M_PI) / float(m_iNumButtons);
                
                pie.addPieSegment(0.0f, 0.0f, getWidth(), getHeight(), fromRadians, toRadians, 0.0f);
                pie.closeSubPath();
                
                m_pcClassButton.getUnchecked(side-1)->setShape(pie, true, true);
                addAndMakeVisible(m_pcClassButton.getUnchecked(side-1));
                m_pcClassButton.getUnchecked(side-1)->setBounds(0, 0, getWidth(), getHeight());
            }
            
            
        }
    }
    //==============================================================================
    
    
    
    
    //==============================================================================
    // Flash All Buttons
    
//    void flashArray(std::vector<float> alphaValues)
//    {
//        if (m_iNumButtons > 0)
//        {
//            for (int i=0; i < m_iNumButtons; i++)
//            {
//                m_pcClassButton.getUnchecked(i)->setAlpha(alphaValues[i]);
//            }
//            
//            m_bFlashArray = true;
//        }
//        
//        startTimer(m_iButtonFlashTime_ms);
//    }
    //==============================================================================

    
    
    //==============================================================================
    // Set Opacity of one Button
    
    void setAlphaAt(int index, float alpha)
    {
        if (m_iNumButtons > 0)
        {
            m_pcClassButton.getUnchecked(index)->setAlpha(alpha);
        }
    }
    //==============================================================================
    
    
    
    
    //==============================================================================
    // Set Button Flash Time
    
    void setButtonFlashTime_ms(int flashTime)
    {
        m_iButtonFlashTime_ms = flashTime;
    }
    //==============================================================================
    
    
    
    
    //==============================================================================
    // Flash Button for an instant
    
    void flashButton(int index, float finalAlpha)
    {
        if (m_iNumButtons > 0)
        {
            m_fFinalAlpha   =   finalAlpha;
            m_iCurrentButtonIndex = index;
            m_pcClassButton.getUnchecked(index)->setAlpha(1.0f);
        }
        
        m_bFlashArray = false;
        
        startTimer(m_iButtonFlashTime_ms);
        
    }
    //==============================================================================
    
    
    
    
    //==============================================================================
    // Reset Opacity of all buttons to 1.0
    
    void resetAlpha()
    {
        if (m_iNumButtons > 0)
        {
            for (int i=0; i < m_iNumButtons; i++) {
                m_pcClassButton.getUnchecked(i)->setAlpha(1.0f);
            }
        }
    }
    //==============================================================================
    
    
    
    //==============================================================================
    // Set Opacity of all buttons to 0.0
    
    void setZeroAlpha()
    {
        if (m_iNumButtons > 0)
        {
            for (int i=0; i < m_iNumButtons; i++) {
                m_pcClassButton.getUnchecked(i)->setAlpha(0.0f);
            }
        }
    }
    //==============================================================================

    
    
    OwnedArray<CustomShapeButton> m_pcClassButton;
    
    
private:
    
    void timerCallback()
    {
        if (m_bFlashArray)
        {
            for (int i=0; i < m_iNumButtons; i++)
            {
                m_pcClassButton.getUnchecked(i)->setAlpha(0.0f);
            }
        }
        
        else
        {
            m_pcClassButton.getUnchecked(m_iCurrentButtonIndex)->setAlpha(m_fFinalAlpha);
        }
        
        stopTimer();
    }
    
    
    int m_iNumButtons;
    int m_iCurrentButtonIndex;
    int m_fFinalAlpha;
    
    bool m_bFlashArray;
    int m_iButtonFlashTime_ms;
    
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ClassButtonArray)
};


//========================================================================================================================







//====================================== Clock Display Component =========================================================
// Component that displays the metronome
//========================================================================================================================


class ClockDisplayComponent     :   public Button,
                                    public Timer
{
    
public:
    
    ClockDisplayComponent()  :   Button("transportButton")
    {
        
        //    addAndMakeVisible (beatLabel = new Label ("beatLabel", TRANS("0")));
        //    beatLabel->setFont (Font ("Myriad Pro", 14.00f, Font::bold));
        //    beatLabel->setJustificationType (Justification::centred);
        //    beatLabel->setEditable (false, false, false);
        //    beatLabel->setColour (Label::textColourId, Colour (0xFF334455));
        //    beatLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
        //    beatLabel->setColour (TextEditor::highlightColourId, Colour (0x00000000));
        //    beatLabel->toBack();
        
        normalColour        = Colour(0.1f, 0.8f, 1.0f, 0.5f);
        overColour          = Colour(0.1f, 0.8f, 0.75f, 0.5f);
        downColour          = Colour(0.1f, 0.8f, 0.4f, 0.5f);
        downFlashColour     = Colour(0.15f, 1.0f, 1.0f, 0.8f);
        normalFlashColour   = Colour(0.1f, 0.8f, 0.6f, 0.7f);
        offFlashColour      = Colour(0.1f, 0.8f, 0.2f, 0.5f);
        
        m_fFlashTime = 50.0f;   // 50 ms
        
        currentBeatType = 0;
        paintToggle     = false;
    }
    
    
    ~ClockDisplayComponent()
    {
        //    beatLabel = nullptr;
    }
    
    
    void resized()
    {
        //    beatLabel->setBounds(0,0, getWidth(), getHeight());
        ellipse.addEllipse(0.0f, 0.0f, getWidth(), getHeight());
    }
    
    
    
    void paintButton(juce::Graphics &g, bool isMouseOverButton, bool isButtonDown)
    {
        //    g.fillAll(Colours::white);
        
        
        if (globalClock->getMetronomeStatus())
        {
            if (paintToggle)
            {
                if (currentBeatType == 1)
                {
                    g.setColour(downFlashColour);
                }
                
                else
                {
                    g.setColour(normalFlashColour);
                }
            }
            
            else
            {
                g.setColour(offFlashColour);
            }
            
        }
        
        else
        {
            g.setColour (isButtonDown ? normalColour
                         : isMouseOverButton ? overColour
                         : downColour);
        }
        
        
        g.fillPath(ellipse);
    }
    
    
    void flashBeat(int newBeatType)
    {
        currentBeatType = newBeatType;
        paintToggle     = true;
        startTimer(m_fFlashTime);
        repaint();
    }
    
    
    void setFlashTime_ms(float flashTime)
    {
        m_fFlashTime = flashTime;
    }
    
    
    void timerCallback()
    {
        paintToggle = false;
        stopTimer();
        repaint();
    }
    
    
    bool hitTest(int x, int y)
    {
        return ellipse.contains(x,y);
    }
    
    
    
private:
    
    ScopedPointer<Label>    beatLabel;
    
    Colour  normalColour, overColour, downColour;
    Colour  downFlashColour, normalFlashColour, offFlashColour;
    
    Path ellipse;
    
    float m_fFlashTime;
    int currentBeatType;
    bool paintToggle;
    
};

//========================================================================================================================






//======================================= Custom Inc/Dec Buttons =========================================================
// Custom Increment and Decrement Button Component
//========================================================================================================================



class IncDecButton      :       public Slider,
                                public ButtonListener
{
    
public:
    
    IncDecButton(const String& name)
    {
        addAndMakeVisible(incrementButton = new TextButton(name + "inc"));
        incrementButton->addListener(this);
        incrementButton->setButtonText (TRANS("+"));
        
        addAndMakeVisible(decrementButton = new TextButton(name + "dec"));
        decrementButton->addListener(this);
        decrementButton->setButtonText (TRANS("-"));
        
        addAndMakeVisible(valueLabel = new Label(name + "label"));
        valueLabel->setFont (Font ("Myriad Pro", 11.0f, Font::plain));
        valueLabel->setJustificationType (Justification::centred);
        valueLabel->setEditable (false, false, false);
        
        
        m_fMinValue = 0.0f;
        m_fMaxValue = 0.0f;
        m_fIncrement = 0.0f;
        m_fCurrentValue = 0.0f;
    }
    
    
    ~IncDecButton()
    {
        incrementButton = nullptr;
        decrementButton = nullptr;
        valueLabel      = nullptr;
    }
    
    
    void setRange(float minValue, float increment, float maxValue)
    {
        m_fMinValue     =   minValue;
        m_fMaxValue     =   maxValue;
        m_fIncrement    =   increment;
        
        m_fCurrentValue = m_fMinValue;
    }
    
    
    void setBackgroundColours(Colour normalColour, Colour overColour, Colour downColour)
    {
        incrementButton->setColour(TextButton::buttonOnColourId, downColour);
        incrementButton->setColour(TextButton::buttonColourId, normalColour);
        
        decrementButton->setColour(TextButton::buttonOnColourId, downColour);
        decrementButton->setColour(TextButton::buttonColourId, normalColour);
    }
    
    
    void setTextColours(Colour normalColour, Colour textBoxBackgroundColour, Colour textBoxHighlightColour)
    {
        valueLabel->setColour(TextEditor::backgroundColourId, textBoxBackgroundColour);
        valueLabel->setColour(TextEditor::highlightColourId, textBoxHighlightColour);
        valueLabel->setColour(TextEditor::textColourId, normalColour);
    }
    
    
private:
    
    void buttonClicked(Button* buttonThatWasClicked)
    {
        if (buttonThatWasClicked == incrementButton)
        {
            m_fCurrentValue += m_fIncrement;
            
            if (m_fCurrentValue > m_fMaxValue)
            {
                m_fCurrentValue = m_fMaxValue;
            }
            
            setValue(m_fCurrentValue);
            
        }
        
        if (buttonThatWasClicked == decrementButton)
        {
            m_fCurrentValue -= m_fIncrement;
            
            if (m_fCurrentValue < m_fMinValue)
            {
                m_fCurrentValue = m_fMinValue;
            }
            
            setValue(m_fCurrentValue);
        }
        
    }
    

    
    ScopedPointer<TextButton> incrementButton;
    ScopedPointer<TextButton> decrementButton;
    ScopedPointer<Label>    valueLabel;
    
    
    
    void resized()
    {
        incrementButton->setBounds(0, 0, getWidth()/2, getHeight()/2);
        decrementButton->setBounds(0, getHeight()/2, getWidth()/2, getHeight()/2);
        valueLabel->setBounds(getWidth()/2, 0, getWidth()/2, getHeight());
    }
    
    
    void startedDragging()
    {
        
    }
    
    
    void paint (Graphics& g)
    {
        g.fillAll(Colours::white);
    }
    
    
    
    float m_fMaxValue;
    float m_fMinValue;
    float m_fIncrement;
    
    float m_fCurrentValue;
    
    
};


#endif  // BEATSURFACEHEADER_H_INCLUDED
