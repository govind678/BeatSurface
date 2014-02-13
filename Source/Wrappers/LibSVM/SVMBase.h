/*
  ==============================================================================

    SVMBase.h
    Created: 13 Feb 2014 3:16:00am
    Author:  Govinda Ram Pingali

 
  Wrapper Class for LibSVM - www.csie.ntu.edu.tw/~cjlin/libsvm
  ==============================================================================
*/

#ifndef SVMBASE_H_INCLUDED
#define SVMBASE_H_INCLUDED


#include "svm.h"
#include <string.h>


class SVMBase
{
    
public:
    
    
    SVMBase();
    ~SVMBase();
    
    
    enum Error_t
    {
        kNoError,                       //!< no error occurred
        kMemError,                      //!< memory allocation failed
        kInvalidFunctionParamError,     //!< one or more function parameters are not valid
        kNotInitializedError,           //!< instance has not been initialized yet
        kUnknownError,                  //!< unknown error occurred
        
        kNumErrors
    };
    
    
    enum SvmType_t
    {
        kSvmType_C_SVC,                 //!< classification: C-SVC
        kSvmType_NU_SVC,                //!< classification: nu-SVC
        kSvmType_ONE_CLASS,             //!< distribution estimation
        kSvmType_EPSILON_SVR,           //!< regression: epsilon-SVR
        kSvmType_NU_SVR,                //!< regression: nu-SVR
        
        kNumSvmTypes
    };
    
    
    enum SvmKernelType_t
    {
        kSvmKernelType_LINEAR,          //!< linear kernel function
        kSvmKernelType_RBF,             //!< gaussian kernel function
        
        kNumSvmKernelTypes
    };
    
    
    struct SvmParameter_t
    {
        // fundamental parameters
        SvmType_t       eSvmType;       //!< see SvmType_t
        SvmKernelType_t eSvmKernelType; //!< see SvmKernelType_t
        
        double          dGamma;	        //!< only in case of RBF
        
        // other important SVM parameters */
        double dEps;	                //<! stopping criterion
        double dC;	                    //<! penalty factor for C_SVC, EPSILON_SVR and NU_SVR. high -> overfitting, low -> underfitting
        double dNu;	                    //<! for NU_SVC, ONE_CLASS, and NU_SVR
        double dP;	                    //<! loss tolerance for EPSILON_SVR: high -> low accuracy, low number of SVs, low (0) -> overfitting; has to be smaller than dataset range and higher than noise range
    };
    
    
    
    
protected:
    
    static SvmType_t       SvmTypeInternal2External (int iInternalSvmType);
    static int             SvmTypeExternal2Internal (SvmType_t eExternalType);
    static SvmKernelType_t SvmKernelTypeInternal2External (int iInternalSvmKernelType);
    static int             SvmKernelTypeExternal2Internal (SvmKernelType_t eExternalKernelType);
    
    
    bool            m_bIsInitialized;
    
    
    
    svm_model       *m_pSVMModel;
    svm_parameter   *m_pSVMParameters;
    svm_problem     *m_pSVMProblem;
    
    double  m_dResult;
    double  *m_pdProbability;
    
    int     m_iNumFolds;
    
    
};


#endif  // SVMBASE_H_INCLUDED
