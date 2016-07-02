
#ifndef FTRL_HPP
#define FTRL_HPP

#include<cmath>
#include<map>
#include<fstream>

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

    inline float logistic(vector<int>& x) {
		float sum = 0;
		for (unsigned int i = 0; i < x.size(); i++) {
			sum += w[x[i]];
		}
		return sigmod(sum);
	}

    void train_single_instance(vector<int>& x, int y);

    void dumpw(string& filename);

    //void multithread_train(, int thread_idx);

    int predict_single_instance(vector<int> &x);
};



#endif /* FTRL_HPP */
