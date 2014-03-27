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
    //--- Audio Device Settings ---//
    sharedAudioDeviceManager->getAudioDeviceSetup(deviceSetup);
    
    m_pcOnsetClassifier = new OnsetClassification();
    m_pcMidiOut         = new MidiOut();
    
    
    m_iCurrentClassIndexToTrain     = 0;
    m_iCurrentClassificationResult  = 0;
    m_fCurrentRMS                   = 0.0f;
    m_iCurrentIndexToPlay           = 0;
    m_bOnsetDetected                = false;
    
    m_eCurrentMode                  = BeatSurfaceBase::IdleMode;
    
    
    transportSource.setSource(nullptr);
    formatManager.registerBasicFormats();
    audioSourcePlayer.setSource(this);
    
    
    m_pfOnsetProbabilities.clear();
    m_ppfOnsetAudio.clear();
    m_piClassLabels.clear();
    
    m_AudioStreamThread.startThread(m_iAudioThreadPriority);

}



AudioStream::~AudioStream()
{
    transportSource.setSource(nullptr);
    currentAudioFileSource  =   nullptr;
    audioSourcePlayer.setSource(0);
    
    m_pcOnsetClassifier     = nullptr;
    m_pcMidiOut             = nullptr;
    
    
    m_pfOnsetProbabilities.clear();
    m_ppfOnsetAudio.clear(true);
    m_piClassLabels.clear();
    
    sharedAudioDeviceManager->removeAudioCallback(this);
    
    m_AudioStreamThread.stopThread(m_iStopThreadTimeOut_ms);
}




//====================================== Process Methods ============================================



//==============================================================================
// AudioIODevice Callback Methods
// !!! Running on Audio Thread
//==============================================================================

void AudioStream::audioDeviceAboutToStart(AudioIODevice* device)
{
    m_pcOnsetClassifier->initialize(device->getCurrentBufferSizeSamples(), device->getCurrentSampleRate());
    audioSourcePlayer.audioDeviceAboutToStart (device);
}



void AudioStream::audioDeviceIOCallback( const float** inputChannelData,
                                        int totalNumInputChannels,
                                        float** outputChannelData,
                                        int totalNumOutputChannels,
                                        int blockSize)
{
    
//    
//    if (m_eCurrentMode == BeatSurfaceBase::TrainMode)
//    {
//        if(m_pcOnsetClassifier->detectOnset(inputChannelData))
//        {
//            m_pcOnsetClassifier->train(m_iCurrentClassIndexToTrain);
//            guiUpdater->displayTrainingOnset(m_iCurrentClassIndexToTrain);
//            
//            m_ppfOnsetAudio.add(new AudioSampleBuffer(1, blockSize));
//            
//            for (int sample = 0; sample < blockSize; sample++)
//            {
//                m_ppfOnsetAudio.getLast()->copyFrom(0, 0, inputChannelData[0], blockSize);
//            }
//            
//            m_piClassLabels.add(m_iCurrentClassIndexToTrain);
//            
//            guiUpdater->drawSpectralCentroid(m_pcOnsetClassifier->getCurrentSpectralCentroid());
//            guiUpdater->drawSpectrum(m_pcOnsetClassifier->getCurrentSpectrum());
//        }
//    }
//    
//    
//
//    
//    
//    
//    else if (m_eCurrentMode == BeatSurfaceBase::PlayMode)
//    {
//        m_fCurrentRMS = m_pcOnsetClassifier->rootMeanSquare(inputChannelData);
//        guiUpdater->drawWaveformWithSample(m_fCurrentRMS);
//
//        
//        if (m_pcOnsetClassifier->detectOnsets(inputChannelData))
//        {
//            if(m_pcOnsetClassifier->getNumClasses() > 0)
//            {
//                m_iCurrentClassificationResult  =  m_pcOnsetClassifier->classify();
//                guiUpdater->displayPlayingOnset(m_iCurrentClassificationResult);
//                m_pcMidiOut->makeNoteAndSend(1, m_iCurrentClassificationResult + 35, int((m_fCurrentRMS * 120.0f) / 0.6f), 250);
//            }
//            
//            guiUpdater->drawSpectralCentroid(m_pcOnsetClassifier->getCurrentSpectralCentroid());
//            guiUpdater->drawSpectrum(m_pcOnsetClassifier->getCurrentSpectrum());
//        }
//    }
//    
//    
//    else if (BeatSurfaceBase::eCurrentMode == BeatSurfaceBase::IdleMode)
//    {
////        guiUpdater->drawWaveformWithSample(m_pcOnsetClassifier->rootMeanSquare(inputChannelData));
//        
//        for (int channel = 0; channel < totalNumOutputChannels; channel++)
//        {
//            if (m_ppfOnsetAudio.size() > 0)
//            {
//                for (int sample = 0; sample < blockSize; sample++)
//                {
//                    outputChannelData[channel][sample] = m_ppfOnsetAudio.getUnchecked(m_iCurrentIndexToPlay)->getSampleData(0)[sample];
//                }
//            }
//        }
//        
//        
//    }
    
    if (m_eCurrentMode == BeatSurfaceBase::FileTrainMode)
    {
        audioSourcePlayer.audioDeviceIOCallback (inputChannelData,
                                                 totalNumInputChannels,
                                                 outputChannelData,
                                                 totalNumOutputChannels,
                                                 blockSize);
    }
    
    
    else if (m_eCurrentMode != BeatSurfaceBase::IdleMode)
    {
        if (m_bOnsetDetected)
        {
            if (m_eCurrentMode == BeatSurfaceBase::TrainMode)
            {
                m_pcOnsetClassifier->trainAtClass(inputChannelData, m_iCurrentClassIndexToTrain);
                guiUpdater->displayTrainingOnset();
            }
        
        
            else if (m_eCurrentMode == BeatSurfaceBase::PlayMode)
            {
                m_iCurrentClassificationResult = m_pcOnsetClassifier->classify(inputChannelData);
                m_pcMidiOut->makeNoteAndSend(1, m_iCurrentClassificationResult + 35, 100, 250);
                guiUpdater->displayPlayingOnset();
            }
            
            m_bOnsetDetected = false;
        }
    
    
        if (m_pcOnsetClassifier->detectOnset(inputChannelData))
        {
            m_bOnsetDetected = true;
        }
    }
    
}


void AudioStream::audioDeviceStopped()
{
    audioSourcePlayer.audioDeviceStopped();
}



//==============================================================================
// Audio File Player Callback Methods
// !!! Running on Audio Thread
//==============================================================================

void AudioStream::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    m_pcOnsetClassifier->initialize(samplesPerBlockExpected, sampleRate);
}


void AudioStream::releaseResources()
{
    stopPlayback();
    transportSource.releaseResources();
}


void AudioStream::getNextAudioBlock(const AudioSourceChannelInfo &audioSourceChannelInfo)
{
    transportSource.getNextAudioBlock(audioSourceChannelInfo);
    processAudioBlock(audioSourceChannelInfo.buffer->getArrayOfReadPointers(), audioSourceChannelInfo.numSamples);
}



//==============================================================================
// Process Block for File Training
// !!!
//==============================================================================

void AudioStream::processAudioBlock(const float **audioBuffer, int numSamples)
{
    if (m_bOnsetDetected == true)
    {
        if (m_eCurrentMode == BeatSurfaceBase::FileTrainMode)
        {
            m_pcOnsetClassifier->trainAtClass(audioBuffer, m_iCurrentClassIndexToTrain);
        }
        
        else if (m_eCurrentMode == BeatSurfaceBase::PlayMode)
        {
            m_iCurrentClassificationResult = m_pcOnsetClassifier->classify(audioBuffer);
            m_pcMidiOut->makeNoteAndSend(1, m_iCurrentClassificationResult + 35, 100, 250);
            guiUpdater->displayPlayingOnset();
        }
        
        m_bOnsetDetected = false;
    }
    
    if (m_pcOnsetClassifier->detectOnset(audioBuffer))
    {
        m_bOnsetDetected = true;
    }
    
}




void AudioStream::loadAudioFileToTrain(File audioTrainFile)
{
    transportSource.stop();
    transportSource.setSource(nullptr);
    
    currentAudioFileSource  =   nullptr;
    
    AudioFormatReader* reader = formatManager.createReaderFor(audioTrainFile);
    
    
    if (reader != nullptr)
    {
        currentAudioFileSource = new AudioFormatReaderSource (reader, true);
        currentAudioFileSource->setLooping(false);
        transportSource.setSource(currentAudioFileSource, 32768, &m_AudioStreamThread, reader->sampleRate);
    }
}




//==============================================================================
// Transport Control Methods
//==============================================================================

void AudioStream::startPlayback()
{
    transportSource.setPosition(0);
    transportSource.start();
}

void AudioStream::stopPlayback()
{
    transportSource.stop();
}


bool AudioStream::isPlaying()
{
    return transportSource.isPlaying();
}





//==============================================================================
// Onset Classification Methods
// Interface methods to pass messages between GUI and Backend
//==============================================================================

void AudioStream::setParameter(BeatSurfaceBase::ParameterID parameterID, double value)
{
    switch (parameterID)
    {
        case BeatSurfaceBase::VelocitySensitivity :
            m_pcOnsetClassifier->setVelocitySensitivity(value);
            break;
            
        case BeatSurfaceBase::DecayTimeSensitivity :
            m_pcOnsetClassifier->setDecayTimeSensitivity(value);
            break;
            
        default:
            break;
    }
}


void AudioStream::performTraining()
{
    m_pcOnsetClassifier->performTraining();
}


void AudioStream::addClass()
{
    m_pcOnsetClassifier->addClass();
}

void AudioStream::deleteClass(int index)
{
    m_pcOnsetClassifier->deleteClass(index);
}

void AudioStream::saveTraining(File trainingFile)
{
    m_pcOnsetClassifier->saveTraining(trainingFile);
}

void AudioStream::loadTraining(File trainingFile)
{
    m_pcOnsetClassifier->loadTraining(trainingFile);
    guiUpdater->updateGUIOnLoadTraining();
}





//==============================================================================
// Generic Methods
//==============================================================================

void AudioStream::setSystemMode(BeatSurfaceBase::SystemMode newMode)
{
    m_eCurrentMode = newMode;
}

void AudioStream::setCurrentClassToTrain(int newClassIndex)
{
    m_iCurrentClassIndexToTrain = newClassIndex;
}


void AudioStream::playTrainingDataAtClass(int classIndex)
{
////    m_iCurrentIndexToPlay   =   m_piClassLabels.indexOf(classIndex);
//    m_iCurrentIndexToPlay       =   classIndex - 1;
//    
//    sharedAudioDeviceManager->addAudioCallback(this);
//    startTimer((deviceSetup.bufferSize / deviceSetup.sampleRate) * 1000.0f);
////    startTimer(50);
}


void AudioStream::audioDeviceSettingsChanged()
{
    sharedAudioDeviceManager->getAudioDeviceSetup(deviceSetup);
    m_pcOnsetClassifier->initialize(deviceSetup.bufferSize, deviceSetup.sampleRate);
}



//==============================================================================
// Get Interface Methods
//==============================================================================

int AudioStream::getNumClasses()
{
    return (m_pcOnsetClassifier->getNumClasses());
}

int AudioStream::getCurrentTrainingClass()
{
    return m_iCurrentClassIndexToTrain;
}

double AudioStream::getParameter(BeatSurfaceBase::ParameterID parameterID)
{
    switch (parameterID)
    {
        case BeatSurfaceBase::VelocitySensitivity :
            return (m_pcOnsetClassifier->getVelocitySensitivity());
            break;
            
        case BeatSurfaceBase::DecayTimeSensitivity :
            return (m_pcOnsetClassifier->getDecayTimeSensitivity());
            break;
            
        default:
            return 0.0f;
            break;
    }
}

double AudioStream::getClassificationConfidence()
{
    return m_pcOnsetClassifier->getClassificationConfidence();
}

int AudioStream::getCurrentObservation()
{
    return m_pcOnsetClassifier->getCurrentObservation();
}

String AudioStream::getCurrentFeatureVector()
{
    return m_pcOnsetClassifier->getCurrentFeatureVector();
}

int AudioStream::getCurrentClassificationResult()
{
    return m_iCurrentClassificationResult;
}

//==============================================================================
// Timer Callback Methods
//==============================================================================
void AudioStream::timerCallback()
{
//    sharedAudioDeviceManager->removeAudioCallback(this);
    stopTimer();
}
