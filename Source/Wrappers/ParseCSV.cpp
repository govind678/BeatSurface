/*
  ==============================================================================

    ParseCSV.cpp
    Created: 13 Feb 2014 4:26:54pm
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#include "ParseCSV.h"


ParseCSV::ParseCSV()
{
    
}


ParseCSV::~ParseCSV()
{
    
}


void ParseCSV::parseMatrix(std::string filename, float** data, int numRows, int numColumns)
{
    inputFile.open(filename);
    
    if (inputFile.is_open())
    {
        
        for(int row = 0; row < numRows; row++)
        {
            std::string line;
            std::getline(inputFile, line);
            
            if ( !inputFile.good() )
                break;
            
            std::stringstream inputStream(line);
            
            for (int col = 0; col < numColumns; col++)
            {
                std::string val;
                std::getline(inputStream, val, ',');
                
                std::stringstream convertor(val);
                convertor >> data[row][col];
                
                if ( !inputStream.good() )
                    break;
                
            }
        }
        
        inputFile.close();
        
    }
    
    else
    {
        std::cout << "@ParseCSV: Error: Opening File: " << filename << std::endl;
    }
    
}



void ParseCSV::parseList(std::string filename, float* data, int numRows)
{
    
    inputFile.open(filename);
    
    if (inputFile.is_open()) {
        
        for(int row = 0; row < numRows; row++)
        {
            std::string line;
            std::getline(inputFile, line);
            
            std::stringstream inputStream(line);
            inputStream >> data[row];
            
            if ( !inputFile.good() )
                break;
            
        }
        
        inputFile.close();
        
    }
    
    else
    {
        std::cout << "@ParseCSV: Error: Opening File: " << filename << std::endl;
    }
    
    
}



void ParseCSV::getFileSize(std::string filename, int& numRows, int& numColumns)
{
    int row = 0;
    int column = 0;
    
    inputFile.open(filename);
    
    if (inputFile.is_open())
    {
        while (!inputFile.eof())
        {
            std::string line;
            std::getline(inputFile, line);
            
            std::stringstream inputStream(line);
            
            while (inputStream.good())
            {
                std::string val;
                std::getline(inputStream, val, ',');
                
                column++;
            }
            
            row++;
        }
        
        numColumns = int(column / (row - 1));
        numRows = row - 1;
        
        inputFile.close();
    }
    
    else
    {
        std::cout << "@ParseCSV: Error: Opening File: " << filename << std::endl;
    }
    
}



void ParseCSV::getMaxElementInFile(std::string filename, int& maxElement)
{
    
    int currentMaxElement = 0;
    
    inputFile.open(filename);
    
    if (inputFile.is_open())
    {
        while (!inputFile.eof())
        {
            std::string line;
            std::getline(inputFile, line);
            
            std::stringstream inputStream(line);
            
            while (inputStream.good())
            {
                int element;
                
                std::string val;
                std::getline(inputStream, val, ',');
                
                std::stringstream convertor(val);
                convertor >> element;
                
                if (element > currentMaxElement) {
                    currentMaxElement = element;
                }
            }
            
        }
        
        maxElement = currentMaxElement;
        
        inputFile.close();
    }
    
    else
    {
        std::cout << "@ParseCSV: Error: Opening File: " << filename << std::endl;
    }
    
    
}




void ParseCSV::getMaxElementInFile(std::string filename, double& maxElement)
{
    
    double currentMaxElement = 0.0;
    
    inputFile.open(filename);
    
    if (inputFile.is_open())
    {
        while (!inputFile.eof())
        {
            std::string line;
            std::getline(inputFile, line);
            
            std::stringstream inputStream(line);
            
            while (inputStream.good())
            {
                double element;
                
                std::string val;
                std::getline(inputStream, val, ',');
                
                std::stringstream convertor(val);
                convertor >> element;
                
                if (element > currentMaxElement) {
                    currentMaxElement = element;
                }
            }
            
        }
        
        maxElement = currentMaxElement;
        
        inputFile.close();
    }
    
    else
    {
        std::cout << "@ParseCSV: Error: Opening File: " << filename << std::endl;
    }
    
    
}