
#ifndef FAST_FTRL_HPP
#define FAST_FTRL_HPP

#include<cmath>
#include<map>
#include<fstream>
#include<iostream>
#include<vector>
#include<string>
#include<stdlib.h>
#include"utils.h"

using namespace std;


inline int sgn(float x) {
    if (x > 0)
        return 1;
    if (x < 0)
        return -1;
    return 0;
}

inline float sigmod(float x) {
    if (x < -35)
        x = -35;
    if (x > 35)
        x = 35;
    return 1 / (1 + exp(-1.0 * x));
}

class FtrlModel {
private:
    float* z;
    float* n;
    int dim;
    float* w;
    float p;
    float lambda1;
    float lambda2;
    float alpha;
    float beta;
public:
    FtrlModel(int _dim, float _lambda1 = 0.001, float _lambda2 = 0.001, float _alpha = 0.05, float _beta = 1);

    ~FtrlModel();
    
    void clean_w();

    float logistic(fea_items& x);

    bool train_single_instance(fea_items& x, int y);

    bool dumpw(string& filename);

    void multithread_train(string path, int thread_idx);

    float predict_single_instance(fea_items &x);
    
    void multithread_predict(string test_path,int thread_idx, vector<float>* predict, vector<int>* Y);
};



#endif


