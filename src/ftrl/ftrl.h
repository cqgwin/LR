
#ifndef FTRL_FTRL_H_
#define FTRL_FTRL_H_

#include <stdlib.h>

#include <fstream>
#include <iostream>

#include "../common/math.h"
#include "../common/utils.h"
#include "../io/local_filesys.h"
#include "../io/parser.h"
#include "../base_model.h"

struct pnode{
    float w;
    float z;
    float n;
};

class FtrlModel:public BaseModel {
private:
    map<index_type, pnode> p_;
    float lambda1_;
    float lambda2_;
    float alpha_;
    float beta_;
    float total_loss_;
    int instance_num_;
    float current_loss_;
public:
    FtrlModel(float _lambda1 = 0, float _lambda2 = 0, float _alpha = 0.005, float _beta = 0.1);
    
    void SetParameter(float _lambda1 = 0, float _lambda2 = 0, float _alpha = 0.005, float _beta = 0.1);

    ~FtrlModel();
      
    float AbsW();

    float AvgLoss();

    float Logistic(const fea_items& x);

    void TrainSingleInstance(const fea_items& x, int y);

    void Train(const string& train_file);

    void SaveModel(const string& model_path);

    void LoadModel(const string& model_path);

    float ObjectFunctionValue();

    float CurrentLoss() { return current_loss_; }

    void MultithreadTrain(const string& train_dir, const vector<string>& file_list, int thread_idx, int thread_num);
    
    void Predict(const string& test_file, const string& predict_path);

    float PredictSingleInstance(const fea_items &x);
    
    void MultithreadPredict(const string& test_dir, const vector<string>& test_list, const string& predict_dir, int thread_idx, int thread_num);
};


#endif


