
#ifndef SGD_SGD_H_
#define SGD_SGD_H_

#include <stdlib.h>

#include <fstream>
#include <iostream>

#include "../common/math.h"
#include "../common/utils.h"
#include "../io/local_filesys.h"
#include "../io/parser.h"
#include "../base_model.h"


class SGDModel:public BaseModel {
 private:
    float learning_rate_;
    map<index_type, float> w_;
    int instance_num_;
    float current_loss_;
    float total_loss_;
 
 public:
    SGDModel(float _learning_rate = 0.0001);
    
    SetParameter(float _learning_rate);

    ~SGDModel();
    
    void CleanW();

    float AbsW();

    float AvgLoss();

    float Logistic(const fea_items& x);

    void TrainSingleInstance(const fea_items& x, int y);

    void Train(const string& train_file);

    void DumpW(const string& filename);

    float ObjectFunctionValue();

    float CurrentLoss() { return current_loss_; }

    void MultithreadTrain(const string& train_dir, const vector<string>& file_list, int thread_idx, int thread_num);
    
    void Predict(const string& test_file, const string& predict_path);

    float PredictSingleInstance(const fea_items &x);
    
    void MultithreadPredict(const string& test_dir, const vector<string>& test_list, const string& predict_dir, int thread_idx, int thread_num);
};



#endif


