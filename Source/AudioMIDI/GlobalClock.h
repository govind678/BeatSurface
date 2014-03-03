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

class GlobalClock   :   public Timer
{
    
public:
    
    GlobalClock();
    ~GlobalClock();
    
    void startClock();
    void stopClock();
    void setTempo(float newTempo);
    void setMeter(int numerator, int denominator);
    void setQuantizationLevel(int quantization);
    bool getMetronomeStatus();
    
private:
    
    void timerCallback();
    
    float m_fTempo;
    int m_iNumerator;
    int m_iDenominator;
    
    float m_fTimeInterval;
    int m_iQuantizationLevel;
    
    int m_iBeat;
    int m_iBar;
    
    bool m_bMetronomeStatus;
    
};


#endif  // GLOBALCLOCK_H_INCLUDED
