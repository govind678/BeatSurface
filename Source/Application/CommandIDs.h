/*
  ==============================================================================

    CommandIDs.h
    Created: 2 Mar 2014 2:32:42pm
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#ifndef COMMANDIDS_H_INCLUDED
#define COMMANDIDS_H_INCLUDED


namespace CommandIDs {

    static const int addClass                    = 0x1000;
    static const int deleteClass                 = 0x1001;
    static const int currentClass                = 0x1002;
        
    static const int recordTraining              = 0x1010;
    static const int doneTraining                = 0x1011;
        
    static const int startAudio                  = 0x1020;
    static const int stopAudio                   = 0x1021;
        
    static const int applySettings               = 0x1030;
        
    static const int goToKioskMode               = 0x1040;

}


namespace CommandCategories {
    
    static const char* const TrainCommands			= "Train";
    static const char* const PlayCommands			= "Play";
    static const char* const SettingsCommands		= "Settings";
    static const char* const CommonCommands         = "Common";
}

#endif  // COMMANDIDS_H_INCLUDED
