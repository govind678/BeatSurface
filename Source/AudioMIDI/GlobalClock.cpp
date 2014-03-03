/*
  ==============================================================================

    GlobalClock.cpp
    Created: 2 Mar 2014 9:33:15pm
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#include "GlobalClock.h"

GlobalClock::GlobalClock()
{
    m_fTempo            =   120.0f;
    m_iNumerator        =   4;
    m_iDenominator      =   4;
    m_iBeat             =   0;
    
    m_fTimeInterval =   float(30000.0 / m_fTempo);
    
    m_bMetronomeStatus  =   false;
}


GlobalClock::~GlobalClock()
{
    
}


void GlobalClock::startClock()
{
    m_bMetronomeStatus  =   true;
    m_iBeat             = 0;
    startTimer(m_fTimeInterval);
}


void GlobalClock::stopClock()
{
    m_bMetronomeStatus  =   false;
    stopTimer();
    m_iBeat             =   0;
}


void GlobalClock::setTempo(float newTempo)
{
    m_fTempo        =   newTempo;
    m_fTimeInterval =   float(30000.0 / m_fTempo);
}


void GlobalClock::setMeter(int numerator, int denominator)
{
    m_iNumerator    =   numerator;
    m_iDenominator  =   denominator;
    
}


void GlobalClock::setQuantizationLevel(int quantization)
{
    m_iQuantizationLevel = quantization;
}


bool GlobalClock::getMetronomeStatus()
{
    return m_bMetronomeStatus;
}


void GlobalClock::timerCallback()
{
    m_iBeat = (m_iBeat % m_iDenominator) + 1;
    std::cout << "Beat: " << m_iBeat << std::endl;
}
