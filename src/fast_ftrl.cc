#include "fast_ftrl.h"


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

float FtrlModel::logistic(fea_items& x) {
    float sum = 0;
    for (fea_items::iterator pos = x.begin(); pos != x.end(); pos++) {
        int idx = *pos;
        if (idx > dim) {
            return 0;
        }
        sum += w[idx];
    }
    return sigmod(sum);
}

bool FtrlModel::train_single_instance(fea_items& x, int y) {
    int sum = 0.0;
    for (fea_items::iterator pos = x.begin(); pos != x.end(); pos++) {
        int i = *pos;
         
	sum += w[i];
	if(i>dim)
	    return false;
        if (abs(z[i]) < lambda1)
            w[i] = 0;
        else {
            float t = -(z[i] - sgn(z[i]) * lambda1) / ((beta + sqrt(n[i])) / alpha + lambda2);
            w[i] = t;
	}
	//sum += w[i];
    }
    float p = sigmod(sum);
    float g = 0.0;
    float sigma = 0.0;

    for (fea_items::iterator pos = x.begin(); pos != x.end(); pos++) {
        int i = *pos;
       	
        g = (p - y);
	if(i>dim)
 	    return false;
        sigma = (sqrt(n[i] + g * g) - sqrt(n[i])) / alpha;
        z[i] += g - sigma * w[i];
        n[i] += g*g;
    }
    return true;
}

void FtrlModel::clean_w() {
    for(int i = 0; i < dim; i++) {
	if(!(w[i] > 10e-10 || w[i] < -10e-10))
	    w[i] = 0;
    }
}

bool FtrlModel::dumpw(string& filename) {
    ofstream ofile;
    ofile.open(filename.c_str(), std::fstream::out);
    for (int i = 0; i < dim; i++) {
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
 	fea_items x;
	int y;
 	utils::feature_parse(line.c_str(), x, y);
        train_single_instance(x, y);
    }
}

float FtrlModel::predict_single_instance(fea_items &x) {
    float val = 0.0;
    for(fea_items::iterator pos = x.begin(); pos != x.end(); pos++) {
	val += w[*pos];
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
        fea_items x;
        int y;
        utils::feature_parse(line.c_str(), x, y);
        Y->push_back(y);
        	
	predict->push_back(predict_single_instance(x));
    }
}

