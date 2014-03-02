/*
  ==============================================================================

    MainComponent.cpp
    Created: 14 Feb 2014 2:41:14am
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#include "MainComponent.h"
#include <stdio.h>
#include <iostream>
#include <math.h>


MainComponent::MainComponent()
{
    playComponent = new PlayContentComponent();
    trainComponent = new TrainContentComponent();
    settingsComponent = new SettingsContentComponent();
    
    beatSurfaceEngine = new BeatSurfaceEngine();
    
    addAndMakeVisible (tabbedComponent = new TabbedComponent (TabbedButtonBar::TabsAtTop));
    tabbedComponent->setTabBarDepth (50);
    
    tabbedComponent->addTab (TRANS("Play"),     Colour (0xff2b2b2b), playComponent,     true);
    tabbedComponent->addTab (TRANS("Train"),    Colour (0xff2b2b2b), trainComponent,    true);
    tabbedComponent->addTab (TRANS("Settings"), Colour (0xff2b2b2b), settingsComponent, true);
    tabbedComponent->setCurrentTabIndex (0);
    
    
    
    
    playComponent->audioStreamToggleButton->addListener(this);
    
    trainComponent->addClassButton->addListener(this);
    trainComponent->deleteClassButton->addListener(this);

    settingsComponent->audioSetupButton->addListener(this);
    
    settingsComponent->velocitySensitivitySlider->addListener(this);
    settingsComponent->decayTimeSensitivitySlider->addListener(this);
    
    
    
    addAndMakeVisible (beatSurfaceLogo = new ImageButton ("aboutButton"));
    beatSurfaceLogo->addListener (this);
    beatSurfaceLogo->setImages (false, true, true,
                            ImageCache::getFromMemory (BinaryData::Icon64_png, BinaryData::Icon64_pngSize), 0.750f, Colour (0x00000000),
                            ImageCache::getFromMemory (BinaryData::Icon64_png, BinaryData::Icon64_pngSize), 0.500f, Colour (0x00000000),
                            ImageCache::getFromMemory (BinaryData::Icon64_png, BinaryData::Icon64_pngSize), 0.750f, Colour (0x44666666));
    
    
    
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
    
    
    
    addAndMakeVisible(classProbSlider = new Slider("class1Slider"));
    classProbSlider->addListener(this);
    classProbSlider->setRange (0, 1, 0.01);
    classProbSlider->setSliderStyle (Slider::IncDecButtons);
    classProbSlider->setTextBoxStyle (Slider::TextBoxBelow, false, 50, 20);
    classProbSlider->setColour(Slider::backgroundColourId, Colour (0x7f363e45));
    classProbSlider->setColour (Slider::thumbColourId, Colour (0xff363e46));
    //numClassesSlider->setColour (Slider::rotarySliderFillColourId, Colour (0x7f363e45));
    classProbSlider->setColour (Slider::textBoxTextColourId, Colour (0xff404952));
    classProbSlider->setColour (Slider::textBoxBackgroundColourId, Colour (0x00000000));
    classProbSlider->setColour (Slider::textBoxHighlightColourId, Colour (0x00000000));
    classProbSlider->setColour (Slider::textBoxOutlineColourId, Colour (0x00808080));
    
    
    addAndMakeVisible(classNumSelectorSlider = new Slider("classNumSelector"));
    classNumSelectorSlider->addListener(this);
    classNumSelectorSlider->setRange (1, 10, 1);
    classNumSelectorSlider->setSliderStyle (Slider::IncDecButtons);
    classNumSelectorSlider->setTextBoxStyle (Slider::TextBoxBelow, false, 50, 20);
    classNumSelectorSlider->setColour(Slider::backgroundColourId, Colour (0x7f363e45));
    classNumSelectorSlider->setColour (Slider::thumbColourId, Colour (0xff363e46));
    //numClassesSlider->setColour (Slider::rotarySliderFillColourId, Colour (0x7f363e45));
    classNumSelectorSlider->setColour (Slider::textBoxTextColourId, Colour (0xff404952));
    classNumSelectorSlider->setColour (Slider::textBoxBackgroundColourId, Colour (0x00000000));
    classNumSelectorSlider->setColour (Slider::textBoxHighlightColourId, Colour (0x00000000));
    classNumSelectorSlider->setColour (Slider::textBoxOutlineColourId, Colour (0x00808080));
    
    
    
    audioSetup = new AudioSetupDisplay;
    audioSetup->applyAudioSettingsButton->addListener(this);
    
    
    
    addAndMakeVisible(shapeButtonArray = new ClassButtonArray);
    
    m_iCurentSelectedClass = 1;
    m_iNumClasses = 0;
    
    setSize (BeatSurfaceBase::iDocumentInitWidth, BeatSurfaceBase::iDocumentInitHeight);
    
}



MainComponent::~MainComponent()
{
    
    sharedAudioDeviceManager->removeAudioCallback(playComponent->liveAudioScroller);
    
    
    tabbedComponent             = nullptr;
    beatSurfaceLogo             = nullptr;
    beatSurfaceLabel            = nullptr;
    gtcmtLabel                  = nullptr;
    
    playComponent               = nullptr;
    trainComponent              = nullptr;
    settingsComponent           = nullptr;
    
    beatSurfaceEngine           = nullptr;
    
    audioSetup                  = nullptr;
    
    shapeButtonArray            = nullptr;
    
    classNumSelectorSlider      = nullptr;
    classProbSlider             = nullptr;

    
    if (m_pfCurrentOnsetProbabilities != nullptr) {
        m_pfCurrentOnsetProbabilities   =   nullptr;
    }
    
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
    
    classProbSlider->setBounds(getWidth() - 140, getHeight()/2 + 0, 120, 64);
    classNumSelectorSlider->setBounds(getWidth() - 140, getHeight()/2 + 64, 120, 64);
    
    shapeButtonArray->setBounds(getWidth()/2 - getHeight()/4, getHeight()/2 - getHeight()/4 + 30, getHeight()/2, getHeight()/2);
    
}



void MainComponent::buttonClicked (Button* buttonThatWasClicked)
{
    
    if(buttonThatWasClicked == settingsComponent->audioSetupButton)
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
    
    
    if (buttonThatWasClicked == audioSetup->applyAudioSettingsButton)
    {
        beatSurfaceEngine->audioDeviceSettingsChanged();
    }

    
    if (buttonThatWasClicked == trainComponent->addClassButton)
    {
        m_iNumClasses++;
        
        shapeButtonArray->setNumClasses(m_iNumClasses);
        
        
        if (m_pfCurrentOnsetProbabilities != nullptr) {
            m_pfCurrentOnsetProbabilities   =   nullptr;
        }
        
        
        if (m_iNumClasses > 0)
        {
            m_pfCurrentOnsetProbabilities   = new float [m_iNumClasses];
            
            for (int i = 0; i < m_iNumClasses; i++)
            {
                shapeButtonArray->m_pcClassButton.getUnchecked(i)->addListener(this);
                m_pfCurrentOnsetProbabilities[i] = 0.0f;
            }
        }
        
        shapeButtonArray->setArrayAlpha(m_pfCurrentOnsetProbabilities);
        
    }
    
    
    
    if (buttonThatWasClicked == trainComponent->deleteClassButton)
    {
        PopupMenu pop;
        
        if (m_iNumClasses > 0) {
            for (int i=0; i < m_iNumClasses; i++) {
                pop.addItem(i+1, "Class " + String(i+1));
            }
        }
        
        pop.showMenuAsync (PopupMenu::Options().withTargetComponent (trainComponent->deleteClassButton), nullptr);
       
        
        m_iNumClasses--;
        
        if (m_iNumClasses < 0) {
            m_iNumClasses = 0;
        }
        
        shapeButtonArray->setNumClasses(m_iNumClasses);
        
        
        if (m_pfCurrentOnsetProbabilities != nullptr) {
            m_pfCurrentOnsetProbabilities   =   nullptr;
        }
        
        
        if (m_iNumClasses > 0)
        {
            m_pfCurrentOnsetProbabilities   = new float [m_iNumClasses];
            
            for (int i = 0; i < m_iNumClasses; i++)
            {
                shapeButtonArray->m_pcClassButton.getUnchecked(i)->addListener(this);
                m_pfCurrentOnsetProbabilities[i] = 0.0f;
            }
        }
        
        shapeButtonArray->setArrayAlpha(m_pfCurrentOnsetProbabilities);
        
    }
    
    
    if (buttonThatWasClicked == playComponent->audioStreamToggleButton)
    {
        if (playComponent->audioStreamToggleButton->getToggleState()) {
            //playComponent->audioStreamToggleButton->setButtonText (TRANS("Audio On"));
            //beatSurfaceInterface->liveAudioStreamButtonClicked(true);
            sharedAudioDeviceManager->addAudioCallback(playComponent->liveAudioScroller);
        }
        
        else {
            //beatSurfaceInterface->liveAudioStreamButtonClicked(false);
            sharedAudioDeviceManager->removeAudioCallback(playComponent->liveAudioScroller);
            //playComponent->audioStreamToggleButton->setButtonText (TRANS("Audio Off"));
        }
        
        
    }
    
    
    if (buttonThatWasClicked == trainComponent->loadTrainingButton)
    {
        
    }
    

    if (buttonThatWasClicked == trainComponent->saveTrainingButton)
    {
        
    }
    
    
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
    
    
    if (m_iNumClasses > 0)
    {
        for (int i = 0; i < m_iNumClasses; i++) {
            if (buttonThatWasClicked == shapeButtonArray->m_pcClassButton.getUnchecked(i)) {
                std::cout << "Class: " << i+1 << std::endl;
            }
        }
    }
    
    
}




void MainComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    if (sliderThatWasMoved == settingsComponent->velocitySensitivitySlider)
    {
        
    }
    
    else if (sliderThatWasMoved == settingsComponent->decayTimeSensitivitySlider)
    {
        
    }
    
    
    else if (sliderThatWasMoved == classProbSlider)
    {
        m_pfCurrentOnsetProbabilities[m_iCurentSelectedClass - 1] = classProbSlider->getValue();
        shapeButtonArray->setArrayAlpha(m_pfCurrentOnsetProbabilities);
    }
    
    
    else if (sliderThatWasMoved == classNumSelectorSlider)
    {
        m_iCurentSelectedClass = int(classNumSelectorSlider->getValue());
    }
    
}





void MainComponent::focusGained (FocusChangeType cause)
{

}


void MainComponent::focusLost (FocusChangeType cause)
{
    
}

void MainComponent::currentTabChanged(int newCurrentTabIndex, const juce::String &newCurrentTabName)
{
    std::cout << newCurrentTabIndex << ": " << newCurrentTabName << std::endl;
}


void MainComponent::timerCallback()
{
    
}



//========================================================================================================================
// Audio Setup Display
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
// Class Button Array
// Use extended Shape Button to dynamically create pie-shaped buttons based on number of classes
//========================================================================================================================

ClassButtonArray::ClassButtonArray()
{
    m_iNumClasses = 0;
    m_pcClassButton.clear(true);
}


ClassButtonArray::~ClassButtonArray()
{
    
}


void ClassButtonArray::paint(juce::Graphics &g)
{
    
    if (m_iNumClasses > 0) {
        
        
        for (int side = 1; side <= m_iNumClasses; side++)
        {
            Path pie;
            
            float fromRadians   = float((side - 1.0f) * 2.0f * M_PI) / float(m_iNumClasses);
            float toRadians     = float(side * 2.0f * M_PI) / float(m_iNumClasses);
            
            pie.addPieSegment(0.0f, 0.0f, getWidth(), getHeight(), fromRadians, toRadians, 0.0f);
            pie.closeSubPath();
            g.setColour(Colours::black);
            g.strokePath(pie, PathStrokeType(1.0f));
        }
        
        
    }
}


void ClassButtonArray::resized()
{
    if (m_iNumClasses > 0)
    {
        for (int side = 1; side <= m_iNumClasses; side++)
        {
            Path pie;
            
            float fromRadians   = float((side - 1.0f) * 2.0f * M_PI) / float(m_iNumClasses);
            float toRadians     = float(side * 2.0f * M_PI) / float(m_iNumClasses);
            
            pie.addPieSegment(0.0f, 0.0f, getWidth(), getHeight(), fromRadians, toRadians, 0.0f);
            pie.closeSubPath();
            
            m_pcClassButton.getUnchecked(side-1)->setShape(pie, true, true);
            m_pcClassButton.getUnchecked(side-1)->setBounds(0, 0, getWidth(), getHeight());
        }
    }
}


void ClassButtonArray::setNumClasses(int numClasses)
{
    m_iNumClasses = numClasses;
    
    if(m_pcClassButton.size() > 0)
    {
        m_pcClassButton.clear(true);
    }
    
    
    
    if (m_iNumClasses > 0) {
        
        
        for (int side = 1; side <= m_iNumClasses; side++) {
            
            m_pcClassButton.add(new CustomShapeButton(String(side),
                                                      Colour(float(side)/float(m_iNumClasses), 0.75f, 0.9f, 1.0f),
                                                      Colour(float(side)/float(m_iNumClasses), 0.75f, 0.7f, 1.0f),
                                                      Colour(float(side)/float(m_iNumClasses), 0.75f, 0.5f, 1.0f)));
            
            Path pie;
            
            float fromRadians   = float((side - 1.0f) * 2.0f * M_PI) / float(m_iNumClasses);
            float toRadians     = float(side * 2.0f * M_PI) / float(m_iNumClasses);
            
            pie.addPieSegment(0.0f, 0.0f, getWidth(), getHeight(), fromRadians, toRadians, 0.0f);
            pie.closeSubPath();
            
            m_pcClassButton.getUnchecked(side-1)->setShape(pie, true, true);
            addAndMakeVisible(m_pcClassButton.getUnchecked(side-1));
            m_pcClassButton.getUnchecked(side-1)->setBounds(0, 0, getWidth(), getHeight());
        }
        
        
    }
}


void ClassButtonArray::setArrayAlpha(float* alphaValues)
{
    if (m_iNumClasses > 0)
    {
        for (int i=0; i < m_iNumClasses; i++) {
            m_pcClassButton.getUnchecked(i)->setAlpha(alphaValues[i]);
        }
    }
}
