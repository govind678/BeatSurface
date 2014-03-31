/*
  ==============================================================================

    MainComponent.cpp
    Created: 14 Feb 2014 2:41:14am
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#include "MainComponent.h"
#include "BeatSurfaceEngine.h"

#include <stdio.h>
#include <iostream>
#include <math.h>


ScopedPointer<BeatSurfaceEngine> m_pcBeatSurfaceEngine;


MainComponent::MainComponent()
{
    playComponent               = new PlayContentComponent();
    settingsComponent           = new SettingsContentComponent();
    transcriptionComponent      = new TranscriptionComponent();
    
    m_pcBeatSurfaceEngine       = new BeatSurfaceEngine();
    globalClock->setBeatSurfaceEngine(m_pcBeatSurfaceEngine);
    
    commandManager->registerAllCommandsForTarget (this);
    
    addAndMakeVisible (tabbedComponent = new TabbedComponent (TabbedButtonBar::TabsAtTop));
    tabbedComponent->setTabBarDepth (50);
    
    tabbedComponent->addTab (TRANS("Play"),     Colour (0xff2b2b2b), playComponent,     true);
    tabbedComponent->addTab (TRANS("Settings"), Colour (0xff2b2b2b), settingsComponent, true);
    tabbedComponent->addTab (TRANS("Results"), Colour (0xff2b2b2b), transcriptionComponent, true);
    tabbedComponent->setCurrentTabIndex (0);
    
    
    
    
    playComponent->toolBarControls->audioStreamToggleButton->addListener(this);
    playComponent->toolBarControls->recordToggleButton->addListener(this);
    playComponent->toolBarControls->addClassButton->addListener(this);
    playComponent->toolBarControls->deleteClassButton->addListener(this);
    playComponent->toolBarControls->saveTrainingButton->addListener(this);
    playComponent->toolBarControls->loadTrainingButton->addListener(this);
    playComponent->trainingTimeinBarsSlider->addListener(this);


    settingsComponent->audioSetupButton->addListener(this);
    settingsComponent->velocitySensitivitySlider->addListener(this);
    settingsComponent->decayTimeSensitivitySlider->addListener(this);
    settingsComponent->tempoNumBox->addListener(this);
    settingsComponent->numeratorNumBox->addListener(this);
    settingsComponent->denominatorNumBox->addListener(this);
    
    settingsComponent->setMainComponent(this);
    
    
    transcriptionComponent->retrainButton->addListener(this);
    transcriptionComponent->clearButton->addListener(this);
    
    
    
    addAndMakeVisible (beatSurfaceLogo = new ImageButton ("aboutButton"));
    beatSurfaceLogo->addListener (this);
    beatSurfaceLogo->setImages (false, true, true,
                            ImageCache::getFromMemory (BinaryData::Icon128_png, BinaryData::Icon128_pngSize), 0.750f, Colour (0x00000000),
                            ImageCache::getFromMemory (BinaryData::Icon128_png, BinaryData::Icon128_pngSize), 0.500f, Colour (0x00000000),
                            ImageCache::getFromMemory (BinaryData::Icon128_png, BinaryData::Icon128_pngSize), 0.750f, Colour (0x44666666));
    
    
    
    addAndMakeVisible (beatSurfaceLabel = new Label ("beatSurfaceLabel",
                                          TRANS("Beat Surface")));
    beatSurfaceLabel->setFont (Font ("Myriad Pro", 18.00f, Font::bold | Font::italic));
    beatSurfaceLabel->setJustificationType (Justification::centred);
    beatSurfaceLabel->setEditable (false, false, false);
    beatSurfaceLabel->setColour (Label::textColourId, Colour (0xff4c5256));
    beatSurfaceLabel->setColour (TextEditor::textColourId, Colours::black);
    beatSurfaceLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    
    
    addAndMakeVisible (gtcmtLabel = new Label ("gtcmtLabel",
                                           TRANS("GTCMT")));
    gtcmtLabel->setFont (Font ("Myriad Pro", 12.00f, Font::bold | Font::italic));
    gtcmtLabel->setJustificationType (Justification::centred);
    gtcmtLabel->setEditable (false, false, false);
    gtcmtLabel->setColour (Label::textColourId, Colour (0xff4c5256));
    gtcmtLabel->setColour (TextEditor::textColourId, Colours::black);
    gtcmtLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    
    
    audioSetup = new AudioSetupDisplay;
    audioSetup->applyAudioSettingsButton->addListener(this);
    
    addAndMakeVisible(clockDisplay = new ClockDisplayComponent);
    clockDisplay->setClickingTogglesState(true);
    clockDisplay->addListener(this);
    
    
    m_iCurentSelectedClass = 1;
    m_iNumClasses = 0;

    
    addAndMakeVisible(confidenceDisplayBar = new Slider("confidenceDisplaySlider"));
    confidenceDisplayBar->setSliderStyle(Slider::LinearBar);
    confidenceDisplayBar->setTextValueSuffix(" %");
    confidenceDisplayBar->setRange(0.0f, 100.0f, 0.01);
    confidenceDisplayBar->setColour(Slider::backgroundColourId, Colours::lightgrey);
    confidenceDisplayBar->setColour(Slider::trackColourId, Colour(0xFF474F57));
    confidenceDisplayBar->setInterceptsMouseClicks(false, false);
    confidenceDisplayBar->setAlpha(0.8);
    confidenceDisplayBar->setValue(0.0f, dontSendNotification);
    
    
    //--- Test Transcribing ---//
//    transcriptionComponent->setNumClasses(5);
//    for (int i=0; i < 5; i++)
//    {
//        transcriptionComponent->addRow(i+1, "Hello: " + String(i), i+1, "Features: " + String(i));
//    }
    
    setSize(getParentWidth(), getParentHeight());
    
    
    playComponent->shapeButtonArray->setButtonFlashTime_ms(15);
    playComponent->metroDisplay->setTotalBeats(16);
    
    startTimer(BeatSurfaceBase::iGUITimerInterval);
    
}



MainComponent::~MainComponent()
{
    m_pfOnsetProbabilities.clear();
    
    confidenceDisplayBar        = nullptr;
    beatSurfaceLogo             = nullptr;
    beatSurfaceLabel            = nullptr;
    gtcmtLabel                  = nullptr;
    
    audioSetup                  = nullptr;
    
    clockDisplay                = nullptr;
    
    playComponent               = nullptr;
    settingsComponent           = nullptr;
    transcriptionComponent      = nullptr;
    
    tabbedComponent             = nullptr;
    
    m_pcBeatSurfaceEngine       = nullptr;
    
    
    deleteAllChildren();
    
}



void MainComponent::paint (Graphics& g)
{
    g.fillAll (Colour (0xff2b2b2b));
}


void MainComponent::resized()
{
    tabbedComponent->setBounds (0, 0, proportionOfWidth (1.0000f), proportionOfHeight (1.0000f));
    
    beatSurfaceLogo->setBounds(getWidth() - 182, getHeight() - 80, 64, 64);
    
    beatSurfaceLabel->setBounds (getWidth() - 130, getHeight() - 45 - 24, 128, 24);
    gtcmtLabel->setBounds (getWidth() - 38 - 64, getHeight() - 25 - 20, 64, 20);
    
    clockDisplay->setBounds(getWidth() - 80, 60, 64, 64);
    
    confidenceDisplayBar->setBounds(getWidth()/2 - 150, getHeight() - 25, 300, 20);
}



void MainComponent::focusGained (FocusChangeType cause)
{
    
}


void MainComponent::focusLost (FocusChangeType cause)
{
    
}




//==============================================================================
// Button Listener
//==============================================================================

void MainComponent::buttonClicked (Button* buttonThatWasClicked)
{
    
    //--- Launch Audio Preferences ---//
    if(buttonThatWasClicked == settingsComponent->audioSetupButton)
    {
        launchPreferences();
    }
    
    //--- Apply Current Audio Settings ---//
    if (buttonThatWasClicked == audioSetup->applyAudioSettingsButton)
    {
        m_pcBeatSurfaceEngine->audioDeviceSettingsChanged();
    }

    
    //--- Add Class ---//
    if (buttonThatWasClicked == playComponent->toolBarControls->addClassButton)
    {
        addClass(true);
    }
    
    
    //--- Delete Class ---//
    if (buttonThatWasClicked == playComponent->toolBarControls->deleteClassButton)
    {
        PopupMenu pop;
        
        if (m_iNumClasses > 0) {
            for (int i=0; i < m_iNumClasses; i++) {
                pop.addItem(i+1, "Class " + String(i+1));
            }
        }
        
        pop.showMenuAsync (PopupMenu::Options().withTargetComponent (playComponent->toolBarControls->deleteClassButton), nullptr);
       
        
//        deleteClass(0);
    }
    
    
    
    
    //--- Play Button Clicked ---//
    if (buttonThatWasClicked == playComponent->toolBarControls->audioStreamToggleButton)
    {
        if (playComponent->toolBarControls->recordToggleButton->getToggleState())
        {
            playComponent->toolBarControls->recordToggleButton->setToggleState(false, dontSendNotification);
            m_pcBeatSurfaceEngine->recordButtonClicked(false);
            transcriptionComponent->clearRows();
        }
        
        
        if (playComponent->toolBarControls->audioStreamToggleButton->getToggleState())
        {
            m_pcBeatSurfaceEngine->playButtonClicked(true);
            playComponent->shapeButtonArray->setZeroAlpha();
        }
        
        else
        {
            m_pcBeatSurfaceEngine->playButtonClicked(false);
            playComponent->shapeButtonArray->resetAlpha();
        }
        
        transcriptionComponent->setSystemMode(m_pcBeatSurfaceEngine->getSystemMode());
    }
    
    
    //--- Record Button Clicked ---//
    if (buttonThatWasClicked == playComponent->toolBarControls->recordToggleButton)
    {
        if (playComponent->toolBarControls->audioStreamToggleButton->getToggleState())
        {
            m_pcBeatSurfaceEngine->playButtonClicked(false);
            playComponent->toolBarControls->audioStreamToggleButton->setToggleState(false, dontSendNotification);
            m_pcBeatSurfaceEngine->playButtonClicked(false);
        }
        
        if (playComponent->toolBarControls->recordToggleButton->getToggleState())
        {
            m_pcBeatSurfaceEngine->recordButtonClicked(true);
        }
        
        else
        {
            m_pcBeatSurfaceEngine->recordButtonClicked(false);
            transcriptionComponent->clearRows();
        }
        
        transcriptionComponent->setSystemMode(m_pcBeatSurfaceEngine->getSystemMode());
    }
    
  
    //--- Load Training ---//
    if (buttonThatWasClicked == playComponent->toolBarControls->loadTrainingButton)
    {
        FileChooser fc ("Choose Training File To Load...",
                        File::getSpecialLocation(File::userDocumentsDirectory), "", false);
        
        if(fc.browseForFileToOpen())
        {
            File result = fc.getResult();
            
            if (result.existsAsFile())
            {
                m_pcBeatSurfaceEngine->loadTraining(result);
            }
            
        }
        
        transcriptionComponent->setSystemMode(m_pcBeatSurfaceEngine->getSystemMode());
    }
    

    //--- Save Training ---//
    if (buttonThatWasClicked == playComponent->toolBarControls->saveTrainingButton)
    {
        FileChooser fc ("Save Current Training...",
                        File::getSpecialLocation(File::userDocumentsDirectory), "*.txt", false);
        
        if (fc.browseForFileToSave (true))
        {
            m_pcBeatSurfaceEngine->saveTraining(fc.getResult());
        }
        
    }
    
    
    //--- Beat Surface Logo Clicked ---//
    if (buttonThatWasClicked == beatSurfaceLogo)
    {
        String dialogBoxText;
        
        dialogBoxText << "Georgia Center for Music Technology" << newLine << newLine << "Govinda Ram Pingali";
        
        DialogWindow::LaunchOptions options;
        options.componentToCentreAround = nullptr;
        Label* label = new Label();
        label->setText (dialogBoxText, dontSendNotification);
        label->setJustificationType(Justification::centred);
        label->setColour (Label::textColourId, Colours::whitesmoke);
        options.content.setOwned (label);
        
        
        Rectangle<int> area (0, 0, 400, 370);
        options.content->setSize (area.getWidth(), area.getHeight());
        
        options.dialogTitle                   = "About";
        options.escapeKeyTriggersCloseButton  = true;
        options.useNativeTitleBar             = false;
        options.resizable                     = false;
        
        options.launchAsync();
        
    }
    
    
    //--- Metronome Button Clicked ---//
    if (buttonThatWasClicked == clockDisplay)
    {
        if(clockDisplay->getToggleState())
        {
            globalClock->startClock();
        }
        
        else
        {
            globalClock->stopClock();
        }
        
    }
    
    
    if (buttonThatWasClicked == transcriptionComponent->retrainButton)
    {
        m_pcBeatSurfaceEngine->updateDataset(transcriptionComponent->getCurrentIncludes(),
                                             transcriptionComponent->getCurrentClasses());
        
        
        transcriptionComponent->clearRows();
    }
    
    
    
    if (buttonThatWasClicked == transcriptionComponent->clearButton)
    {
        
        
        transcriptionComponent->clearRows();
    }
    
    
    
    //--- Training Class Button Clicked ---//
    if (m_iNumClasses > 0)
    {
        for (int i = 0; i < m_iNumClasses; i++)
        {
            if (buttonThatWasClicked == playComponent->shapeButtonArray->m_pcClassButton.getUnchecked(i))
            {
                m_pcBeatSurfaceEngine->classButtonClicked(i+1);
                
                if (m_pcBeatSurfaceEngine->getSystemMode() == BeatSurfaceBase::TrainMode)
                {
                    playComponent->shapeButtonArray->m_pcClassButton.getUnchecked(i)->setAlpha(0.2f);
                }
                
            }
        }
    }
    
}


void MainComponent::loadAudioFileToPlay(int index)
{
    FileChooser fc ("Load Audio File for Class: " + String(index+1),
                    File::getSpecialLocation(File::userMusicDirectory), "", false);
    
    if(fc.browseForFileToOpen())
    {
        File result = fc.getResult();
        
        if (result.existsAsFile())
        {
            m_pcBeatSurfaceEngine->setAudioOutputFile(index, result);
            settingsComponent->setAudioFileName(index, result.getFileName());
        }
        
    }
}

void MainComponent::setMidiOutput(int classIndex, int channelNo, int noteNum, int duration_ms)
{
    m_pcBeatSurfaceEngine->setMidiOutput(classIndex, channelNo, noteNum, duration_ms);
}

void MainComponent::setAudioFileParam(int index, bool looping, bool toggle)
{
    m_pcBeatSurfaceEngine->setAudioOutputLooping(index, looping);
    m_pcBeatSurfaceEngine->setAudioOutputToggle(index, toggle);
}


//==============================================================================
// Slider Listener
//==============================================================================

void MainComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    if (sliderThatWasMoved == settingsComponent->velocitySensitivitySlider)
    {
        m_pcBeatSurfaceEngine->setParameter (BeatSurfaceBase::VelocitySensitivity,
                                        settingsComponent->velocitySensitivitySlider->getValue());
    }
    
    if (sliderThatWasMoved == settingsComponent->decayTimeSensitivitySlider)
    {
        m_pcBeatSurfaceEngine->setParameter (BeatSurfaceBase::DecayTimeSensitivity,
                                        settingsComponent->decayTimeSensitivitySlider->getValue());
    }
    
    
    if (sliderThatWasMoved == playComponent->trainingTimeinBarsSlider)
    {
        m_pcBeatSurfaceEngine->setTrainingTimeinBars(int(playComponent->trainingTimeinBarsSlider->getValue()));
        playComponent->metroDisplay->setTotalBeats(int(playComponent->trainingTimeinBarsSlider->getValue()) * 4);
    }
}



//==============================================================================
// Label Listener
//==============================================================================

void MainComponent::labelTextChanged (Label *labelThatHasChanged)
{
    if (labelThatHasChanged == settingsComponent->tempoNumBox)
    {
        globalClock->setTempo(settingsComponent->tempoNumBox->getText().getFloatValue());
    }
    
    if (labelThatHasChanged == settingsComponent->numeratorNumBox)
    {
        globalClock->setNumerator(settingsComponent->numeratorNumBox->getText().getIntValue());
    }
    
    if (labelThatHasChanged == settingsComponent->denominatorNumBox)
    {
        globalClock->setDenominator(settingsComponent->denominatorNumBox->getText().getIntValue());
    }
    
}



//==============================================================================
// Timer Callback to Update GUI
//==============================================================================

void MainComponent::timerCallback()
{
    if (guiUpdater->DisplayTrainingOnset)
    {
        playComponent->shapeButtonArray->flashButton(m_pcBeatSurfaceEngine->getCurrentTrainingClass() - 1, 0.2f);
        
        transcriptionComponent->addRow(m_pcBeatSurfaceEngine->getCurrentObservation(),
                                       "Time!",
                                       m_pcBeatSurfaceEngine->getCurrentTrainingClass(),
                                       m_pcBeatSurfaceEngine->getCurrentFeatureVector());
        
        guiUpdater->DisplayTrainingOnset = false;
    }
    
    
    if (guiUpdater->DoneTraining)
    {
        if (m_iNumClasses > 0)
        {
            playComponent->shapeButtonArray->resetAlpha();
            playComponent->metroDisplay->setZeroAlpha();
        }
        
        confidenceDisplayBar->setValue(m_pcBeatSurfaceEngine->getClassificationConfidence(), dontSendNotification);
        
        guiUpdater->DoneTraining = false;
    }
    
    
    if (guiUpdater->DisplayPlayingOnset)
    {
        playComponent->shapeButtonArray->flashButton(m_pcBeatSurfaceEngine->getCurrentClassificationResult() - 1, 0.0f);
        
        transcriptionComponent->addRow(m_pcBeatSurfaceEngine->getCurrentObservation(),
                                       "Time!",
                                       m_pcBeatSurfaceEngine->getCurrentClassificationResult(),
                                       m_pcBeatSurfaceEngine->getCurrentFeatureVector());
        
        guiUpdater->DisplayPlayingOnset = false;
    }
    
    
    if(guiUpdater->DrawWaveform)
    {
        playComponent->waveformLiveScroller->getSampleToDraw(guiUpdater->getSampleToDrawWaveform());
        
//        float sample = guiUpdater->getSampleToDrawWaveform();
//        playComponent->liveAudioScroller->pushSample(sample);
//        playComponent->liveAudioScroller->repaint();
        guiUpdater->DrawWaveform = false;
    }
    
    if(guiUpdater->DrawSpectrum)
    {
        playComponent->spectrumLiveScroller->setPointToDisplayInTime(guiUpdater->getSpectralCentroidToDraw());
        playComponent->spectrumLiveScroller->setArrayToDraw(guiUpdater->getArrayToDrawSpectrum());
        guiUpdater->DrawSpectrum = false;
    }
    
    
    if(guiUpdater->LoadTrainingUpdate)
    {
        updateGUIOnLoadTraining();
        guiUpdater->LoadTrainingUpdate = false;
    }
    
}



//==============================================================================
// Action Listener
//==============================================================================

void MainComponent::actionListenerCallback(const String &message)
{
    if (message.startsWith("DBEAT"))
    {
//        clockDisplay->flashBeat(message.getLastCharacters(1).getIntValue());
        playComponent->metroDisplay->countBeat();
    }
    
    
    if (message == "UPDATE_TRANSPORT")
    {
        playComponent->waveformLiveScroller->updateScrollLineSpacing();
        playComponent->spectrumLiveScroller->updateScrollLineSpacing();
    }
    
}



//==============================================================================
// Component - Internal Methods
//==============================================================================

void MainComponent::addClass(bool updateBackend)
{
    if (updateBackend)
    {
        m_iNumClasses++;
        m_pcBeatSurfaceEngine->addClass();
    }
    
    
    playComponent->shapeButtonArray->setNumClasses(m_iNumClasses);
    transcriptionComponent->setNumClasses(m_iNumClasses);
    settingsComponent->addClass();
    
    
    if (m_iNumClasses > 0)
    {
        
        for (int i = 0; i < m_iNumClasses; i++)
        {
            playComponent->shapeButtonArray->m_pcClassButton.getUnchecked(i)->addListener(this);
        }
    }

    if (m_pcBeatSurfaceEngine->getSystemMode() == BeatSurfaceBase::PlayMode)
    {
        playComponent->shapeButtonArray->setZeroAlpha();
    }
    
    
    m_pfOnsetProbabilities.resize(m_iNumClasses);
    
}



void MainComponent::deleteClass(int classIndex, bool updateBackend)
{
    if (updateBackend)
    {
        m_pcBeatSurfaceEngine->deleteClass(classIndex);
        
        m_iNumClasses--;
        
        if (m_iNumClasses < 0)
        {
            m_iNumClasses = 0;
        }
    }
    
    
    playComponent->shapeButtonArray->setNumClasses(m_iNumClasses);
    transcriptionComponent->setNumClasses(m_iNumClasses);

    
    if (m_iNumClasses > 0)
    {
        for (int i = 0; i < m_iNumClasses; i++)
        {
            playComponent->shapeButtonArray->m_pcClassButton.getUnchecked(i)->addListener(this);
        }
    }
    
    if (m_pcBeatSurfaceEngine->getSystemMode() == BeatSurfaceBase::PlayMode)
    {
        playComponent->shapeButtonArray->setZeroAlpha();
    }
    
    
    
    m_pfOnsetProbabilities.resize(m_iNumClasses);
}




void MainComponent::updateGUIOnLoadTraining()
{
    for (int i = 0; i < m_iNumClasses; i++)
    {
        deleteClass(i, false);
    }
    
    
    m_iNumClasses = m_pcBeatSurfaceEngine->getNumClasses();
    
    for (int i = 0; i < m_iNumClasses; i++)
    {
        addClass(false);
    }
    
    
    settingsComponent->velocitySensitivitySlider->
                                setValue(m_pcBeatSurfaceEngine->getParameter(BeatSurfaceBase::VelocitySensitivity));
    
    
    settingsComponent->decayTimeSensitivitySlider->
                                setValue(m_pcBeatSurfaceEngine->getParameter(BeatSurfaceBase::DecayTimeSensitivity));
    
}




void MainComponent::launchPreferences()
{
    DialogWindow::LaunchOptions options;
    options.componentToCentreAround = nullptr;
    options.content.setNonOwned (audioSetup);
    Rectangle<int> area (0, 0, 420, 390);
    
    options.content->setSize (area.getWidth(), area.getHeight());
    
    
    options.dialogTitle                   = "Audio Preferences";
    options.dialogBackgroundColour        = Colour (0xBB24272D);
    options.escapeKeyTriggersCloseButton  = true;
    options.useNativeTitleBar             = false;
    options.resizable                     = true;
    
    options.launchAsync();
    
}



//==============================================================================
// Application Command Target
//==============================================================================

ApplicationCommandTarget* MainComponent::getNextCommandTarget()
{
	return findFirstTargetParentComponent();
}

void MainComponent::getAllCommands (Array <CommandID>& commands)
{
	const CommandID ids[] =
    {
        CommandIDs::AddClass,
        CommandIDs::DeleteClass,
        CommandIDs::CurrentClass,
        CommandIDs::RecordTraining,
        CommandIDs::DoneTraining,
        CommandIDs::ToggleAudio,
        CommandIDs::ToggleClock,
        CommandIDs::Preferences,
        CommandIDs::GoToKioskMode,
    };
	
	commands.addArray (ids, numElementsInArray (ids));
}


void MainComponent::getCommandInfo (const CommandID commandID, ApplicationCommandInfo& result)
{
    
    //within 'setInfo()' below, the name sets the String that appears in the Menu bar,
    //and the description sets what would appear in the tooltip if the command is set to a button
    //and the tooltip parameter is set to 'true'
	
    switch (commandID)
    {
        case CommandIDs::AddClass:
            result.setInfo ("Add New Class", "Adds a new class of sounds", CommandCategories::TrainCommands, 0);
            result.addDefaultKeypress ('=', ModifierKeys::commandModifier);
            break;
            
        case CommandIDs::DeleteClass:
            result.setInfo ("Delete Selected Class", "Deletes a selected class", CommandCategories::TrainCommands, 0);
            result.addDefaultKeypress ('-', ModifierKeys::commandModifier);
            break;
            
        case CommandIDs::CurrentClass:
            result.setInfo ("Select Class", "Selects the current class", CommandCategories::CommonCommands, 0);
            result.addDefaultKeypress ('t', ModifierKeys::commandModifier);
            break;
            
        case CommandIDs::RecordTraining:
            result.setInfo ("Record Training", "Start currently selected class recording", CommandCategories::TrainCommands, 0);
            result.addDefaultKeypress ('3', ModifierKeys::commandModifier);
            break;
            
        case CommandIDs::DoneTraining:
            result.setInfo ("Finished Training", "Finished currently selected training", CommandCategories::TrainCommands, 0);
            result.addDefaultKeypress ('4', ModifierKeys::commandModifier);
            break;
            
        case CommandIDs::ToggleAudio:
            result.setInfo ("Start/Stop System", "Turns on/off detection and classification system", CommandCategories::PlayCommands, 0);
            result.addDefaultKeypress ('1', ModifierKeys::commandModifier);
            break;
            
        case CommandIDs::ToggleClock:
            result.setInfo ("Start Metronome", "Metronome default to 120BPM", CommandCategories::CommonCommands, 0);
            result.addDefaultKeypress (' ', ModifierKeys::noModifiers);
            break;
            
        case CommandIDs::Preferences:
            result.setInfo ("Open Preferences Panel", "Audio, MIDI and other Preferences", CommandCategories::SettingsCommands, 0);
            result.addDefaultKeypress (',', ModifierKeys::commandModifier);
            break;
            
            
        #if ! JUCE_LINUX
        case CommandIDs::GoToKioskMode:
            result.setInfo ("Show full-screen kiosk mode", String::empty, CommandCategories::CommonCommands, 0);
            result.addDefaultKeypress ('f', ModifierKeys::commandModifier);
            result.setTicked (Desktop::getInstance().getKioskModeComponent() != 0);
            break;
        #endif
            
        default:
            break;
    }
}



bool MainComponent::perform (const InvocationInfo& info)
{
	switch (info.commandID)
    {
        case CommandIDs::AddClass:
            addClass(true);
            break;
            
        case CommandIDs::DeleteClass:
            // do something
            deleteClass(0, true);
            std::cout << "@MainComponent: Delete class" << std::endl;
            break;
            
        case CommandIDs::CurrentClass:
            // do something
            std::cout << "@MainComponent: Current class" << std::endl;
            break;
            
            
            
        case CommandIDs::RecordTraining:
            // do something
            std::cout << "@MainComponent: Record Training" << std::endl;
            break;
            
        case CommandIDs::DoneTraining:
            // do something
            std::cout << "@MainComponent: Done Training" << std::endl;
            break;
            
            
            
        case CommandIDs::ToggleAudio:
            // do something
            
            break;
            
            
        case CommandIDs::ToggleClock:
            if (!globalClock->getMetronomeStatus()) {
                globalClock->startClock();
            } else {
                globalClock->stopClock();
                clockDisplay->repaint();
            }
            break;
            
            
            
        case CommandIDs::Preferences:
            // do something
            launchPreferences();
            break;
            
            
        #if ! JUCE_LINUX
        case CommandIDs::GoToKioskMode:
        {
            Desktop& desktop = Desktop::getInstance();
            
            if (desktop.getKioskModeComponent() == nullptr)
                desktop.setKioskModeComponent (getTopLevelComponent());
            else
                desktop.setKioskModeComponent (nullptr);
            
            break;
        }
        #endif
            
        default:
            return false;
    }

    return true;
	
}


ApplicationCommandManager* MainComponent::getCommandManager()
{
    return commandManager;
}


//========================================================================================================================







//======================================== Audio Setup Display ===========================================================
// Component that holds Audio Device Selector Component
//========================================================================================================================

AudioSetupDisplay::AudioSetupDisplay()
{
    deviceSelectorComponent = new AudioDeviceSelectorComponent (*sharedAudioDeviceManager,
                                                                BeatSurfaceBase::iMinNoInputChannelsRequired,
                                                                BeatSurfaceBase::iMaxNoInputChannelsRequired,
                                                                BeatSurfaceBase::iMinNoOutputChannelsRequired,
                                                                BeatSurfaceBase::iMaxNoOutputChannelssRequired,
                                                                BeatSurfaceBase::bShowMidiInputOptions,
                                                                BeatSurfaceBase::bShowMidiOutputSelector,
                                                                BeatSurfaceBase::bShowChannelsAsStereoPairs,
                                                                BeatSurfaceBase::bHideAdvancedOptionsWithButton);
    
    deviceSelectorComponent->setName("audioDeviceSelectorComponent");
    
    addAndMakeVisible(deviceSelectorComponent);
    
    
    addAndMakeVisible (applyAudioSettingsButton = new TextButton ("applySettingsButton"));
    applyAudioSettingsButton->setButtonText (TRANS("Apply"));
    applyAudioSettingsButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    applyAudioSettingsButton->setColour (TextButton::buttonColourId, Colour (0xff3e3e3e));
    applyAudioSettingsButton->setColour (TextButton::buttonOnColourId, Colour (0xff909090));
    applyAudioSettingsButton->setColour (TextButton::textColourOnId, Colour (0xff2a2a2a));
    applyAudioSettingsButton->setColour (TextButton::textColourOffId, Colours::black);
    
    
}


AudioSetupDisplay::~AudioSetupDisplay()
{
    deviceSelectorComponent = nullptr;
    applyAudioSettingsButton = nullptr;
    
    deleteAllChildren();
}


void AudioSetupDisplay::paint(juce::Graphics &g)
{
    g.fillAll (Colour (0xAA364045));
}

void AudioSetupDisplay::resized()
{
    deviceSelectorComponent->setBounds(0, 0, getWidth(), getHeight() - 40);
    applyAudioSettingsButton->setBounds(getWidth()/2 - 50, getHeight() - 40, 100, 30);
}


//========================================================================================================================
