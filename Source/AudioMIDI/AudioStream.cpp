/*
  ==============================================================================

    AudioStream.cpp
    Created: 16 Nov 2013 11:22:57am
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#include "AudioStream.h"

AudioStream::AudioStream() : m_AudioStreamThread("Audio IO")
{
    
    m_AudioStreamThread.startThread(m_iAudioThreadPriority);

    
    
    //--- Audio Device Settings ---//
    sharedAudioDeviceManager->getAudioDeviceSetup(deviceSetup);
    
    m_pcOnsetClassifier = new OnsetClassification();
    
    deviceSettings.fSampleRate  =   float(deviceSetup.sampleRate);
    deviceSettings.iBufferSize  =   deviceSetup.bufferSize;
    deviceSettings.iNumChannels =   deviceSetup.inputChannels.toInteger();
    
    m_pcOnsetClassifier->setAudioDeviceSettings(deviceSettings);
    
    
    m_eCurrentMode = BeatSurfaceBase::IdleMode;
    
    m_iCurrentClassIndexToTrain = 0;
    
    m_pcMidiOut = new MidiOut;
    
    m_pfOnsetProbabilities.clear();
    m_ppfOnsetAudio.clear();

}



AudioStream::~AudioStream()
{
    m_pcOnsetClassifier     = nullptr;
    m_pcMidiOut             = nullptr;
    
    m_pfOnsetProbabilities.clear();
    
    m_ppfOnsetAudio.clear(true);
    m_piClassLabels.clear();
    
    sharedAudioDeviceManager->removeAudioCallback(this);
    
    m_AudioStreamThread.stopThread(m_iStopThreadTimeOut_ms);
}



void AudioStream::audioDeviceAboutToStart(AudioIODevice* device)
{
    
}



void AudioStream::audioDeviceStopped()
{
    
}




//==============================================================================
// Process Block
// !!! Running on Audio Thread
//==============================================================================

void AudioStream::audioDeviceIOCallback( const float** inputChannelData,
                                        int totalNumInputChannels,
                                        float** outputChannelData,
                                        int totalNumOutputChannels,
                                        int blockSize)
{
    
    if (m_eCurrentMode == BeatSurfaceBase::TrainMode)
    {
        if(m_pcOnsetClassifier->detectOnsets(inputChannelData))
        {
            m_pcOnsetClassifier->train(m_iCurrentClassIndexToTrain);
            guiUpdater->displayTrainingOnset(m_iCurrentClassIndexToTrain);
            
            m_ppfOnsetAudio.add(new AudioSampleBuffer(1, blockSize));
            
            for (int sample = 0; sample < blockSize; sample++)
            {
                m_ppfOnsetAudio.getLast()->copyFrom(0, 0, inputChannelData[0], blockSize);
            }
            
            m_piClassLabels.add(m_iCurrentClassIndexToTrain);
            
            guiUpdater->drawSpectralCentroid(m_pcOnsetClassifier->getCurrentSpectralCentroid());
            guiUpdater->drawSpectrum(m_pcOnsetClassifier->getCurrentSpectrum());
        }
    }
    
    
    else if (m_eCurrentMode == BeatSurfaceBase::PlayMode)
    {
        guiUpdater->DrawWaveform = true;
        m_fCurrentRMS = m_pcOnsetClassifier->rootMeanSquare(inputChannelData);
        guiUpdater->drawWaveformWithSample(m_fCurrentRMS);

        
        if (m_pcOnsetClassifier->detectOnsets(inputChannelData))
        {
            if(m_pcOnsetClassifier->getNumClasses() > 0)
            {
                m_iCurrentClassificationResult  =  m_pcOnsetClassifier->classify();
                guiUpdater->displayOnsetProbabilities(m_pcOnsetClassifier->getCurrentProbabilityEstimates());
                m_pcMidiOut->makeNoteAndSend(1, m_iCurrentClassificationResult + 35, int((m_fCurrentRMS * 120.0f) / 0.6f), 250);
            }
            
            guiUpdater->drawSpectralCentroid(m_pcOnsetClassifier->getCurrentSpectralCentroid());
            guiUpdater->drawSpectrum(m_pcOnsetClassifier->getCurrentSpectrum());
        }
    }
    
    
    else if (m_eCurrentMode == BeatSurfaceBase::IdleMode)
    {
//        guiUpdater->drawWaveformWithSample(m_pcOnsetClassifier->rootMeanSquare(inputChannelData));
        
        for (int channel = 0; channel < totalNumOutputChannels; channel++)
        {
            if (m_ppfOnsetAudio.size() > 0)
            {
                for (int sample = 0; sample < blockSize; sample++)
                {
                    outputChannelData[channel][sample] = m_ppfOnsetAudio.getUnchecked(m_iCurrentIndexToPlay)->getSampleData(0)[sample];
                }
            }
        }
        
        
    }
    
    
    
}



void AudioStream::setMode(BeatSurfaceBase::SystemMode newMode)
{
    m_eCurrentMode = newMode;
}


BeatSurfaceBase::SystemMode AudioStream::getMode()
{
    return m_eCurrentMode;
}


void AudioStream::setClassIndexToTrain(int newClassIndex)
{
    m_iCurrentClassIndexToTrain = newClassIndex;
}


void AudioStream::timerCallback()
{
    sharedAudioDeviceManager->removeAudioCallback(this);
    stopTimer();
}


void AudioStream::playTrainingDataAtClass(int classIndex)
{
//    m_iCurrentIndexToPlay   =   m_piClassLabels.indexOf(classIndex);
    m_iCurrentIndexToPlay       =   classIndex - 1;
    
    sharedAudioDeviceManager->addAudioCallback(this);
    startTimer((deviceSetup.bufferSize / deviceSetup.sampleRate) * 1000.0f);
//    startTimer(50);
}



void AudioStream::audioDeviceSettingsChanged()
{
    sharedAudioDeviceManager->getAudioDeviceSetup(deviceSetup);
    deviceSettings.fSampleRate  =   float(deviceSetup.sampleRate);
    deviceSettings.iBufferSize  =   deviceSetup.bufferSize;
    deviceSettings.iNumChannels =   deviceSetup.inputChannels.toInteger();
    
    m_pcOnsetClassifier->setAudioDeviceSettings(deviceSettings);
}
