/*
  ==============================================================================

    LiveScrollDisplay.h
    Created: 4 Mar 2014 1:13:41am
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#ifndef LIVESCROLLDISPLAY_H_INCLUDED
#define LIVESCROLLDISPLAY_H_INCLUDED

#include "BeatSurfaceHeader.h"
#include <stdio.h>
#include <vector>






class LiveScrollDisplay     :   public Component
{
    
public:
    
    LiveScrollDisplay(int displayType)
    {
        m_iSampleIndex              =   0;
        m_iNumScrollBoxes           =   0;
        m_fScrollLineSpacing        =   0.0f;
        m_fSubDivisionLineSpacing   =   0.0f;
        m_fPixelsPerBlock           =   0;
        m_fDataScaling              =   40.0f;
        m_iBlockIndex               =   0;
        m_iPointToDisplayInTime     =   0.0f;
        m_iDisplayType              =   displayType;
        m_iWidth                    =   0;
        
        sharedAudioDeviceManager->getAudioDeviceSetup(deviceSetup);
        
        m_fSamples.clear();
    }
    
    
    ~LiveScrollDisplay()
    {
        
    }
    
    
    void getSampleToDraw(float sample)
    {
//        if (m_fPixelsPerBlock > 0)
//        {
//            for (int i=m_fPixelsPerBlock - 1; i > 0; i--)
//            {
//                m_fSamples.move(i, i+1);
//            }
//            
////            m_fSamples.insert(0, (sample * m_fDataScaling) + m_iBaseLine);
//            m_fSamples.insert(0, sample);
//        }
        
        for (int i=1; i < m_iWidth; i++)
        {
            m_fSamples.move(i, i-1);
        }
        
        m_fSamples.insert(m_iWidth-1, sample);
//        m_iSampleDrawIndex = (m_iSampleDrawIndex + 1) % m_iWidth;
        
        
        
        repaint();
    }
    
    
    void setArrayToDraw(std::vector<float> data)
    {
        int numSamples = data.size();
        
        m_fSamples.clear();
        
        m_fSamples.resize(getWidth());
        
        if (getWidth() > numSamples)
        {
            int iterator = int((getWidth() / numSamples) + 0.5f);
            
            for (int i = 0; i < numSamples; i++)
            {
                for (int j = 0; j < iterator; j++)
                {
                    m_fSamples.insert((i*iterator) + j, data[i]);
                }
            }
        }
        
        else if (getWidth() < numSamples)
        {
            int iterator = int((numSamples / getWidth()) + 0.5f);
            
            for (int i = 0; i < getWidth(); i++)
            {
                for (int j=0; j < iterator; j++)
                {
                    m_fSamples.insert(i, data[(i*iterator) + j]);
                }
            }
        }
        
        else
        {
            for (int i = 0; i < numSamples; i++)
            {
                m_fSamples.insert(i, data[i]);
            }
        }
        
        repaint();
    }
    
    
    
    void setPointToDisplayInTime(float value)
    {
        m_iPointToDisplayInTime = int(value + 0.5);
    }
    
    
    
    void updateScrollLineSpacing()
    {
        m_iNumScrollBoxes           =   int (float((m_iWindowLength_s * globalClock->getTempo()) / 60.0f) + 0.5);
        m_fScrollLineSpacing        =   float(getWidth()) / float(m_iNumScrollBoxes);
        m_fSubDivisionLineSpacing   =   float(m_fScrollLineSpacing / globalClock->getNumerator());
        
        m_fPixelsPerBlock           =   int( ((deviceSetup.bufferSize * getWidth()) / (deviceSetup.sampleRate * m_iWindowLength_s)) + 0.5);
        
        mainScrollLines.clear();
        subDivisionScrollLines.clear();
        
        int index = 0;
        for (int i=0; i < m_iNumScrollBoxes; i++)
        {
            mainScrollLines.addRectangle((i * m_fScrollLineSpacing), 0, m_fScrollLineSpacing, getHeight());
            
            for (int j=0; j < globalClock->getNumerator(); j++)
            {
                subDivisionScrollLines.addRectangle((index * m_fSubDivisionLineSpacing), 0, m_fSubDivisionLineSpacing, getHeight());
                index++;
            }
        }
        
        
        
        // Clearing Old Samples and Resizing Array
        m_fSamples.clear();
        
        m_fSamples.resize(getWidth());
        
    }
    
    
    void setDataScaling(float scaling)
    {
        m_fDataScaling = scaling;
    }
    
    
    void setOrientation(bool orientation)
    {
        if (!orientation)
        {
            m_iBaseLine = getHeight() / 2;
        }
        
        else
        {
            m_iBaseLine = 0;
            m_fDataScaling = m_fDataScaling * 2;
        }
    }
    
    
    
private:
    
    int     m_iSampleIndex;
    int     m_iNumScrollBoxes;
    float   m_fScrollLineSpacing;
    float   m_fSubDivisionLineSpacing;
    float   m_fPixelsPerBlock;
    float   m_fDataScaling;
    bool    m_bOrientation;             // 'Zero': Center, '1': Bottom
    int     m_iBaseLine;
    int     m_iBlockIndex;
    int     m_iDisplayType;
    int     m_iPointToDisplayInTime;
    int     m_iWidth;
    
    static const int m_iWindowLength_s     = 4;
    
    Path mainScrollLines, subDivisionScrollLines;
    
    RectangleList<float> waveform;
    
    Array<float> m_fSamples;
    
    
    AudioDeviceManager::AudioDeviceSetup    deviceSetup;
    
    
    void paint(Graphics& g)
    {
        waveform.clear();
        
        g.fillAll (Colour (0xBB1A1A1A));
        
        
        
        for (int pixel = 0; pixel < getWidth(); pixel++)
        {
            waveform.addWithoutMerging(Rectangle<float> (float(pixel),
                                                         getHeight() - (m_fSamples[pixel] * getHeight()) ,
                                                         m_fPixelsPerBlock,
                                                         getHeight() ));
            
//            std::cout << "P: " << pixel << " " << m_fSamples[pixel] << std::endl;
        }
        
        if (m_iDisplayType == 1)
        {
            Rectangle<float> pointToDrawInTime(m_iPointToDisplayInTime, getHeight()/2, 5, getHeight());
            g.setColour(Colours::green);
            g.fillRect(pointToDrawInTime);
            
            
            g.setColour(Colours::red);
        }
        
        
        else if (m_iDisplayType == 0)
        {
            g.setColour(Colour (0x883B3B3B));
            g.strokePath(subDivisionScrollLines, PathStrokeType(1.0f));
            
            g.setColour(Colour(0x88676767));
            g.strokePath(mainScrollLines, PathStrokeType(2.0f));
            
            g.setColour(Colours::blue);
            
        }
        
        
        g.fillRectList (waveform);
        
    }
    
    
    void resized()
    {
        updateScrollLineSpacing();
        m_iWidth    =   getWidth();
    }
    
    
};



#endif  // LIVESCROLLDISPLAY_H_INCLUDED
