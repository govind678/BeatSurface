/*
  ==============================================================================

    SVMEvaluate.cpp
    Created: 13 Feb 2014 4:14:00pm
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#include "SVMEvaluate.h"


SVMEvaluate::SVMEvaluate()
{
    m_bIsInitialized  = true;
}


SVMEvaluate::~SVMEvaluate()
{
    m_bIsInitialized  = false;
}


SVMBase::Error_t SVMEvaluate::crossValidationOnTrainingDataset()
{
    if (!m_bIsInitialized) {
        std::cout << "Error: Instance not initialized" << std::endl;
        return kNotInitializedError;
    }
    
    if (!m_pSVMProblem) {
        std::cout << "Error: SVM Problem" << std::endl;
        return kUnknownError;
    }
    
    if (m_pSVMProblem->l <= 1) {
        std::cout << "Error: SVM Problem Not Initialized" << std::endl;
        return kNotInitializedError;
    }
    
    // verify parameter
    if (svm_check_parameter (m_pSVMProblem, m_pSVMParameters)) {
        std::cout << "Error: Fail SVM Parameter Check" << std::endl;
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
		printf("Cross Validation Accuracy = %g%%\n",100.0*total_correct/m_pSVMProblem->l);
	}
    
    
    
    if (pLabels)
        delete [] pLabels;
    pLabels = 0;
    
    
    
    return kNoError;
}




SVMBase::Error_t SVMEvaluate::setNumOfFolds( int iNumFolds /*= 10*/ )
{
    if (iNumFolds <= 0)
        return kInvalidFunctionParamError;
    
    m_iNumFolds = iNumFolds;
    
    return kNoError;
}




SVMBase::Error_t SVMEvaluate::evaluationOnTestDataset(float **ppfTestDataset, float *pfTargetLabels, int iNumObservations, int iNumFeatures)
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
        classify(ppfTestDataset[i], iNumFeatures);
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
    
    
    std::cout << "Accuracy: " << accuracy << std::endl;
    std::cout << "Confusion Matrix: " << std::endl;
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
