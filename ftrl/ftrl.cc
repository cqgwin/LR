#include "ftrl.h"

FtrlModel::FtrlModel(float _lambda1, float _lambda2, float _alpha, float _beta): lambda1_(_lambda1), lambda2_(_lambda2), alpha_(_alpha), beta_(_beta) {
    total_loss_ = 0.0;
    instance_num_ = 0;
    p_.clear();
}

FtrlModel::~FtrlModel() {
    p_.clear();
}

float FtrlModel::Logistic(const fea_items& x) {
    float sum = 0;
    for (auto pos = x.begin(); pos != x.end(); ++pos) {
        index_type idx = *pos;
        
        auto p = p_.find(idx);
        if (p != p_.end())
            sum += p->second.w;
    }
    return Sigmoid(sum);
}

void Train(const string& train_file) {
    FILE* fp = fopen(train_file.c_str(), "r");
    fea_items idxs;

    if (fp == NULL)
        printf("file open error!");
    while (ReadLine(fp) != NULL) {
        int label;
        char* begin = line;
        begin = Split(' ', begin, label);
        
        while (begin != NULL && *begin != '\0' && *begin!= '\n') {
            index_type t;
            begin = SplitL(' ', begin, t);
            idxs.push_back(t);
        }
        ++idx;
        Y.push_back(label);
        predict.push_back(ftrl.PredictSingleInstance(idxs));
        if (idx %100000 == 0) {
            printf("%s %dth instance is predicting\t", Utils::GetTime().c_str(), idx);
        }
        idxs.clear();      
    }
}

void FtrlModel::TrainSingleInstance(const fea_items& x, int y) {
    //printf("in training\n");
    float p = Logistic(x);
    for (auto pos = x.begin(); pos != x.end(); ++pos) {
        index_type i = *pos;
        //printf("%llu\n", i);
        auto pp = p_.find(i);
        if (pp == p_.end()) {
            pnode t = {0.0, 0.0, 0.0};
            p_.insert(std::pair<index_type, pnode>(i, t));
            pp = p_.find(i);
        }
        float g = p - y;
        float sigma = (sqrt(pp->second.n + g * g) - sqrt(pp->second.n)) / alpha_;
        pp->second.z += g - sigma * pp->second.w; 
        pp->second.n += g * g;
        if (abs(pp->second.z) < lambda1_)
            pp->second.w = 0;
        else
            pp->second.w = -(pp->second.z - Sgn(pp->second.z) * lambda1_) / ((beta_ + sqrt(pp->second.n)) / alpha_ + lambda2_);
    }
    current_loss_ = LogLoss(p, 1.0 * y);
    //printf("log los is: %f\n", current_loss_);
    //if(!(current_loss_ <= 1 && current_loss_ >= 0))
    //   printf("log los is: %f\n", current_loss_); 
    total_loss_ += current_loss_;
    ++instance_num_;
}



void FtrlModel::CleanW() {
    for (auto itr = p_.begin(); itr != p_.end(); ++itr) {
        if (!(itr->second.w > 10e-10 || itr->second.w < -10e-10))
            itr->second.w = 0;
    }
}

bool FtrlModel::DumpW(const string& filename) {
    std::ofstream ofile;
    ofile.open(filename.c_str(), std::fstream::out);
    for (auto itr = p_.begin(); itr != p_.end(); ++itr) {
        if (!(itr->second.w > 10e-10 || itr->second.w < -10e-10))
            ofile<<itr->first<<":"<<itr->second.w<<std::endl;
    }
    ofile.close();
    return true;
}

/*
void FtrlModel::MultithreadTrain(const string& path, int thread_idx) {
    ifstream ifile;
    char t_char[10];
    sprintf(t_char, "%d", thread_idx); 
    string t_str = t_char;
    ifile.open(path + t_str);
     
    if (!ifile.good())
	    return;
    string line;
    while (getline(ifile, line)) {
        if (line == "")
	       break;
 	    fea_items x;
	    int y;
 	    Utils::VWFormatParse(line.c_str(), x, y);
        TrainSingleInstance(x, y);
    }
}*/

float FtrlModel::PredictSingleInstance(const fea_items &x) {
    float val = 0.0;
    for (auto pos = x.begin(); pos != x.end(); ++pos) {
	    index_type idx = *pos;
        auto itr = p_.find(idx);
        if (itr != p_.end()) {
            val += itr->second.w;
        }
    }
    //float result = Sigmoid(val);
    return val;
}

float FtrlModel::ObjectFunctionValue() {
    return current_loss_ + AbsW();
}

float FtrlModel::AvgLoss() {
    //printf("totoal loss: %f\n", total_loss_);
    //printf("instance num: %d\n", instance_num_);
    return total_loss_ / instance_num_;
}

float FtrlModel::AbsW() {
    float sum = 0.0;
    for (auto itr = p_.begin(); itr != p_.end(); ++itr) {
        sum += abs(itr->second.w);
    }
    return sum;
}

/*
void FtrlModel::MultithreadPredict(const string& test_path,int thread_idx, vector<float>* predict, vector<int>* Y){
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
	    if(line == "")
	        break;
        idx++;
        fea_items x;
        int y;
        Utils::VWFormatParse(line.c_str(), x, y);
        Y->push_back(y);
        	
	    predict->push_back(PredictSingleInstance(x));
    }
}*/

