
#ifndef FTRL_HPP
#define FTRL_HPP

#include<cmath>
#include<map>
#include<fstream>

using namespace std;


typedef std::map<int, int> feature_items;

inline int sgn(float x);

float sigmod(float x);

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
    FtrlModel(int _dim, float _lambda1 = 1, float _lambda2 = 1, float _alpha = 0.1, float _beta = 1);

    ~FtrlModel();

    float logistic(feature_items& x);

    bool trainSingleInstance(feature_items& x, int y);

    bool dumpw(string& filename);

    void multithread_train(
};



#endif /* FTRL_HPP */

