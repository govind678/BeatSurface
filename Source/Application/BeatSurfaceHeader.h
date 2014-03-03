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


extern ScopedPointer<ApplicationCommandManager> commandManager;
extern ScopedPointer<AudioDeviceManager> sharedAudioDeviceManager;
extern ScopedPointer<GlobalClock>   globalClock;
extern ScopedPointer<GUIUpdater>    guiUpdater;


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
        m_iButtonFlashTime_ms = 10;    // Default 10 ms
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
    
    void flashArray(float* alphaValues)
    {
        if (m_iNumButtons > 0)
        {
            for (int i=0; i < m_iNumButtons; i++) {
                m_pcClassButton.getUnchecked(i)->setAlpha(alphaValues[i]);
            }
            
            m_bFlashArray = true;
        }
        
        startTimer(m_iButtonFlashTime_ms);
    }
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
            for (int i=0; i < m_iNumButtons; i++) {
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



#endif  // BEATSURFACEHEADER_H_INCLUDED
