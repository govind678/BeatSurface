/*
  ==============================================================================

    SVMTrain.cpp
    Created: 13 Feb 2014 3:16:11am
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#include "SVMTrain.h"


SVMTrain::SVMTrain()
{
    // add initialization
    setDefaultParameters();
    
    //Error_t eError = SetTrainData (ppfFeatureData, pfTargets, iNumFeatures, iNumObservations);
    
    m_bIsInitialized  = true;
    
}


SVMTrain::~SVMTrain()
{
    m_bIsInitialized    = false;
    
    if (m_pdProbability != NULL) {
        delete [] m_pdProbability;
    }
    m_pdProbability     = NULL;
}



void SVMTrain::setDefaultParameters ()
{
    if (!m_pSVMParameters)
    {
        return;
    }
    
    m_pSVMParameters->svm_type      = C_SVC;
    m_pSVMParameters->kernel_type   = RBF;
    m_pSVMParameters->degree        = 3;
    m_pSVMParameters->gamma         = 0.3;	// 1/num_features
    m_pSVMParameters->coef0         = 0;
    m_pSVMParameters->nu            = 0.5;
    m_pSVMParameters->cache_size    = 100;
    m_pSVMParameters->C             = 1;
    m_pSVMParameters->eps           = 1e-3;
    m_pSVMParameters->p             = 0.1;
    m_pSVMParameters->shrinking     = 1;
    m_pSVMParameters->probability   = 1;
    m_pSVMParameters->nr_weight     = 0;
    m_pSVMParameters->weight_label  = NULL;
    m_pSVMParameters->weight        = NULL;
}




SVMBase::Error_t SVMTrain::setParameters(const SvmParameter_t &stSvmParam)
{
    /////////////////////////////////////////////////////////
    // SvmParameter_t
    m_pSVMParameters->svm_type    = SvmTypeExternal2Internal (stSvmParam.eSvmType);
    m_pSVMParameters->kernel_type = SvmKernelTypeExternal2Internal (stSvmParam.eSvmKernelType);
    m_pSVMParameters->gamma       = stSvmParam.dGamma;
    
    /////////////////////////////////////////////////////////
    m_pSVMParameters->eps     = stSvmParam.dEps;
    m_pSVMParameters->C       = stSvmParam.dC;
    m_pSVMParameters->nu      = stSvmParam.dNu;
    m_pSVMParameters->p       = stSvmParam.dP;
    
    return kNoError;
}





SVMBase::Error_t SVMTrain::setTrainingDataAndTrain(float **ppfFeatureData, float *pfTargets, int iNumFeatures, int iNumObservations)
{
    
    int i, iCurrCol = 0;
    
    if (!ppfFeatureData || !pfTargets) {
        std::cout << "@SVMTrain: Error: Invalid Function Parameter" << std::endl;
        return kInvalidFunctionParamError;
    }
    
    
    if (iNumFeatures <= 0 || iNumObservations <= 0) {
        std::cout << "@SVMTrain: Error: Invalid Function Parameter" << std::endl;
        return kInvalidFunctionParamError;
    }
    
    
    m_pSVMProblem->l  = iNumObservations;
    
    // alloc memory
    m_pSVMProblem->y  = new double [iNumObservations];
    m_pSVMProblem->x  = new svm_node* [iNumObservations];
    
    if (!m_pSVMProblem->y || !m_pSVMProblem->x) {
        std::cout << "@SVMTrain: Error: Memory Error" << std::endl;
        return kMemError;
    }
    
    
    for (i = 0; i < iNumObservations; i++)
    {
        m_pSVMProblem->x[i]   = new svm_node [iNumFeatures+1];
        if (!m_pSVMProblem->x[i]) {
            std::cout << "@SVMTrain: Error: Memory Error" << std::endl;
            return kMemError;
        }
    }
    
    
    
    
    //--- Copy Data to SVM Problem ---/
    
    for (i = 0; i < iNumObservations; i++)
    {
        int j;
        iCurrCol    = 0;
        m_pSVMProblem->y[i]   = pfTargets[i];
        for (j = 0; j < iNumFeatures; j++)
        {
            double dVal = ppfFeatureData[i][j];             // Switching index order: rows -> observations, columns -> features
            if (dVal == 0)
                continue;
            else
            {
                m_pSVMProblem->x[i][iCurrCol].index   = j;
                m_pSVMProblem->x[i][iCurrCol].value   = dVal;
                iCurrCol++;
            }
        }
        m_pSVMProblem->x[i][iCurrCol].index   = -1;
    }
    
    if (m_pSVMParameters->gamma == 0.0)
    {
        m_pSVMParameters->gamma   = 1.0/iNumFeatures;
    }
    
    
    
    
    //--- Train SVM Model ---//
    
    if (!m_bIsInitialized)
        return kNotInitializedError;
    
    if (!m_pSVMProblem)
        return kUnknownError;
    
    if (m_pSVMProblem->l <= 1)
        return kNotInitializedError;
    
    // verify parameter
    if (svm_check_parameter (m_pSVMProblem, m_pSVMParameters))
        return kUnknownError;
    
    // train
    m_pSVMModel   = svm_train(m_pSVMProblem, m_pSVMParameters);
    
    
    
    if (m_pdProbability != nullptr) {
        delete [] m_pdProbability;
    }
    
    m_pdProbability = new double [m_pSVMModel->nr_class];
    memset (m_pdProbability, 0, sizeof(double)*m_pSVMModel->nr_class);
    
    
    return kNoError;
}





SVMBase::Error_t SVMTrain::saveModelToDisk(std::string modelFilePath)
{
    if(svm_save_model((char*)modelFilePath.c_str(), m_pSVMModel)) {
        std::cout << "@SVMTrain: Error: Cannot Save Model" << std::endl;
        return kUnknownError;
    }
    
    return kNoError;
}



double* SVMTrain::classify(float *pfFeatures, int iNumFeatures)
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




double SVMTrain::getResult( double *pdProbability /*= 0*/ )
{
    if (pdProbability)
        memcpy (pdProbability, m_pdProbability, sizeof(double)*m_pSVMModel->nr_class);
    
    return m_dResult;
}



double* SVMTrain::getProbability()
{
    return m_pdProbability;
}



int SVMTrain::getNumSvElements(std::string strSvString )
{
    int iNumElements = 0;
    for (unsigned int i=0; i<strSvString.size(); i++)
    {
        if (strSvString[i] == ':') iNumElements++;
    }
    return iNumElements;
}


SVMBase::Error_t SVMTrain::loadModelFromDisk(std::string modelFilePath)
{
    
    if((m_pSVMModel = svm_load_model((char*)modelFilePath.c_str())) == 0) {
        std::cout << "@SVMClassify: Error: Cannot Load SVM model" << std::endl;
        return kUnknownError;
    }
    
    if (m_pdProbability != nullptr) {
        delete [] m_pdProbability;
    }
    
    m_pdProbability = new double [m_pSVMModel->nr_class];
    memset (m_pdProbability, 0, sizeof(double)*m_pSVMModel->nr_class);
    
    return kNoError;
}