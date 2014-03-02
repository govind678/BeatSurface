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


extern ScopedPointer<ApplicationCommandManager> commandManager;
extern ScopedPointer<AudioDeviceManager> sharedAudioDeviceManager;

class BeatSurfaceBase
{
    
public:
    
    
    //--- Document Window Init Size ---//
    static const int iDocumentInitWidth                 = 850;
    static const int iDocumentInitHeight                = 550;
    
    
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
    
    
private:
    
    
//    JUCE_DECLARE_NON_COPYABLE (BeatSurfaceBase);
};



#endif  // BEATSURFACEHEADER_H_INCLUDED
