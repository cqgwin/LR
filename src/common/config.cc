// =====================================================================================
// 
//       Filename:  config.cc
// 
//    Description:  implementation of config
// 
//        Version:  1.0
//        Created:  09/19/2016 04:36:44 PM
//       Revision:  none
//       Compiler:  g++
// 
// =====================================================================================

#include "config.h"

Config::Config() {
    trainDir_ = "";
    testDir_ = "";
    predictDir_ = "";
    task_ = "";
    modelPath_ = "";
    threadNum_ = 1;
    modelType_ = "";
    lambda1_ = 0;
    lambda2_ = 0;
    learningRate_ = 0.0001;
    alpha_ = 0.005;
    beta_ = 0.1;
}


string DirEndCheck(char *s) {
    int len = strlen(s);
    string t = s;
    if (s[len-1] != '/')
        t += "/";
    return t;
}

BaseModel Config::Init(int argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
         vector<string> items = Split(argv[i], ':');
         if(items[0] == "trainDir") {
             trainDir_ = DirEndCheck(items[1]); 
         } else if (items[0] == "testDir") {
             testDir_ = DirEndCheck(items[1]);
         } else if (items[0] == "predictDir") {
             predictDir_ = DirEndCheck(items[1]);
         } else if (items[0] == "task") {
             task_ = string(items[1]);
         } else if (items[0] == "modelPath") {
             modelPath_ = string(items[1]);
         } else if (items[0] == "threadNum") {
             threadNum_ = atoi(items[1]);
         } else if (items[0] == "modelType") {
             modelType_ = items[1];
         } else if (items[0] == "lambda1") {
             lambda1_ = atof(items[1]);
         } else if (items[0] == "lambda2") {
             lambda2_ = atof(items[1]);
         } else if (items[0] == "learningRate") {
             learningRate_ = atof(items[1]);
         } else if (items[0] == "alpha") {
             alpha_ = atof(items[1]);
         } else if (items[0] == "beta") {
             beta_ = atof(items[1]);
         } else {
             printf("%s Input arguments error!\n", argv[i]);
             return NULL;
         }
    }
    BaseModel learner;
    if (modelType_ == "ftrl") {
        learner = new FTRLModel(lambda1_, lambda2_, alpha, beta_); 
    } else if (modelType == "sgd") {
        learner = new SGDModel(learningRate_);
    } else if (modelType_ == "") {
        printf("modelType is empty!\n");
        return NULL;
    } else {
        printf("%s is not support model!\n", modelType_.c_str());
        return NULL;
    }

    if (task_ == "train") {
        if (trainDir_ == "") {
            printf("trainDir is empty!\n");
            return NULL;
        }
    } else if (task_ == "test") {
        if (modelPath_ == "") {
            printf("modelDir is empty!\n");
            return NULL;
        }
        if (testDir_ == "") {
            printf("testDir is empty!\n");
            return NULL;
        }
        if (predictDir_ == "") {
            printf("predictDir is empty!\n");
            return NULL;
        }
    } else if (task_ == "") { 
        printf("task is empty!\n");
        return NULL;
    } else {
        printf("%s if not support task!\n", task_.c_str());
        return NULL;
    }
    return learner;
}
