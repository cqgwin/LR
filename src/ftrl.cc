#include "ftrl.h"


FtrlModel::FtrlModel(int _dim, float _lambda1, float _lambda2, float _alpha, float _beta) : dim(_dim), lambda1(_lambda1), lambda2(_lambda2), alpha(_alpha), beta(_beta) {
    w = new float[_dim]();
    z = new float[_dim]();
    n = new float[_dim]();
}

FtrlModel::~FtrlModel() {
    delete[] w;
    delete[] z;
    delete[] n;
}

float FtrlModel::logistic(feature_items& x) {
    float sum = 0;
    for (feature_items::iterator pos = x.begin(); pos != x.end(); pos++) {
        int idx = pos->first;
        int val = pos->second;
        if (idx > dim) {
            return 0;
        }
        sum += val * w[idx];
    }
    return sigmod(sum);
}

bool FtrlModel::train_single_instance(feature_items& x, int y) {
    int sum = 0.0;
    for (feature_items::iterator pos = x.begin(); pos != x.end(); pos++) {
        int i = pos->first;
        int val = pos->second;
	if(i>dim)
	    return false;
        if (abs(z[i]) < lambda1)
            w[i] = 0;
        else {
            float t = -(z[i] - sgn(z[i]) * lambda1) / ((beta + sqrt(n[i])) / alpha + lambda2);
            w[i] = t;
	}
	sum += val * w[i];
    }
    float p = sigmod(sum);
    float g = 0.0;
    float sigma = 0.0;

    for (feature_items::iterator pos = x.begin(); pos != x.end(); pos++) {
        int i = pos->first;
        int val = pos->second;
        g = (p - y) * val;
	if(i>dim)
 	    return false;
        sigma = (sqrt(n[i] + g * g) - sqrt(n[i])) / alpha;
        z[i] += g - sigma * w[i];
        n[i] += g*g;
    }
    return true;
}

bool FtrlModel::dumpw(string& filename) {
    ofstream ofile;
    ofile.open(filename.c_str(), std::fstream::out);
    //printf("%d\n", dim);
    for (int i = 0; i < dim; i++) {
        //printf("%f\n",w[i]);
        if (w[i] > 10e-10 || w[i]<-10e-10)
            ofile << i << ":" << w[i] << endl;
    }
    ofile.close();
    return true;
}

void FtrlModel::multithread_train(string path, int thread_idx) {
    ifstream ifile;
    char t_char[10];
    sprintf(t_char, "%d", thread_idx); 
    string t_str = t_char;
    ifile.open(path + t_str);
    
    if(!ifile.good())
	return;
    string line;
    while(getline(ifile, line)) {
        if(line=="")
	    break;
 	feature_items x;
	int y;
 	utils::libsvm_format_parse(line.c_str(), x, y);
        train_single_instance(x, y);
    }
}

float FtrlModel::predict_single_instance(feature_items &x) {
    float val = 0.0;
    for(feature_items::iterator pos = x.begin(); pos != x.end(); pos++) {
	val += w[pos->first] * pos->second;
    }
    float result = sigmod(val);
    return val;
    if(result > 0)
	return 1;
    else
	return -1;
}

void FtrlModel::multithread_predict(string test_path,int thread_idx, vector<float>* predict, vector<int>* Y){
    ifstream ifile;
    char t_char[10];
    sprintf(t_char, "%d", thread_idx);
    string t_str = t_char;
    ifile.open(test_path + t_str);
    if(!ifile.good())
	return;
    string line;
    int idx = 0;
    while(getline(ifile, line)) {
	if(line=="")
	    break;
        idx++;
        feature_items x;
        int y;
        utils::libsvm_format_parse(line.c_str(), x, y);
        Y->push_back(y);
	
	predict->push_back(predict_single_instance(x));
    }
}

