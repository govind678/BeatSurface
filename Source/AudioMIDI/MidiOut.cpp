/*
  ==============================================================================

    MidiOut.cpp
    Created: 20 Dec 2013 12:10:06am
    Author:  Govinda Ram Pingali

  ==============================================================================
*/


#include "MidiOut.h"


MidiOut::MidiOut()
{
    
#if JUCE_MAC || JUCE_LINUX
    
    // Creating Virtual MIDI Device
    midiOutput = MidiOutput::createNewDevice("from BeatSurface");
    
    if(midiOutput) {
        midiOutput->startBackgroundThread();
    } else {
        DBG("Failed to create virtual MIDI Device");
    }
    
#endif // if JUCE_MAC || JUCE_LINUX
    
    
#if JUCE_WINDOWS
    // TODO: Must figure a 3rd party MIDI library
#endif
    
}


MidiOut::~MidiOut()
{
    
#if JUCE_MAC || JUCE_LINUX
    midiOutput->stopBackgroundThread();
#endif
    
    midiOutput = nullptr;
}


void MidiOut::sendMidiOut(int channel, int noteNumber, int velocity)
{
    if (velocity != 0) {
        midiOutput->sendMessageNow(MidiMessage::noteOn(channel, noteNumber, (uint8)velocity));
    } else {
        midiOutput->sendMessageNow(MidiMessage::noteOff(channel, noteNumber));
    }
}



void MidiOut::makeNoteAndSend(int channel, int noteNumber, int velocity, int duration)
{
    channelNo = channel;
    noteNo = noteNumber;
    
    sendMidiOut(channelNo, noteNo, velocity);
    startTimer(duration);
}



void MidiOut::timerCallback()
{
    sendMidiOut(channelNo, noteNo, 0);
    stopTimer();
}
