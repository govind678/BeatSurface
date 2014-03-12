/*
  ==============================================================================

    GlobalClock.h
    Created: 2 Mar 2014 9:33:15pm
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#ifndef GLOBALCLOCK_H_INCLUDED
#define GLOBALCLOCK_H_INCLUDED

#include "JuceHeader.h"

class MainComponent;
class BeatSurfaceEngine;

class GlobalClock   :   public Timer,
                        public ActionBroadcaster
{
    
public:
    
    GlobalClock();
    ~GlobalClock();
    
    void startClock();
    void stopClock();
    void setTempo(float newTempo);
    void setNumerator(int numerator);
    void setDenominator(int denominator);
    void setQuantizationLevel(int quantization);
    
    bool getMetronomeStatus();
    float getTempo();
    int getNumerator();
    float getTimeInterval();
    
    void timerCallback();
    
    void setMainComponent(MainComponent* component);
    void setBeatSurfaceEngine(BeatSurfaceEngine* engine);
    
private:
    
    void updateTimer();
    
    float m_fTempo;
    int m_iNumerator;
    int m_iDenominator;
    
    float m_fTimeInterval;
    int m_iQuantizationLevel;
    
    int m_iBeat;
    int m_iBar;
    
    bool m_bMetronomeStatus;
    
    MainComponent* mainComponent;
    BeatSurfaceEngine* beatSurfaceEngine;
    
};


#endif  // GLOBALCLOCK_H_INCLUDED
