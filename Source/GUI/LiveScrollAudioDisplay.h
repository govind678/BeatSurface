/*
  ==============================================================================

    LiveScrollDisplay.h
    Created: 14 Feb 2014 3:22:16am
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#ifndef LIVESCROLLDISPLAY_H_INCLUDED
#define LIVESCROLLDISPLAY_H_INCLUDED


#include "BeatSurfaceHeader.h"

//==============================================================================

class LiveScrollAudioDisplay  :      public Component
{
public:
    
        LiveScrollAudioDisplay()     : nextSample (0), subSample (0), accumulator (0)
        {
            setOpaque (true);
            clear();
        }
    
        //==============================================================================
    
    
        void pushSample (const float newSample)
        {
            accumulator += (newSample * 100.0f);
    
            if (subSample == 0)
            {
                const int inputSamplesPerPixel = 1;
        
                samples[nextSample] = accumulator / inputSamplesPerPixel;
                nextSample = (nextSample + 1) % numElementsInArray (samples);
                subSample = inputSamplesPerPixel;
                accumulator = 0;
            }
    
            else
            {
                --subSample;
            }
        }


        int miChannelNoToDraw = 0;


private:

        float samples[512];
        int nextSample, subSample;
        float accumulator;
        float midY;

        void clear()
        {
            zeromem (samples, sizeof (samples));
            accumulator = 0;
            subSample = 0;
        }


        void paint (Graphics& g) override
        {
            g.fillAll (Colour (0xAA1A1A1A));
    
            int samplesAgo = (nextSample + numElementsInArray (samples) - 1);

            RectangleList<float> waveform;

            for (int x = jmin (getWidth(), (int) numElementsInArray (samples)); --x >= 0;)
            {
                const float sampleSize = midY * samples [samplesAgo-- % numElementsInArray (samples)];
                waveform.addWithoutMerging (Rectangle<float> ((float) x, midY - sampleSize, 1.0f, sampleSize * 2.0f));
            }

            g.setColour (Colours::blue);
            g.fillRectList (waveform);
        }


        void resized()
        {
            midY = getHeight() / 2;
//            midY = 0;
        }


JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LiveScrollingAudioDisplay);

};


#endif  // LIVESCROLLDISPLAY_H_INCLUDED
