/*
  ==============================================================================

    AudioLiveScrollDisplay.h
    Created: 14 Feb 2014 3:22:16am
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#ifndef AUDIOLIVESCROLLDISPLAY_H_INCLUDED
#define AUDIOLIVESCROLLDISPLAY_H_INCLUDED


#include "BeatSurfaceHeader.h"

//==============================================================================
/* This component scrolls a continuous waveform showing the audio that's
 coming into whatever audio inputs this object is connected to.
 */

class LiveScrollingAudioDisplay  :      public AudioIODeviceCallback,
                                        public Component,
                                        private Timer
{
public:
    
        LiveScrollingAudioDisplay()     : nextSample (0), subSample (0), accumulator (0)
        {
            setOpaque (true);
            clear();
        
            startTimer (1000 / 75); // use a timer to keep repainting this component
        }
    
        //==============================================================================
    
        void audioDeviceAboutToStart (AudioIODevice*) override
        {
            clear();
        }

        void audioDeviceStopped() override
        {
            clear();
        }


        void audioDeviceIOCallback (const float** inputChannelData, int numInputChannels,
                                    float** outputChannelData, int numOutputChannels,
                                    int numSamples) override
        {

            for (int i = 0; i < numSamples; ++i)
            {
                float inputSample = inputChannelData[miChannelNoToDraw][i];
    
//                for (int chan = 0; chan < numInputChannels; ++chan)
//                {
//                    if (inputChannelData[chan] != nullptr)
//                    {
//                        //inputSample += std::abs (inputChannelData[chan][i]);  // find the sum of all the channels
//                    }
//                }
            
                pushSample (10.0f * inputSample); // boost the level to make it more easily visible.
            }



            // We need to clear the output buffers before returning, in case they're full of junk..
            for (int j = 0; j < numOutputChannels; ++j)
            {
                if (outputChannelData[j] != nullptr)
                {
                    zeromem (outputChannelData[j], sizeof (float) * (size_t) numSamples);
                }
            }

        }


        int miChannelNoToDraw = 0;


private:

        float samples[1024];
        int nextSample, subSample;
        float accumulator;

        void clear()
        {
            zeromem (samples, sizeof (samples));
            accumulator = 0;
            subSample = 0;
        }


        void paint (Graphics& g) override
        {
            g.fillAll (Colour (0xAA1A1A1A));

            const float midY = getHeight() * 0.5f;
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


        void timerCallback() override
        {
            repaint();
        }


        void pushSample (const float newSample)
        {
            accumulator += newSample;
    
            if (subSample == 0)
            {
                const int inputSamplesPerPixel = 200;
        
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


JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LiveScrollingAudioDisplay);

};


#endif  // AUDIOLIVESCROLLDISPLAY_H_INCLUDED
