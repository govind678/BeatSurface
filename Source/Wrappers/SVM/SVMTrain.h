/*
  ==============================================================================

    SVMTrain.h
    Created: 13 Feb 2014 3:16:11am
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#ifndef SVMTRAIN_H_INCLUDED
#define SVMTRAIN_H_INCLUDED


#include <vector>
#include <iostream>
#include <iterator>
#include <string.h>

#include "svm.h"

using std::vector;

class SVMTrain
{
    
public:
    
    SVMTrain();
    ~SVMTrain();
    
    
    
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
    
    
    
    
    
    Error_t setTrainingDataAndTrain(vector<vector<double>> ppdTrainingData,
                                    vector<int> piTrainingClassLabels,
                                    int iNumFeatures,
                                    int iNumObservations);
    
    Error_t setParameters(const SvmParameter_t &stSvmParam);
    
    
    double classify(double* pfFeatures, double* pdProbability = 0);
    
    int getNumSvElements( std::string strSvString );
    
    Error_t saveModelToDisk(std::string modelFilePath);
    Error_t loadModelFromDisk(std::string modelFilePath);
    
    
    Error_t crossValidationOnTrainingDataset(int numFolds, double& accuracy);
    Error_t evaluationOnTestDataset(double** ppfTestDataset, double* pfTargetLabels, int iNumObservations, int iNumFeatures);
    Error_t setNumOfFolds (int iNumFolds = 10);
    
    
private:
    
    
    static SvmType_t       SvmTypeInternal2External (int iInternalSvmType);
    static int             SvmTypeExternal2Internal (SvmType_t eExternalType);
    static SvmKernelType_t SvmKernelTypeInternal2External (int iInternalSvmKernelType);
    static int             SvmKernelTypeExternal2Internal (SvmKernelType_t eExternalKernelType);
    
    
    
    svm_model       *m_pSVMModel;
    svm_parameter   *m_pSVMParameters;
    svm_problem     *m_pSVMProblem;
    
    double  m_dResult;
    double  *m_pdProbability;
    
    int     m_iNumFolds;
    
    
    
    
    void setDefaultParameters();
    
    vector<double> m_pdProbabilityEstimate;
    
    int     m_iNumFeatures;
    
    svm_node    *m_pSvmFeatures;
    vector<svm_node>    m_pcSvmFeatures;
    
};





#endif  // SVMTRAIN_H_INCLUDED
