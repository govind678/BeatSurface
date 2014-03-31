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
    
    
    
    // add initialization
    setDefaultParameters();
    
    
    
    //Error_t eError = SetTrainData (ppfFeatureData, pfTargets, iNumFeatures, iNumObservations);
    
    m_pSvmFeatures = NULL;
    m_pdProbabilityEstimate.assign(0, 0.0f);
    
}


SVMTrain::~SVMTrain()
{
    
    if (m_pdProbability != NULL) {
        delete [] m_pdProbability;
    }
    m_pdProbability     = NULL;
    
    if (m_pSvmFeatures != NULL) {
        delete [] m_pSvmFeatures;
    }
    m_pSvmFeatures = NULL;
    
    
    svm_free_and_destroy_model (&m_pSVMModel);
    
    if (m_pSVMProblem)
    {
        for (int i = 0; i < m_pSVMProblem->l; i++)
            delete [] m_pSVMProblem->x[i];
        delete [] m_pSVMProblem->x;
        delete [] m_pSVMProblem->y;
    }
    
    
    
    
    if (m_pSVMProblem != NULL) {
        delete m_pSVMProblem;
    }
    
    m_pSVMProblem = NULL;
    
}



void SVMTrain::setDefaultParameters ()
{
    if (!m_pSVMParameters)
    {
        return;
    }
    
    m_pSVMParameters->svm_type      = NU_SVC;
    m_pSVMParameters->kernel_type   = RBF;
    m_pSVMParameters->degree        = 3;
    m_pSVMParameters->gamma         = 0.3;	// 1/num_features
    m_pSVMParameters->coef0         = 0;
    m_pSVMParameters->nu            = 0.5;
    m_pSVMParameters->cache_size    = 500;
    m_pSVMParameters->C             = 1;
    m_pSVMParameters->eps           = 1e-3;
    m_pSVMParameters->p             = 0.1;
    m_pSVMParameters->shrinking     = 1;
    m_pSVMParameters->probability   = 1;
    m_pSVMParameters->nr_weight     = 0;
    m_pSVMParameters->weight_label  = NULL;
    m_pSVMParameters->weight        = NULL;
}




SVMTrain::Error_t SVMTrain::setParameters(const SvmParameter_t &stSvmParam)
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






SVMTrain::Error_t SVMTrain::setTrainingDataAndTrain(vector<vector<double>> ppdTrainingData,
                                                   vector<int> piTrainingClassLabels,
                                                   int iNumFeatures,
                                                   int iNumObservations)

{
    
    int i, iCurrCol = 0;
    
    if (ppdTrainingData.empty() || piTrainingClassLabels.empty())
    {
        std::cout << "@SVMTrain: Error: Invalid Function Parameter" << std::endl;
        return kInvalidFunctionParamError;
    }
    
    
    if (iNumFeatures <= 0 || iNumObservations <= 0) {
        std::cout << "@SVMTrain: Error: Invalid Function Parameter" << std::endl;
        return kInvalidFunctionParamError;
    }
    
    
    m_pSVMProblem->l    = iNumObservations;
    m_iNumFeatures      = iNumFeatures;
    
    // alloc memory
    m_pSVMProblem->y    = new double [iNumObservations];
    m_pSVMProblem->x    = new svm_node* [iNumObservations];
    m_pSvmFeatures      = new svm_node [iNumFeatures + 1];
    
    if (!m_pSVMProblem->y || !m_pSVMProblem->x)
    {
        std::cout << "@SVMTrain: Error: Memory Error" << std::endl;
        return kMemError;
    }
    
    
    for (i = 0; i < iNumObservations; i++)
    {
        m_pSVMProblem->x[i]   = new svm_node [iNumFeatures+1];
        if (!m_pSVMProblem->x[i])
        {
            std::cout << "@SVMTrain: Error: Memory Error" << std::endl;
            return kMemError;
        }
    }
    
    
    
    
    //--- Copy Data to SVM Problem ---/
    
    for (i = 0; i < iNumObservations; i++)
    {
        int j;
        iCurrCol    = 0;
        m_pSVMProblem->y[i]   = piTrainingClassLabels.at(i);
        
        for (j = 0; j < iNumFeatures; j++)
        {
            double dVal = ppdTrainingData.at(i).at(j);     // Switching index order: rows -> observations, columns -> features
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
    
    if (m_pSVMProblem == NULL)
    {
        std::cout << "@SVMTrain: Error: With SVM Problem" << std::endl;
        return kUnknownError;
    }
    
    if (m_pSVMProblem->l <= 1)
    {
        std::cout << "@SVMTrain: Error: SVs <= 1" << std::endl;
        return kNotInitializedError;
    }
    
    // verify parameter
    if (svm_check_parameter (m_pSVMProblem, m_pSVMParameters))
    {
        std::cout << "@SVMTrain: Error: With Parameters" << std::endl;
        return kUnknownError;
    }
    
    // train
    m_pSVMModel   = svm_train(m_pSVMProblem, m_pSVMParameters);
    
    
    
    if (m_pdProbability != nullptr)
    {
        delete [] m_pdProbability;
    }
    
    m_pdProbability = new double [m_pSVMModel->nr_class];
    memset (m_pdProbability, 0, sizeof(double)*m_pSVMModel->nr_class);
    
    
    return kNoError;
}





SVMTrain::Error_t SVMTrain::saveModelToDisk(std::string modelFilePath)
{
    if(svm_save_model((char*)modelFilePath.c_str(), m_pSVMModel)) {
        std::cout << "@SVMTrain: Error: Cannot Save Model" << std::endl;
        return kUnknownError;
    }
    
    return kNoError;
}



double SVMTrain::classify(double* pfFeatures, double* pdProbability /*= 0*/)
{
    
    int iNodeIdx        = 0;
    
    // copy feature data to svm array
    for (int i = 0; i < m_iNumFeatures; i++)
    {
        // libsvm expects "sparse" vectors e.g. elements with a value of 0 are left out
        if (pfFeatures[i] == 0)
            continue;
        
        m_pSvmFeatures[iNodeIdx].index    = i;
        m_pSvmFeatures[iNodeIdx].value    = pfFeatures[i];
        iNodeIdx++;
    }
    m_pSvmFeatures[iNodeIdx].index    = -1;
    
    
    m_dResult  = svm_predict_probability (m_pSVMModel, m_pSvmFeatures, m_pdProbability);
    
    if (pdProbability)
        memcpy (pdProbability, m_pdProbability, sizeof(double)*m_pSVMModel->nr_class);
    
    return m_dResult;
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




SVMTrain::Error_t SVMTrain::loadModelFromDisk(std::string modelFilePath)
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


SVMTrain::Error_t SVMTrain::crossValidationOnTrainingDataset(int numFolds, double& accuracy)
{
    if (numFolds <= 0)
        return kInvalidFunctionParamError;
    
    m_iNumFolds = numFolds;
    
    
    if (!m_pSVMProblem) {
        std::cout << "@SVMEvaluate: Error: SVM Problem" << std::endl;
        return kUnknownError;
    }
    
    if (m_pSVMProblem->l <= 1) {
        std::cout << "@SVMEvaluate: Error: SVM Problem Not Initialized" << std::endl;
        return kNotInitializedError;
    }
    
    // verify parameter
    if (svm_check_parameter (m_pSVMProblem, m_pSVMParameters)) {
        std::cout << "@SVMEvaluate: Error: Fail SVM Parameter Check" << std::endl;
        return kUnknownError;
    }
    
    double* pLabels   = new double [m_pSVMProblem->l];
    
    // cross validation
    svm_cross_validation (  m_pSVMProblem,
                          m_pSVMParameters,
                          m_iNumFolds,
                          pLabels);
    
    
    
    
    double total_error = 0;
    int total_correct = 0;
    double sumv = 0, sumy = 0, sumvv = 0, sumyy = 0, sumvy = 0;
    if(m_pSVMParameters->svm_type == EPSILON_SVR ||
	   m_pSVMParameters->svm_type == NU_SVR)
	{
		for(int i=0 ; i<m_pSVMProblem->l ; i++)
		{
			double y = m_pSVMProblem->y[i];
			double v = pLabels[i];
			total_error += (v-y)*(v-y);
			sumv += v;
			sumy += y;
			sumvv += v*v;
			sumyy += y*y;
			sumvy += v*y;
		}
		printf("Cross Validation Mean squared error = %g\n",total_error/m_pSVMProblem->l);
		printf("Cross Validation Squared correlation coefficient = %g\n",
               ((m_pSVMProblem->l*sumvy-sumv*sumy)*(m_pSVMProblem->l*sumvy-sumv*sumy))/
               ((m_pSVMProblem->l*sumvv-sumv*sumv)*(m_pSVMProblem->l*sumyy-sumy*sumy))
               );
	}
	else
	{
		for(int i=0 ; i<m_pSVMProblem->l ; i++)
			if(pLabels[i] == m_pSVMProblem->y[i])
				++total_correct;
		
        accuracy = double(100.0f * total_correct) /m_pSVMProblem->l;
        
        printf("Cross Validation Accuracy = %g%%\n",accuracy);
        
	}
    
    
    
    if (pLabels)
        delete [] pLabels;
    pLabels = 0;
    
    
    
    return kNoError;
}




SVMTrain::Error_t SVMTrain::setNumOfFolds( int iNumFolds /*= 10*/ )
{
    if (iNumFolds <= 0)
        return kInvalidFunctionParamError;
    
    m_iNumFolds = iNumFolds;
    
    return kNoError;
}




SVMTrain::Error_t SVMTrain::evaluationOnTestDataset(double **ppfTestDataset, double *pfTargetLabels, int iNumObservations, int iNumFeatures)
{
    float accuracy = 0;
    float** ppfConfusionMatrix;
    
    ppfConfusionMatrix = new float*[m_pSVMModel->nr_class];
    
    for (int i=0; i < m_pSVMModel->nr_class; i++) {
        
        ppfConfusionMatrix[i] = new float[m_pSVMModel->nr_class];
        
        for (int j=0; j < m_pSVMModel->nr_class; j++) {
            ppfConfusionMatrix[i][j] = 0;
        }
    }
    
    float* pfResultLabels = new float[iNumObservations];
    
    
    for (int i=0; i < iNumObservations; i++) {
        classify(ppfTestDataset[i]);
        pfResultLabels[i] = m_dResult;
        
        if (pfTargetLabels[i] == m_dResult) {
            accuracy += 1;
        }
    }
    
    accuracy = (accuracy / iNumObservations) * 100;
    
    
    //---- MATLAB -----//
    // confusionMatrix(predictedLabel(i),loops_labels(i)) = confusionMatrix(predictedLabel(i),loops_labels(i)) + 1;
    //----------------//
    
    for (int i=0; i < iNumObservations; i++) {
        ppfConfusionMatrix[int(pfResultLabels[i]-1)][int(pfTargetLabels[i]-1)] += 1;
    }
    
    
    std::cout << "@SVMEvaluate: Accuracy: " << accuracy << std::endl;
    std::cout << "@SVMEvaluate: Confusion Matrix: " << std::endl;
    for (int i=0; i < m_pSVMModel->nr_class; i++) {
        for (int j=0; j < m_pSVMModel->nr_class; j++) {
            std::cout << ppfConfusionMatrix[i][j] << "\t";
        }
        std::cout<<std::endl;
    }
    
    
    delete [] ppfConfusionMatrix;
    delete [] pfResultLabels;
    
    
    return kNoError;
}





//--- Internal Methods to Convert SVMBase Enums to LibSVM Type ---//



SVMTrain::SvmType_t SVMTrain::SvmTypeInternal2External( int iInternalSvmType )
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




int SVMTrain::SvmTypeExternal2Internal( SvmType_t eExternalType )
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



SVMTrain::SvmKernelType_t SVMTrain::SvmKernelTypeInternal2External( int iInternalSvmKernelType )
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



int SVMTrain::SvmKernelTypeExternal2Internal( SvmKernelType_t eExternalKernelType )
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
