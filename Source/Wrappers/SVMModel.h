/*
  ==============================================================================

    SVMModel.h
    Created: 18 Jan 2014 2:19:31am
    Author:  Govinda Ram Pingali

    Wrapper Class for libsvm 3.17 : http://www.csie.ntu.edu.tw/~cjlin/libsvm
  ==============================================================================
*/

#ifndef SVMMODEL_H_INCLUDED
#define SVMMODEL_H_INCLUDED


#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include "svm.h"

#define Malloc(type,n) (type *)malloc((n)*sizeof(type))

class SVMModel {
    
public:
    
    SVMModel(const char *trainingFilename, const char *modelFilename);
    ~SVMModel();
    
    
private:
    
    void read_problem(const char *filename);
    void predict(FILE *input, FILE *output);
    char* readline(FILE *input);
    void exit_input_error(int line_num);
    const char* svm_check_parameter(const svm_problem *prob, const svm_parameter *param);
    void do_cross_validation();
    
    int (*info)(const char *fmt,...) = &printf;
    
    struct svm_parameter param;
    struct svm_problem prob;
    struct svm_model *model;
    struct svm_node *x_space;
    struct svm_node *x;
    int predict_probability=0;
    int cross_validation;
    int nr_fold = 10;
    int max_nr_attr = 64;
    
    char *line;
    int max_line_len;
    
};





#endif  // SVMMODEL_H_INCLUDED
