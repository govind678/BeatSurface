/*
  ==============================================================================

    MidiOut.h
    Created: 20 Dec 2013 12:10:06am
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#ifndef MIDIOUT_H_INCLUDED
#define MIDIOUT_H_INCLUDED

#include "JuceHeader.h"


class MidiOut :     public MidiOutput,
                    public MidiMessage,
                    private Timer

{
    
public:
    
    MidiOut();
    ~MidiOut();
    
    ScopedPointer <MidiOutput> midiOutput;
    
    void sendMidiOut(int channel, int noteNumber, int velocity);
    
    void makeNoteAndSend(int channel, int noteNumber, int velocity, int duration);
    
private:
    
    MidiMessage outMessage;
    
    int channelNo;
    int noteNo;
    
    void timerCallback();
};




#endif  // MIDIOUT_H_INCLUDED
