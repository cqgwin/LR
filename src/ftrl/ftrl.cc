#include "ftrl.h"

FtrlModel::FtrlModel(float _lambda1, float _lambda2, float _alpha, float _beta): lambda1_(_lambda1), lambda2_(_lambda2), alpha_(_alpha), beta_(_beta) {
    total_loss_ = 0.0;
    instance_num_ = 0;
    p_.clear();
}

FtrlModel::~FtrlModel() {
    p_.clear();
}

void SetParameter(float _lambda1, float _lambda2, float _alpha, float _beta) {
    lambda1_ = _lambda1;
    lambda2_ = _lambda2;
    alpha_ = _alpha;
    beta_ = _beta; 
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

void FtrlModel::Train(const string& train_file) {
    FILE* fp = fopen(train_file.c_str(), "r");
    fea_items idxs;

    LocalFileSystem lfs;
    Parser parser;
    int y;
    if (fp == NULL)
        printf("file open error!");
    char* line;
    while ((line = lfs.ReadLine(fp)) != NULL) {
        //printf("%s\n", line);
        idxs.clear();
        parser.TabParser(line, idxs, y);
        TrainSingleInstance(idxs, y);
    }
}

void FtrlModel::Predict(const string& test_file, const string& predict_file) {
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

void FtrlModel::TrainSingleInstance(const fea_items& x, int y) {
    float p = Logistic(x);
    for (auto pos = x.begin(); pos != x.end(); ++pos) {
        index_type i = *pos;
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
    total_loss_ += current_loss_;
    ++instance_num_;
}

void FtrlModel::SaveModel(const string& model_path) { 
    std::ofstream ofile;
    ofile.open(model_path.c_str(), std::fstream::out);
    for (auto itr = p_.begin(); itr != p_.end(); ++itr) {
        ofile<<itr->first<<" "<<itr->second.w<<" "<<itr->second.z<<" "<<itr->second.n<<std::endl;
    }
    ofile.close();
}

void FtrlModel::LoadModel(const string& model_path) {
    FILE* fp = fopen(model_path.c_str(), "r");
    std::ifstream in(model_path);
    index_type idx;
    float w, z, n;
    LocalFileSystem lfs;
    printf("model path is %s\n", model_path.c_str());
    int ii = 1;
    while (!in.eof() ) {
        in>>idx>>w>>z>>n;
        pnode t = {w,z,n};
        p_.insert(std::pair<index_type, pnode>(idx, t));
    }
}

void FtrlModel::MultithreadTrain(const string& train_dir, const vector<string>& train_list, int thread_idx, int thread_num) {
    for(unsigned int i = 0; i <  train_list.size(); ++i) {
        if((int)(i % thread_num) == thread_idx) {
            printf("%s in thread %d, FILE:%s is trainging!\n", GetTime().c_str(), thread_idx, train_list[i].c_str());
            Train(train_dir + train_list[i]);
            printf("%s in thread %d, FILE:%s is trained over!\n", GetTime().c_str(), thread_idx, train_list[i].c_str());
        }
    }
}

float FtrlModel::PredictSingleInstance(const fea_items &x) {
    float val = 0.0;
    for (auto pos = x.begin(); pos != x.end(); ++pos) {
	    index_type idx = *pos;
        auto itr = p_.find(idx);
        if (itr != p_.end()) {
            val += itr->second.w;
        }
    }
    return val;
}

float FtrlModel::ObjectFunctionValue() {
    return current_loss_ + AbsW();
}

float FtrlModel::AvgLoss() {
    return total_loss_ / instance_num_;
}

float FtrlModel::AbsW() {
    float sum = 0.0;
    for (auto itr = p_.begin(); itr != p_.end(); ++itr) {
        sum += abs(itr->second.w);
    }
    return sum;
}

void FtrlModel::MultithreadPredict(const string& test_dir, const vector<string>& test_list, const string& predict_dir, int thread_idx, int thread_num){    
    for(unsigned int i = 0; i < test_list.size(); ++i) {
        if((int)(i % thread_num) == thread_idx)
            Predict(test_dir + test_list[i], predict_dir + test_list[i]);
    }
}
