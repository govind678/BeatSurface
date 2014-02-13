/*
  ==============================================================================

    ParseCSV.h
    Created: 13 Feb 2014 4:26:54pm
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#ifndef PARSECSV_H_INCLUDED
#define PARSECSV_H_INCLUDED

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>

class ParseCSV
{
public:
    
    ParseCSV();
    ~ParseCSV();
    
    void parseMatrix(std::string filename, float** data, int numRows, int numColumns);
    void parseList(std::string filename, float* data, int numRows);
    
    void getFileSize(std::string filename, int& numRows, int& numColumns);
    
    void getMaxElementInFile(std::string filename, double& maxElement);
    void getMaxElementInFile(std::string filename, int& maxElement);
    
private:
    
    std::ifstream inputFile;
    
};




#endif  // PARSECSV_H_INCLUDED
