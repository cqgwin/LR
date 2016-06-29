
#ifndef FTRL_HPP
#define FTRL_HPP

#include<cmath>
#include<map>
#include<fstream>

using namespace std;

inline int sgn(float x);

inline float sigmod(float x);

class FastFtrlModel {
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
    FastFtrlModel(int _dim, float _lambda1 = 1, float _lambda2 = 1, float _alpha = 0.1, float _beta = 1);

    ~FastFtrlModel();

    inline float logistic(vector<int>& x);

    void train_single_instance(vector<int>& x, int y);

    void dumpw(string& filename);

    //void multithread_train(, int thread_idx);

    int predict_single_instance(vector<int> &x);
};



#endif /* FTRL_HPP */

