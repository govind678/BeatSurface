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
    m_pcAudioMixer      = new AudioMixerPlayer;
    
    m_psMidiNoteData.clear();
    m_pbAudioInclude.clear();
    
    m_iCurrentClassIndexToTrain     = 0;
    m_iCurrentClassificationResult  = 0;
    m_fCurrentRMS                   = 0.0f;
    m_iCurrentIndexToPlay           = 0;
    m_bOnsetDetected                = false;
    m_bMemoryInitialized            = false;
    m_eCurrentMode                  = BeatSurfaceBase::IdleMode;
    
    
//    transportSource.setSource(nullptr);
//    formatManager.registerBasicFormats();
//    audioSourcePlayer.setSource(this);
    
    
    m_pfOnsetProbabilities.clear();
//    m_ppfOnsetAudio.clear();
    m_piClassLabels.clear();
    m_psAudioFilePaths.clear();
    
    m_AudioStreamThread.startThread(m_iAudioThreadPriority);
    
    
    for (int i=0; i < 16; i++)
    {
        m_psAudioFilePaths.add("");
    }

}



AudioStream::~AudioStream()
{
//    transportSource.setSource(nullptr);
//    currentAudioFileSource  =   nullptr;
//    audioSourcePlayer.setSource(0);
    
    m_pcOnsetClassifier     = nullptr;
    m_pcMidiOut             = nullptr;
    m_pcAudioMixer          = nullptr;
    
    m_psMidiNoteData.clear();
    m_pbAudioInclude.clear();
    
    m_psAudioFilePaths.clear();
    
    m_pfOnsetProbabilities.clear();
//    m_ppfOnsetAudio.clear(true);
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
//    audioSourcePlayer.audioDeviceAboutToStart (device);
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
//        audioSourcePlayer.audioDeviceIOCallback (inputChannelData,
//                                                 totalNumInputChannels,
//                                                 outputChannelData,
//                                                 totalNumOutputChannels,
//                                                 blockSize);
    }
    
    
    else if (m_eCurrentMode != BeatSurfaceBase::IdleMode)
    {
        if (m_bOnsetDetected)
        {
            if (m_eCurrentMode == BeatSurfaceBase::TrainMode)
            {
                m_pcOnsetClassifier->trainAtClass(inputChannelData, m_iCurrentClassIndexToTrain);
                guiUpdater->displayTrainingOnset();
                guiUpdater->drawSpectrum();
            }
        
        
            else if (m_eCurrentMode == BeatSurfaceBase::PlayMode)
            {
                m_iCurrentClassificationResult = m_pcOnsetClassifier->classify(inputChannelData);
                
                if ((m_iCurrentClassificationResult > 0))
                {
                    
                    if (m_bMemoryInitialized)
                    {
                        // Trigger MIDI
                        if(m_iCurrentClassificationResult <= m_psMidiNoteData.size())
                        {
                            if (m_psMidiNoteData.getUnchecked(m_iCurrentClassificationResult - 1)->bInclude)
                            {
                                m_pcMidiOut->makeNoteAndSend(m_psMidiNoteData.getUnchecked(m_iCurrentClassificationResult - 1)->iChannelNo,
                                                             m_psMidiNoteData.getUnchecked(m_iCurrentClassificationResult - 1)->iNoteNum,
                                                             m_fCurrentRMS,
                                                             m_psMidiNoteData.getUnchecked(m_iCurrentClassificationResult - 1)->iDuration_ms);
                            }
                            
                        }
                        
                        
                        // Trigger Audio
                        if (m_pbAudioInclude.getUnchecked(m_iCurrentClassificationResult - 1))
                        {
                            m_pcAudioMixer->startPlayback(m_iCurrentClassificationResult - 1, m_fCurrentRMS);
                        }
                    }
                    
                    guiUpdater->displayPlayingOnset();
                    guiUpdater->drawSpectrum();
                    
                }
            }
            // End Play Mode Scope
            
            
            m_bOnsetDetected = false;
        }
    
    
        
        
        if (m_pcOnsetClassifier->detectOnset(inputChannelData))
        {
            m_bOnsetDetected = true;
            m_fCurrentRMS = (m_pcOnsetClassifier->getCurrentRMSIndB() + 40.0f) * 3.0f;
//            std::cout << m_fCurrentRMS << std::endl;
        }
    }
    
    
    for (int sample = 0; sample < blockSize; sample++)
    {
        for (int channel = 0; channel < totalNumOutputChannels; channel++)
        {
            outputChannelData[channel][sample] = 0.0f;
        }
    }
    
    guiUpdater->drawWaveformSample();
    
}


void AudioStream::audioDeviceStopped()
{
    
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
    m_pcAudioMixer->addClass();
    
    m_psMidiNoteData.add(new MidiNoteData());
    
    m_psMidiNoteData.getLast()->iChannelNo = 1;
    m_psMidiNoteData.getLast()->iDuration_ms = 250;
    m_psMidiNoteData.getLast()->iNoteNum = 36;
    m_psMidiNoteData.getLast()->bInclude = false;
    
    m_pbAudioInclude.add(false);
    
    m_bMemoryInitialized = true;
}

void AudioStream::deleteClass(int classIndex)
{
    if (classIndex >= 0)
    {
        m_pcOnsetClassifier->deleteClass(classIndex);
        m_psMidiNoteData.remove(classIndex - 1);
        m_pcAudioMixer->deleteClass(classIndex - 1);
        m_pbAudioInclude.remove(classIndex - 1);
    }
    
    if (m_pbAudioInclude.size() == 0)
    {
        m_bMemoryInitialized = false;
    }
}

void AudioStream::saveTraining(File trainingFile)
{
    m_pcOnsetClassifier->setAudioFilePathsToSave(m_psAudioFilePaths);
    m_pcOnsetClassifier->saveTraining(trainingFile);
}

void AudioStream::loadTraining(File trainingFile)
{
    m_pcOnsetClassifier->loadTraining(trainingFile);
    m_psAudioFilePaths = m_pcOnsetClassifier->getAudioFilePathsToLoad();
    
    
    for (int i=0; i < m_pcOnsetClassifier->getNumClasses(); i++)
    {
        m_pcAudioMixer->addClass();
        
        m_psMidiNoteData.add(new MidiNoteData());
        
        m_psMidiNoteData.getLast()->iChannelNo = 1;
        m_psMidiNoteData.getLast()->iDuration_ms = 250;
        m_psMidiNoteData.getLast()->iNoteNum = 36;
        m_psMidiNoteData.getLast()->bInclude = false;
        
        m_pbAudioInclude.add(false);
        
        m_bMemoryInitialized = true;
        
        setAudioOutputFile(i, m_psAudioFilePaths.getReference(i));
    }
    
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


//void AudioStream::playTrainingDataAtClass(int classIndex)
//{
//////    m_iCurrentIndexToPlay   =   m_piClassLabels.indexOf(classIndex);
////    m_iCurrentIndexToPlay       =   classIndex - 1;
////    
////    sharedAudioDeviceManager->addAudioCallback(this);
////    startTimer((deviceSetup.bufferSize / deviceSetup.sampleRate) * 1000.0f);
//////    startTimer(50);
//}


void AudioStream::audioDeviceSettingsChanged()
{
    sharedAudioDeviceManager->getAudioDeviceSetup(deviceSetup);
    m_pcOnsetClassifier->initialize(deviceSetup.bufferSize, deviceSetup.sampleRate);
}


void AudioStream::updateDataset(Array<bool> includes, Array<int> classes, bool type)
{
    m_pcOnsetClassifier->updateDataset(includes, classes, type);
    guiUpdater->doneTraining();
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


std::vector<float> AudioStream::getCurrentSpectrum()
{
    return m_pcOnsetClassifier->getCurrentSpectrum();
}

float AudioStream::getCurrentSpectralCentroid()
{
    return m_pcOnsetClassifier->getCurrentSpectralCentroid();
}

float AudioStream::getCurrentBlockRMS()
{
    return m_pcOnsetClassifier->getCurrentRMSIndB();
}


//==============================================================================
// Audio MIDI Output Methods
//==============================================================================

void AudioStream::setMIDIOutput(int index, bool include, int channelNo, int noteNumber, int duration_ms)
{
    if (index >= 0)
    {
        m_psMidiNoteData.getUnchecked(index)->bInclude = include;
        m_psMidiNoteData.getUnchecked(index)->iChannelNo = channelNo;
        m_psMidiNoteData.getUnchecked(index)->iNoteNum   = noteNumber;
        m_psMidiNoteData.getUnchecked(index)->iDuration_ms = duration_ms;
    }
}


void AudioStream::setAudioOutputFile(int index, File audioFile)
{
    if (index >= 0)
    {
        m_pcAudioMixer->loadAudioFile(index, audioFile);
        m_psAudioFilePaths.set(index, audioFile.getFullPathName());
    }
}


void AudioStream::setAudioOutputLooping(int index, bool looping)
{
    if (index >= 0)
    {
        m_pcAudioMixer->setLooping(index, looping);
    }
}

void AudioStream::setAudioOutputToggle(int index, bool toggle)
{
    if (index >= 0)
    {
        m_pcAudioMixer->setToggle(index, toggle);
    }
}

void AudioStream::setAudioOutputInclude(int index, bool include)
{
    if (index >= 0)
    {
        m_pbAudioInclude.set(index, include);
    }
    
}


//==============================================================================
// Timer Callback Methods
//==============================================================================
void AudioStream::timerCallback()
{
//    sharedAudioDeviceManager->removeAudioCallback(this);
    stopTimer();
}


void AudioStream::sendKeyPressEvent()
{
//    KeyPress slideChange = KeyPress(KeyPress::rightKey);
}