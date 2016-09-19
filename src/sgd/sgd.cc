#include "sgd.h"

SGDModel::SGDModel(float _learning_rate): learning_rate_(_learning_rate) {
    total_loss_ = 0.0;
    instance_num_ = 0;
    w_.clear();
}

void SGDModel::SetParameter(float _learning_rate) {
    learning_rate_ = _learning_rate;
}

SGDModel::~SGDModel() {
    w_.clear();
}

float SGDModel::Logistic(const fea_items& x) {
    float sum = 0;
    for (auto pos = x.begin(); pos != x.end(); ++pos) {
        index_type idx = *pos;
        
        auto p = w_.find(idx);
        if (p != w_.end())
            sum += p->second;
    }
    return Sigmoid(sum);
}

void SGDModel::Train(const string& train_file) {
    FILE* fp = fopen(train_file.c_str(), "r");
    fea_items idxs;

    LocalFileSystem lfs;
    Parser parser;
    int print_idx = 0;
    int y;
    if (fp == NULL)
        printf("file open error!");
    char* line;
    while ((line = lfs.ReadLine(fp)) != NULL) {
        idxs.clear();
        parser.TabParser(line, idxs, y);
        TrainSingleInstance(idxs, y);
        if (print_idx %100000 == 0) {
            printf("%s FILE:%s's %dth instance is predicting\n", GetTime().c_str(), train_file.c_str(), print_idx);
        }
        ++print_idx;
    }
}

void SGDModel::Predict(const string& test_file, const string& predict_file) {
    FILE* fp = fopen(test_file.c_str(), "r");
    fea_items idxs;

    LocalFileSystem lfs;
    Parser parser;
    int y;
    vector<int> Y;
    vector<float> predict;
    if (fp == NULL)
        printf("file open error!");
    char* line;
    while ((line = lfs.ReadLine(fp)) != NULL) {
        idxs.clear();
        parser.TabParser(line, idxs, y);
        predict.push_back(PredictSingleInstance(idxs));
        Y.push_back(y);
    }
    printf("%s FILE:%s is predicted over!\n", GetTime().c_str(), test_file.c_str());
    float auc = GetAUC(predict, Y);
    printf("FILE:%s AUC is %f\n", test_file.c_str(), auc);
    
    std::ofstream ofile;
    ofile.open(predict_file);
    for(unsigned int i = 0; i < Y.size(); ++i) {
        ofile<<Y[i]<<" "<<predict[i]<<std::endl;
        ofile.flush();
    }
    ofile.close();
}

void SGDModel::TrainSingleInstance(const fea_items& x, int y) {
    float p = Logistic(x);
    float g = p - y;
    for (auto pos = x.begin(); pos != x.end(); ++pos) {
        index_type i = *pos;
        auto pp = w_.find(i);
        if (pp == w_.end()) {
            w_.insert(std::pair<index_type, float>(i, 0.0));
            pp = w_.find(i);
        }
        pp->second -= learning_rate_ * g;
    }
    current_loss_ = LogLoss(p, 1.0 * y);
    total_loss_ += current_loss_;
    ++instance_num_;
}

void SGDModel::CleanW() {
    for (auto itr = w_.begin(); itr != w_.end(); ++itr) {
        if (!(itr->second > 10e-10 || itr->second < -10e-10))
            itr->second = 0;
    }
}

void SGDModel::DumpW(const string& filename) {
    std::ofstream ofile;
    ofile.open(filename.c_str(), std::fstream::out);
    for (auto itr = w_.begin(); itr != w_.end(); ++itr) {
        if (!(itr->first > 10e-10 || itr->second < -10e-10))
            ofile<<itr->first<<":"<<itr->second<<std::endl;
    }
    ofile.close();
}

void SGDModel::MultithreadTrain(const string& train_dir, const vector<string>& train_list, int thread_idx, int thread_num) {
    for(unsigned int i = 0; i <  train_list.size(); ++i) {
        if((int)(i % thread_num) == thread_idx)
            Train(train_dir + train_list[i]);
    }
}

float SGDModel::PredictSingleInstance(const fea_items &x) {
    float val = 0.0;
    for (auto pos = x.begin(); pos != x.end(); ++pos) {
	    index_type idx = *pos;
        auto itr = w_.find(idx);
        if (itr != w_.end()) {
            val += itr->second;
        }
    }
    return val;
}

float SGDModel::ObjectFunctionValue() {
    return current_loss_ + AbsW();
}

float SGDModel::AvgLoss() {
    return total_loss_ / instance_num_;
}

float SGDModel::AbsW() {
    float sum = 0.0;
    for (auto itr = w_.begin(); itr != w_.end(); ++itr) {
        sum += abs(itr->second);
    }
    return sum;
}

void SGDModel::MultithreadPredict(const string& test_dir, const vector<string>& test_list, const string& predict_dir, int thread_idx, int thread_num){    
    for(unsigned int i = 0; i < test_list.size(); ++i) {
        if((int)(i % thread_num) == thread_idx)
            Predict(test_dir + test_list[i], predict_dir + test_list[i]);
    }
}
