#include <vector>

#include "fast_ftrl.h"


FastFtrlModel::FastFtrlModel(int _dim, float _lambda1, float _lambda2, float _alpha, float _beta) : dim(_dim), lambda1(_lambda1), lambda2(_lambda2), alpha(_alpha), beta(_beta) {
    w = new float[_dim]();
    z = new float[_dim]();
    n = new float[_dim]();
}

FastFtrlModel::~FastFtrlModel() {
    delete[] w;
    delete[] z;
    delete[] n;
}


void FastFtrlModel::train_single_instance(vector<int>& x, int y) {
    int sum = 0.0;
    for (unsigned int i = 0; i < x.size(); i++) {
        int idx = x[i];
        if (abs(z[idx]) < lambda1)
            w[idx] = 0;
        else
            w[idx] = -(z[idx] - sgn(z[idx]) * lambda1) / ((beta + sqrt(n[idx])) / alpha + lambda2);
        sum += w[idx];
    }
    float p = sigmod(sum);
    float g = 0.0;
    float sigma = 0.0;

    for (unsigned int i = 0; i < x.size(); i++) {
        int idx = x[i];
        g = (p - y);
        sigma = (sqrt(n[idx] + g * g) - sqrt(n[idx])) / alpha;
        z[idx] += g - sigma * w[idx];
        n[idx] += g*g;
    }
}

void FastFtrlModel::dumpw(string& filename) {
    ofstream ofile;
    ofile.open(filename.c_str(), std::fstream::out);
    //printf("%d\n", dim);
    for (int i = 0; i < dim; i++) {
        //printf("%f\n",w[i]);
        if (w[i] > 10e-10 || w[i]<-10e-10)
            ofile << i << ":" << w[i] << endl;
    }
    ofile.close();
}

//void FastFtrlModel::multithread_train(ftrl_data train_data, int thread_idx) {
    //for(unsigned int i = 0; i < train_data.x_data.size(); i++) {
    //    train_single_instance(train_data.x_data[i], train_data.y_data[i]);
    //}
//}

int FastFtrlModel::predict_single_instance(vector<int> &x) {
    float val = 0.0;
    for(unsigned int i = 0; i < x.size(); i++) {
	val += w[x[i]];
    }
    val = sigmod(val);
    if(val > 0.5)
	return 1;
    else
	return 0;
}

