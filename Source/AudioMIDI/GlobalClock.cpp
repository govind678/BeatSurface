/*
  ==============================================================================

    GlobalClock.cpp
    Created: 2 Mar 2014 9:33:15pm
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#include "GlobalClock.h"
#include "MainComponent.h"
#include "BeatSurfaceEngine.h"

GlobalClock::GlobalClock()
{
    m_fTempo            =   120.0f;
    m_iNumerator        =   4;
    m_iDenominator      =   4;
    m_iBeat             =   0;
    
    m_fTimeInterval =   float(240000.0f / (m_iDenominator * m_fTempo));
    
    m_bMetronomeStatus  =   false;
    
    mainComponent = NULL;
    
}


GlobalClock::~GlobalClock()
{
    stopClock();
    removeAllActionListeners();
}


void GlobalClock::startClock()
{
    m_bMetronomeStatus  =   true;
    m_iBeat             =   0;
    startTimer(m_fTimeInterval);
    timerCallback();
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
    sendActionMessage("UPDATE_TRANSPORT");
    updateTimer();
}


void GlobalClock::setNumerator(int numerator)
{
    m_iNumerator    =   numerator;
    sendActionMessage("UPDATE_TRANSPORT");
}


void GlobalClock::setDenominator(int denominator)
{
    m_iDenominator  =   denominator;
    updateTimer();
}


void GlobalClock::setQuantizationLevel(int quantization)
{
    m_iQuantizationLevel = quantization;
}


bool GlobalClock::getMetronomeStatus()
{
    return m_bMetronomeStatus;
}

float GlobalClock::getTempo()
{
    return m_fTempo;
}

int GlobalClock::getNumerator()
{
    return m_iNumerator;
}


void GlobalClock::timerCallback()
{
    m_iBeat = (m_iBeat % m_iNumerator) + 1;
    
    if (m_iBeat == 1)
    {
        sendActionMessage("BEAT1");
    }
    
    else
    {
        sendActionMessage("BEAT0");
    }
    
}


void GlobalClock::updateTimer()
{
    m_fTimeInterval =   float(240000.0f / (m_iDenominator * m_fTempo));
    stopTimer();
    startTimer(m_fTimeInterval);
}



float GlobalClock::getTimeInterval()
{
    return m_fTimeInterval;
}


void GlobalClock::setMainComponent(MainComponent *component)
{
    mainComponent = component;
    
    if (mainComponent != NULL) {
        addActionListener(mainComponent);
    }
    
}


void GlobalClock::setBeatSurfaceEngine(BeatSurfaceEngine* engine)
{
    beatSurfaceEngine   = engine;
    
    if(beatSurfaceEngine != NULL)
    {
        addActionListener(beatSurfaceEngine);
    }
    
}