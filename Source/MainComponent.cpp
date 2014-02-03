/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"



//==============================================================================
MainContentComponent::MainContentComponent()
{
    backgroundColour = new Colour(0xFF333333);
    
    iNumInputChannels = 1;
    iNumInputChannels = 2;
    
    
    
    //--- Buttons ---//
    
    addAndMakeVisible(audioToggleButton = new TextButton (String::empty));
    audioToggleButton->setButtonText("Audio Off");
    audioToggleButton->addListener(this);
    audioToggleButton->setColour (TextButton::buttonColourId, Colours::darkgrey);
    audioToggleButton->setColour (TextButton::textColourOnId, Colours::black);
    
    addAndMakeVisible(readAudioFileButton = new TextButton (String::empty));
    readAudioFileButton->setButtonText("Read Audio File");
    readAudioFileButton->addListener(this);
    readAudioFileButton->setColour (TextButton::buttonColourId, Colours::darkgrey);
    readAudioFileButton->setColour (TextButton::textColourOnId, Colours::black);
    
    addAndMakeVisible(loadTrainingButton = new TextButton (String::empty));
    loadTrainingButton->setButtonText("Load Training");
    loadTrainingButton->addListener(this);
    loadTrainingButton->setColour (TextButton::buttonColourId, Colours::darkgrey);
    loadTrainingButton->setColour (TextButton::textColourOnId, Colours::black);
    
    addAndMakeVisible(saveTrainingButton = new TextButton (String::empty));
    saveTrainingButton->setButtonText("Save Training");
    saveTrainingButton->addListener(this);
    saveTrainingButton->setColour (TextButton::buttonColourId, Colours::darkgrey);
    saveTrainingButton->setColour (TextButton::textColourOnId, Colours::black);
    
    
    addAndMakeVisible(trainClass1Button = new TextButton (String::empty));
    trainClass1Button->setButtonText("Train Class 1");
    trainClass1Button->addListener(this);
    trainClass1Button->setColour (TextButton::buttonColourId, Colours::darkgrey);
    trainClass1Button->setColour (TextButton::textColourOnId, Colours::black);
    
    addAndMakeVisible(trainClass2Button = new TextButton (String::empty));
    trainClass2Button->setButtonText("Train Class 2");
    trainClass2Button->addListener(this);
    trainClass2Button->setColour (TextButton::buttonColourId, Colours::darkgrey);
    trainClass2Button->setColour (TextButton::textColourOnId, Colours::black);
  
    
    addAndMakeVisible(trainClass3Button = new TextButton (String::empty));
    trainClass3Button->setButtonText("Train Class 3");
    trainClass3Button->addListener(this);
    trainClass3Button->setColour (TextButton::buttonColourId, Colours::darkgrey);
    trainClass3Button->setColour (TextButton::textColourOnId, Colours::black);
  
    
    
    
    //--- Sliders ---//
    
    addAndMakeVisible(velocitySensitivitySlider = new Slider(String::empty));
    velocitySensitivitySlider->setRange(0.0001,1,0.0001);
    velocitySensitivitySlider->setSliderStyle(Slider::Rotary);
    velocitySensitivitySlider->setColour (Slider::rotarySliderFillColourId, Colours::darkgrey);
    velocitySensitivitySlider->setColour(Slider::textBoxTextColourId, Colours::whitesmoke);
    velocitySensitivitySlider->setColour(Slider::textBoxBackgroundColourId, *backgroundColour);
    velocitySensitivitySlider->setColour(Slider::textBoxOutlineColourId, Colours::darkgrey);
    velocitySensitivitySlider->setTextBoxStyle(Slider::TextBoxBelow, true, 80, 20);
    velocitySensitivitySlider->addListener(this);

    addAndMakeVisible(decayTimeSensitivitySlider = new Slider(String::empty));
    decayTimeSensitivitySlider->setRange(0.025,1,0.025);
    decayTimeSensitivitySlider->setSliderStyle(Slider::Rotary);
    decayTimeSensitivitySlider->setColour (Slider::rotarySliderFillColourId, Colours::darkgrey);
    decayTimeSensitivitySlider->setColour(Slider::textBoxTextColourId, Colours::whitesmoke);
    decayTimeSensitivitySlider->setColour(Slider::textBoxBackgroundColourId, *backgroundColour);
    decayTimeSensitivitySlider->setColour(Slider::textBoxOutlineColourId, Colours::darkgrey);
    decayTimeSensitivitySlider->setTextBoxStyle(Slider::TextBoxBelow, true, 80, 20);
    decayTimeSensitivitySlider->addListener(this);
    
    
    addAndMakeVisible (velocitySensitivityLabel = new Label (String::empty,("Velocity Sensitivity")));
    velocitySensitivityLabel->setFont (Font (12.0000f));
    velocitySensitivityLabel->setJustificationType (Justification::centred);
    velocitySensitivityLabel->setEditable (false, false, false);
    velocitySensitivityLabel->setColour (Label::textColourId, Colours::whitesmoke);
    velocitySensitivityLabel->setColour (TextEditor::textColourId, Colours::darkgrey);
    velocitySensitivityLabel->setColour (TextEditor::backgroundColourId, Colours::darkgrey);
    
    addAndMakeVisible (decayTimeSensitivityLabel = new Label (String::empty,("Decay Time Sensitivity")));
    decayTimeSensitivityLabel->setFont (Font (12.0000f));
    decayTimeSensitivityLabel->setJustificationType (Justification::centred);
    decayTimeSensitivityLabel->setEditable (false, false, false);
    decayTimeSensitivityLabel->setColour (Label::textColourId, Colours::whitesmoke);
    decayTimeSensitivityLabel->setColour (TextEditor::textColourId, *backgroundColour);
    decayTimeSensitivityLabel->setColour (TextEditor::backgroundColourId, *backgroundColour);
    
    
    
    
    //--- Audio Initialization ---//
    deviceManager.initialise(iNumInputChannels, iNumOutputChannels, 0, true);
    deviceManager.getAudioDeviceSetup(deviceSetup);
    
    
    setSize (800, 600);
}

MainContentComponent::~MainContentComponent()
{
    mpAudioStream               = nullptr;
    
    audioToggleButton           = nullptr;
    loadTrainingButton          = nullptr;
    saveTrainingButton          = nullptr;
    readAudioFileButton         = nullptr;
    
    trainClass1Button           = nullptr;
    trainClass2Button           = nullptr;
    trainClass3Button           = nullptr;
    
    velocitySensitivitySlider   = nullptr;
    velocitySensitivityLabel    = nullptr;
    decayTimeSensitivitySlider  = nullptr;
    decayTimeSensitivityLabel   = nullptr;
    backgroundColour            = nullptr;
}



void MainContentComponent::paint (Graphics& g)
{
    g.fillAll (*backgroundColour);

    g.setFont (Font (16.0f));
    g.setColour (Colours::black);
}



void MainContentComponent::resized()
{
    
    //-- Object --//                            //-- x-pos --//         //-- y-pos --//         //-- width --//     //-- height --//
    
    readAudioFileButton         ->  setBounds( getWidth()/2 - 140,      getHeight()/2 - 20,         120,                30 );
    audioToggleButton           ->  setBounds( getWidth()/2 + 20,       getHeight()/2 - 20,         120,                30 );
    
    trainClass1Button           ->  setBounds( 40,                      40,                         120,                30 );
    trainClass2Button           ->  setBounds( 40,                      80,                         120,                30 );
    trainClass3Button           ->  setBounds( 40,                      120,                        120,                30 );
    
    saveTrainingButton          ->  setBounds( 40,                      180,                        120,                30 );
    loadTrainingButton          ->  setBounds( 40,                      220,                        120,                30 );
    
    velocitySensitivitySlider   ->  setBounds( getWidth()/2 - 110,      getHeight()/2 + 70,         90,                 90 );
    velocitySensitivityLabel    ->  setBounds( getWidth()/2 - 125,      getHeight()/2 + 160,        120,                30 );
    
    decayTimeSensitivitySlider  ->  setBounds( getWidth()/2 + 20,       getHeight()/2 + 70,         90,                 90 );
    decayTimeSensitivityLabel   ->  setBounds( getWidth()/2 + 5,        getHeight()/2 + 160,        120,                30 );

}



void MainContentComponent::buttonClicked(Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == audioToggleButton) {
        
        if (mpAudioStream == nullptr)
        {
            mpAudioStream = new AudioStream(deviceManager, deviceSetup, 0);
            audioToggleButton->setButtonText("Audio On");
            audioToggleButton->setColour (TextButton::buttonColourId, Colours::lightgrey);
            
            velocitySensitivitySlider->setValue(mpAudioStream->onsetClassifier->getVelocitySensitivity());
            decayTimeSensitivitySlider->setValue(mpAudioStream->onsetClassifier->getDecayTimeSensitivity());
        }
        
        else
        {
            mpAudioStream = nullptr;
            audioToggleButton->setButtonText("Audio Off");
            audioToggleButton->setColour (TextButton::buttonColourId, Colours::darkgrey);
        }
        
    }
    
    
    
    
    if (buttonThatWasClicked == loadTrainingButton) {
        
        //        if(audioPlayer->transportSource.isPlaying())
        //        {
        //            audioPlayer->transportSource.stop();
        //            playStopButton->setButtonText("Play");
        //            playStopButton->setColour(TextButton::buttonColourId, Colours::darkgrey);
        //        }
        //
        //        else
        //        {
        //            audioPlayer->transportSource.setPosition(0);
        //            audioPlayer->transportSource.start();
        //            playStopButton->setButtonText("Stop");
        //            playStopButton->setColour(TextButton::buttonColourId, Colours::lightgrey);
        //
        
        
    }
    
    
    
    if (buttonThatWasClicked == saveTrainingButton) {
        
        
        
    }
    
    
    
    
    if (buttonThatWasClicked == trainClass1Button) {
        
        if (mpAudioStream == nullptr)
        {
            mpAudioStream = new AudioStream(deviceManager, deviceSetup, 1);
            trainClass1Button->setButtonText("Click When Done...");
            trainClass1Button->setColour (TextButton::buttonColourId, Colours::lightgrey);
        }
        
        else
        {
            mpAudioStream = nullptr;
            trainClass1Button->setButtonText("Train Class 1");
            trainClass1Button->setColour (TextButton::buttonColourId, Colours::darkgrey);
        }
        
    }
    
    
    
    if (buttonThatWasClicked == trainClass2Button) {
        
        if (mpAudioStream == nullptr)
        {
            mpAudioStream = new AudioStream(deviceManager, deviceSetup, 2);
            trainClass2Button->setButtonText("Click When Done...");
            trainClass2Button->setColour (TextButton::buttonColourId, Colours::lightgrey);
        }
        
        else
        {
            mpAudioStream = nullptr;
            trainClass2Button->setButtonText("Train Class 2");
            trainClass2Button->setColour (TextButton::buttonColourId, Colours::darkgrey);
        }
        
    }
    
    
    
    if (buttonThatWasClicked == trainClass3Button) {
        
        if (mpAudioStream == nullptr)
        {
            mpAudioStream = new AudioStream(deviceManager, deviceSetup, 3);
            trainClass3Button->setButtonText("Click When Done...");
            trainClass3Button->setColour (TextButton::buttonColourId, Colours::lightgrey);
        }
        
        else
        {
            mpAudioStream = nullptr;
            trainClass3Button->setButtonText("Train Class 3");
            trainClass3Button->setColour (TextButton::buttonColourId, Colours::darkgrey);
        }
        
    }
    
    
    
    
    
    if (buttonThatWasClicked == readAudioFileButton) {
        
        
        File mcAudioFile;
        FileChooser mcFileChooser (("Choose Audio File to Read"), File::nonexistent, "*", true);
        mcFileChooser.browseForFileToOpen();
        mcAudioFile = mcFileChooser.getResult();
        
        ScopedPointer<AudioFileIO> mpAudioFileIO = new AudioFileIO(mcAudioFile, deviceSetup.bufferSize);
        
        mpAudioFileIO->onsetClassifier->setVelocitySensitivity(velocitySensitivitySlider->getValue());
        mpAudioFileIO->onsetClassifier->setDecayTimeSensitivity(decayTimeSensitivitySlider->getValue());
        
        mpAudioFileIO->runDetectionAndClassification();
        
        
//        std::ofstream inputAudioTxtFile;
//        inputAudioTxtFile.open("/Users/govindarampingali/Documents/GaTech/Masters Project/MATLAB/outputFile.txt");
//        for (int64 i=0; i < miNumBlocks; i++) {
//            inputAudioTxtFile << onsets[i] << std::endl;
//        }
//        inputAudioTxtFile.close();
        
        mpAudioFileIO = nullptr;
        
    
    }
    
}



void MainContentComponent::sliderValueChanged(Slider* sliderValueThatHasChanged)
{
    if(sliderValueThatHasChanged == velocitySensitivitySlider)
    {
        if (mpAudioStream != nullptr) {
            mpAudioStream->onsetClassifier->setVelocitySensitivity(velocitySensitivitySlider->getValue());
        }
        
    }
    
    else if(sliderValueThatHasChanged == decayTimeSensitivitySlider)
    {
        if (mpAudioStream != nullptr) {
            mpAudioStream->onsetClassifier->setDecayTimeSensitivity(decayTimeSensitivitySlider->getValue());
        }
    }
}

