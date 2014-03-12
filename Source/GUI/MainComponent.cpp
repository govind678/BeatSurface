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


ScopedPointer<BeatSurfaceEngine> beatSurfaceEngine;


MainComponent::MainComponent()
{
    
    m_eCurrentMode              = BeatSurfaceBase::IdleMode;
    
    
    playComponent               = new PlayContentComponent();
    settingsComponent           = new SettingsContentComponent();
    
    beatSurfaceEngine           = new BeatSurfaceEngine();
    globalClock->setBeatSurfaceEngine(beatSurfaceEngine);
    
    commandManager->registerAllCommandsForTarget (this);
    
    addAndMakeVisible (tabbedComponent = new TabbedComponent (TabbedButtonBar::TabsAtTop));
    tabbedComponent->setTabBarDepth (50);
    
    tabbedComponent->addTab (TRANS("Play"),     Colour (0xff2b2b2b), playComponent,     true);
    tabbedComponent->addTab (TRANS("Settings"), Colour (0xff2b2b2b), settingsComponent, true);
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
    
    setSize(getParentWidth(), getParentHeight());
    
    
    playComponent->shapeButtonArray->setButtonFlashTime_ms(15);
    
    startTimer(BeatSurfaceBase::iGUITimerInterval);
    
}



MainComponent::~MainComponent()
{
    m_pfOnsetProbabilities.clear();
    
    
    playComponent               = nullptr;
    settingsComponent           = nullptr;
    
    tabbedComponent             = nullptr;
    
    beatSurfaceLogo             = nullptr;
    beatSurfaceLabel            = nullptr;
    gtcmtLabel                  = nullptr;
    
    
    
    beatSurfaceEngine           = nullptr;
    
    audioSetup                  = nullptr;
    
    clockDisplay                = nullptr;

    
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
        beatSurfaceEngine->audioDeviceSettingsChanged();
    }

    
    //--- Add Class ---//
    if (buttonThatWasClicked == playComponent->toolBarControls->addClassButton)
    {
        addClass();
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
       
        
        deleteClass(0);
    }
    
    
    
    
    //--- Play Button Clicked ---//
    if (buttonThatWasClicked == playComponent->toolBarControls->audioStreamToggleButton)
    {
        if (playComponent->toolBarControls->recordToggleButton->getToggleState())
        {
            playComponent->toolBarControls->recordToggleButton->setToggleState(false, dontSendNotification);
        }
        
        
        if (playComponent->toolBarControls->audioStreamToggleButton->getToggleState())
        {
            beatSurfaceEngine->liveAudioStreamButtonClicked(true);
            playComponent->shapeButtonArray->setZeroAlpha();
            m_eCurrentMode  = BeatSurfaceBase::PlayMode;
        }
        
        else
        {
            beatSurfaceEngine->liveAudioStreamButtonClicked(false);
            playComponent->shapeButtonArray->resetAlpha();
            beatSurfaceEngine->setMode(m_eCurrentMode);
            m_eCurrentMode  = BeatSurfaceBase::IdleMode;
        }
    }
    
    
    //--- Record Button Clicked ---//
    if (buttonThatWasClicked == playComponent->toolBarControls->recordToggleButton)
    {
        if (playComponent->toolBarControls->audioStreamToggleButton->getToggleState())
        {
            beatSurfaceEngine->liveAudioStreamButtonClicked(false);
            playComponent->toolBarControls->audioStreamToggleButton->setToggleState(false, dontSendNotification);
        }
        
        if (playComponent->toolBarControls->recordToggleButton->getToggleState())
        {
            m_eCurrentMode  = BeatSurfaceBase::TrainMode;
            beatSurfaceEngine->setMode(m_eCurrentMode);
        }
        
        else
        {
            m_eCurrentMode  = BeatSurfaceBase::IdleMode;
            beatSurfaceEngine->setMode(m_eCurrentMode);
        }
    }
    
  
    //--- Load Training ---//
    if (buttonThatWasClicked == playComponent->toolBarControls->loadTrainingButton)
    {
        FileChooser fc ("Choose Training File To Load...",
                        File::getCurrentWorkingDirectory(), "*.csv", false);
        
        if(fc.browseForFileToOpen())
        {
            beatSurfaceEngine->loadTraining(fc.getResult().getFullPathName());
        }
    }
    

    //--- Save Training ---//
    if (buttonThatWasClicked == playComponent->toolBarControls->saveTrainingButton)
    {
        FileChooser fc ("Save Current Training...", File::getCurrentWorkingDirectory(), "*.csv", false);
        
        if (fc.browseForFileToSave (true))
        {
            beatSurfaceEngine->saveTraining(fc.getResult().getFullPathName());
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
    
    
    //--- Training Class Button Clicked ---//
    if (m_iNumClasses > 0)
    {
        for (int i = 0; i < m_iNumClasses; i++)
        {
            if (buttonThatWasClicked == playComponent->shapeButtonArray->m_pcClassButton.getUnchecked(i))
            {
                if (m_eCurrentMode == BeatSurfaceBase::PlayMode)
                {
                    
                }
                
                else if (m_eCurrentMode == BeatSurfaceBase::TrainMode)
                {
                    beatSurfaceEngine->trainClassButtonClicked(i+1);
                    playComponent->shapeButtonArray->m_pcClassButton.getUnchecked(i)->setAlpha(0.2f);
                }
                
                else if (m_eCurrentMode == BeatSurfaceBase::IdleMode)
                {
                    beatSurfaceEngine->idleModeClassButtonClicked(i+1);
                }
                
            }
        }
    }
    
    
}




//==============================================================================
// Slider Listener
//==============================================================================

void MainComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    if (sliderThatWasMoved == settingsComponent->velocitySensitivitySlider)
    {
        beatSurfaceEngine->parametersChanged(BeatSurfaceBase::VelocitySensitivity,
                                             settingsComponent->velocitySensitivitySlider->getValue());
    }
    
    if (sliderThatWasMoved == settingsComponent->decayTimeSensitivitySlider)
    {
        beatSurfaceEngine->parametersChanged(BeatSurfaceBase::DecayTimeSensitivity,
                                             settingsComponent->decayTimeSensitivitySlider->getValue());
    }
    
    
    if (sliderThatWasMoved == playComponent->trainingTimeinBarsSlider)
    {
        beatSurfaceEngine->setTrainingTimeinBars(int(playComponent->trainingTimeinBarsSlider->getValue()));
        
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
// Check Current Selected Tab
//==============================================================================

void MainComponent::currentTabChanged(int newCurrentTabIndex, const juce::String &newCurrentTabName)
{
    std::cout << "@MainComponent: " << newCurrentTabIndex << ": " << newCurrentTabName << std::endl;
}




//==============================================================================
// Timer Callback to Update GUI
//==============================================================================

void MainComponent::timerCallback()
{
    if (guiUpdater->DisplayTrainingOnset)
    {
        playComponent->shapeButtonArray->flashButton(guiUpdater->getCurrentTrainingClass() - 1, 0.2f);
        guiUpdater->DisplayTrainingOnset = false;
    }
    
    
    if (guiUpdater->DoneTraining)
    {
        if (m_iNumClasses > 0)
        {
            playComponent->shapeButtonArray->resetAlpha();
        }
        guiUpdater->DoneTraining = false;
    }
    
    if(guiUpdater->DisplayOnsetProbabilities)
    {
        m_pfOnsetProbabilities = guiUpdater->getOnsetProbabilities();
        playComponent->shapeButtonArray->flashArray(m_pfOnsetProbabilities);
        guiUpdater->DisplayOnsetProbabilities = false;
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
    
}



//==============================================================================
// Action Listener
//==============================================================================

void MainComponent::actionListenerCallback(const String &message)
{
    if (message.startsWith("BEAT"))
    {
        clockDisplay->flashBeat(message.getLastCharacters(1).getIntValue());
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

void MainComponent::addClass()
{
    m_iNumClasses++;
    
    playComponent->shapeButtonArray->setNumClasses(m_iNumClasses);
    guiUpdater->setNumClasses(m_iNumClasses);
    
    
    if (m_iNumClasses > 0)
    {
        
        for (int i = 0; i < m_iNumClasses; i++)
        {
            playComponent->shapeButtonArray->m_pcClassButton.getUnchecked(i)->addListener(this);
        }
    }

    if (m_eCurrentMode == BeatSurfaceBase::PlayMode)
    {
        playComponent->shapeButtonArray->setZeroAlpha();
    }
    
    beatSurfaceEngine->addClass();
    
    m_pfOnsetProbabilities.resize(m_iNumClasses);
    
}



void MainComponent::deleteClass(int classIndex)
{
    m_iNumClasses--;
    
    if (m_iNumClasses < 0) {
        m_iNumClasses = 0;
    }
    
    playComponent->shapeButtonArray->setNumClasses(m_iNumClasses);
    

    
    if (m_iNumClasses > 0)
    {
        for (int i = 0; i < m_iNumClasses; i++)
        {
            playComponent->shapeButtonArray->m_pcClassButton.getUnchecked(i)->addListener(this);
        }
    }
    
    if (m_eCurrentMode == BeatSurfaceBase::PlayMode)
    {
        playComponent->shapeButtonArray->setZeroAlpha();
    }
    
    beatSurfaceEngine->deleteClass(classIndex);
    
    m_pfOnsetProbabilities.resize(m_iNumClasses);
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
            addClass();
            break;
            
        case CommandIDs::DeleteClass:
            // do something
            deleteClass(0);
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
