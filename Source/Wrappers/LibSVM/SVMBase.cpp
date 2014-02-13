/*
  ==============================================================================

    SVMBase.cpp
    Created: 13 Feb 2014 3:16:00am
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#include "SVMBase.h"



SVMBase::SVMBase()
{
    m_pSVMParameters  = 0;
    m_pSVMProblem = 0;
    m_pSVMModel   = 0;
    
    m_pSVMParameters    = new svm_parameter;
    memset (m_pSVMParameters, 0, sizeof(svm_parameter));
    
    m_pSVMProblem       = new svm_problem;
    memset (m_pSVMProblem, 0, sizeof(svm_problem));
    
    m_pSVMModel         = new svm_model;
    memset (m_pSVMModel, 0, sizeof(svm_model));
    
    
    
    m_pdProbability     = 0;
    m_dResult           = 0;
    
    
    m_bIsInitialized  = true;
    
}


SVMBase::~SVMBase()
{
    svm_free_and_destroy_model (&m_pSVMModel);
    
    if (m_pSVMProblem)
    {
        for (int i = 0; i < m_pSVMProblem->l; i++)
            delete [] m_pSVMProblem->x[i];
        delete [] m_pSVMProblem->x;
        delete [] m_pSVMProblem->y;
    }
    
    
    
    delete [] m_pdProbability;
    m_pdProbability     = 0;
    
    
    delete m_pSVMProblem;
    m_pSVMProblem = 0;
    
    
    
    m_bIsInitialized    = false;
    
}








//--- Internal Methods to Convert SVMBase Enums to LibSVM Type ---//



SVMBase::SvmType_t SVMBase::SvmTypeInternal2External( int iInternalSvmType )
{
    switch (iInternalSvmType)
    {
        default:
        case C_SVC:
            return kSvmType_C_SVC;
        case NU_SVC:
            return kSvmType_NU_SVC;
        case ONE_CLASS:
            return kSvmType_ONE_CLASS;
        case EPSILON_SVR:
            return kSvmType_EPSILON_SVR;
        case NU_SVR:
            return kSvmType_NU_SVR;
    }
}




int SVMBase::SvmTypeExternal2Internal( SvmType_t eExternalType )
{
    switch (eExternalType)
    {
        default:
        case kSvmType_C_SVC:
            return C_SVC;
        case kSvmType_NU_SVC:
            return NU_SVC;
        case kSvmType_ONE_CLASS:
            return ONE_CLASS;
        case kSvmType_EPSILON_SVR:
            return EPSILON_SVR;
        case kSvmType_NU_SVR:
            return NU_SVR;
    }
    
}



SVMBase::SvmKernelType_t SVMBase::SvmKernelTypeInternal2External( int iInternalSvmKernelType )
{
    switch (iInternalSvmKernelType)
    {
        default:
        case LINEAR:
            return kSvmKernelType_LINEAR;
        case RBF:
            return kSvmKernelType_RBF;
    }
}



int SVMBase::SvmKernelTypeExternal2Internal( SvmKernelType_t eExternalKernelType )
{
    switch (eExternalKernelType)
    {
        default:
        case kSvmKernelType_LINEAR:
            return LINEAR;
        case kSvmKernelType_RBF:
            return RBF;
    }
}
