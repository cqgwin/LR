
#ifndef FTRL_H_
#define FTRL_H_

#include <cmath>
#include <stdlib.h>

#include <fstream>
#include <iostream>

#include "../common/utils.h"
#include "../io/local_filesys.h"
#include "../io/parser.h"


struct pnode{
    float w;
    float z;
    float n;
};

inline int Sgn(float x) {
    if (x < 0.)
        return -1.;
    else
        return 1.;
}

inline float Sigmoid(float x) {
    if (x < -35)
        x = -35;
    if (x > 35)
        x = 35;
    return 1 / (1 + exp(-1.0 * x));
}

inline float LogLoss(float p, float y) {
    if(p < 10e-15)
        p = 10e-15;
    if(p > 1 - 10e-15)
        p = 1 - 10e-15;
    return -y * log(p) - (1-y) * log(1-p);
}

class FtrlModel {
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

    ~FtrlModel();
    
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


