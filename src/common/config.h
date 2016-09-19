// =====================================================================================
// 
//       Filename:  config.h
// 
//    Description:  config file
// 
//        Version:  1.0
//        Created:  09/19/2016 10:45:41 AM
//       Revision:  none
//       Compiler:  g++
// 
// =====================================================================================

#ifndef COMMON_CONFIG_H_
#define COMMON_COnFIG_H_

#include "utils.h"
#include "../base_model.h"
#include "../ftrl/ftrl.h"
#include "../sgd/sgd.h"


class Config {
 public:
     Config();

     ~Config();

     BaseModel Init(int argc, char** argv);

     string TrainDir() { return trainDir_; }

     string TestDir() { return testDir_; }

     string PredictDir() { return predictDir_; }

     string Task() { return task_; }

     string ModelPath() { return modelPath_; }

     int ThreadNum() { return threadNum_; }
 private:
     string trainDir_;
     string testDir_;
     string predictDir_;
     string task_;
     string modelPath_;
     int threadNum_;
     string modelType_;
     float lambda1_;
     float lambda2_;
     float learningRate_;
     float alpha_;
     float beta_;
};




#endif
