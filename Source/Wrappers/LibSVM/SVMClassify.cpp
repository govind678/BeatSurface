/*
  ==============================================================================

    SVMClassify.cpp
    Created: 13 Feb 2014 3:16:19am
    Author:  Govinda Ram Pingali

  ==============================================================================
*/


#include "SVMClassify.h"


SVMClassify::SVMClassify()
{
    m_bIsInitialized  = true;
}



SVMClassify::~SVMClassify()
{
    m_bIsInitialized    = false;
    
}




double* SVMClassify::classify(float *pfFeatures, int iNumFeatures)
{
    
    if (!m_bIsInitialized)
        return 0;
    
    int         iNodeIdx        = 0;
    svm_node    *pSvmFeatures   = new svm_node [iNumFeatures + 1];
    
    // copy feature data to svm array
    for (int i = 0; i < iNumFeatures; i++)
    {
        // libsvm expects "sparse" vectors e.g. elements with a value of 0 are left out
        if (pfFeatures[i] == 0)
            continue;
        pSvmFeatures[iNodeIdx].index    = i;
        pSvmFeatures[iNodeIdx].value    = pfFeatures[i];
        iNodeIdx++;
    }
    pSvmFeatures[iNodeIdx].index    = -1;
    
    
    m_dResult  = svm_predict_probability (m_pSVMModel, pSvmFeatures, m_pdProbability);
    
    delete [] pSvmFeatures;
    
    return m_pdProbability;
}




double SVMClassify::getResult( double *pdProbability /*= 0*/ )
{
    if (pdProbability)
        memcpy (pdProbability, m_pdProbability, sizeof(double)*m_pSVMModel->nr_class);
    
    return m_dResult;
}



double* SVMClassify::getProbability()
{
    return m_pdProbability;
}



int SVMClassify::getNumSvElements(std::string strSvString )
{
    int iNumElements = 0;
    for (unsigned int i=0; i<strSvString.size(); i++)
    {
        if (strSvString[i] == ':') iNumElements++;
    }
    return iNumElements;
}


SVMBase::Error_t SVMClassify::loadModelFromDisk(std::string modelFilePath)
{
    
    if((m_pSVMModel = svm_load_model((char*)modelFilePath.c_str())) == 0) {
        std::cout << "Error: Cannot Load SVM model" << std::endl;
        return kUnknownError;
    }
    
    m_pdProbability = new double [m_pSVMModel->nr_class];
    memset (m_pdProbability, 0, sizeof(double)*m_pSVMModel->nr_class);
    
    return kNoError;
}
