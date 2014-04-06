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

    static const int AddClass                    = 0x1000;
    static const int DeleteClass                 = 0x1001;
    static const int CurrentClass                = 0x1002;
    
    static const int IdleMode                    = 0x1010;
    static const int TrainingMode                = 0x1011;
    static const int PlayMode                    = 0x1012;
    
//    static const int ToggleClock                 = 0x1021;
    
    static const int Preferences                 = 0x1030;
        
    static const int GoToKioskMode               = 0x1040;

}


namespace CommandCategories {
    
    static const char* const TrainCommands			= "Train";
    static const char* const PlayCommands			= "Play";
    static const char* const SettingsCommands		= "Settings";
    static const char* const CommonCommands         = "Common";
}


#endif  // COMMANDIDS_H_INCLUDED
