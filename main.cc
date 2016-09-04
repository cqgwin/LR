#include <thread>

#include "common/utils.h"
#include "ftrl/ftrl.h"

/*
void MultithredTrainPredict(FtrlModel& ftrl, int thread_num, string train_path, string test_path, string w_path, string predict_path) {
    vector<thread> thread_list;
    printf("%s begin training...\n", Utils::GetTime().c_str());
    for(int i = 0; i < thread_num; i++) {
        thread_list.push_back(thread(&FtrlModel::MultithreadTrain, &ftrl, train_path, i));
    }
    for(int i = 0; i < thread_num; i++) {
        thread_list[i].join();
    }
    printf("%s train over!\n", Utils::GetTime().c_str());
    ftrl.DumpW(w_path);
    printf("%s w is saved!\n", Utils::GetTime().c_str());
    vector<float> predict;
    vector<int> Y;
    
    thread_list.clear();
    vector<vector<float> > predict_list(thread_num);
    vector<vector<int> > Y_list(thread_num);
     
    printf("%s begin predicting...\n", Utils::GetTime().c_str());
    for(int i = 0; i < thread_num; i++) {
        thread_list.push_back(thread(&FtrlModel::MultithreadPredict, &ftrl, test_path, i, &predict_list[i], &Y_list[i]));
    }
    for(int i = 0; i < thread_num; i++) {
        thread_list[i].join();
    }
    printf("%s predict over!\n", Utils::GetTime().c_str());
    for(int i = 0; i < thread_num; i++) {
        predict.insert(predict.end(), predict_list[i].begin(), predict_list[i].end());
        Y.insert(Y.end(), Y_list[i].begin(), Y_list[i].end());
    }

    float auc = Utils::GetAUC(predict, Y);
    printf("auc is: %f\n", auc);

    fstream ofile(predict_path, std::fstream::out);
    for(unsigned int i = 0; i < Y.size(); i++) {
        ofile<<predict[i]<<" "<<Y[i]<<endl;
    }
}
*/

index_type atoul(char *s) {
    char* p = s;
    unsigned long long t = 0;
    while(p != NULL && *p != '\n' && *p != '\0') {
        t *= 10;
        t += *p - '0';
        ++p;
    }
    return t;
}

static int max_line_len;
static char* line = NULL;

char* ReadLine(FILE* input) {
    int len;
    line = (char*) malloc(max_line_len * sizeof(char));

    if(fgets(line, max_line_len, input) == NULL)
        return NULL;
    while(strrchr(line, '\n') == NULL) {
        max_line_len *= 2;
        line = (char*) realloc(line, max_line_len);
        len = (int) strlen(line);
        if(fgets(line+len, max_line_len-len, input) == NULL)
            break;
    }
    return line;
}

char* SplitL(char c, char* p, index_type& num) {
    char num_p[65];
    int idx = 0;
    while(p != NULL && *p != c && *p != '\0' && *p != '\n') {
        num_p[idx] = *p;
        ++idx;
        ++p;
    }
    num_p[idx] = '\0';
    if(*p == '\0')
        return NULL;
    ++p;
    num = atoul(num_p);
    return p;
}


char* Split(char c, char* p, int& num) {
    char num_p[65];
    int idx = 0;
    while(p != NULL && *p != c && *p != '\0' && *p != '\n') {
        num_p[idx] = *p;
        ++idx;
        ++p;
    }
    num_p[idx] = '\0';
    if(*p == '\0')
        return NULL;
    ++p;
    num = atoi(num_p);
    return p;
}

void TrainPredict(FtrlModel& ftrl, const string& train_path, const string& test_path, const string& w_path, const string& predict_path) {
    printf("%s begin training...\n", Utils::GetTime().c_str());
    FILE* fp = fopen(train_path.c_str(), "r");
    
    int idx = 0;
    max_line_len = 2000;
    fea_items idxs;
    if (fp == NULL)
        printf("file open error!");
    while(ReadLine(fp) != NULL) {
        int label;
        char* begin = line;
        //printf("%s\n", line);
        begin = Split(' ', begin, label);
        //printf("label splited!\n");
        while(begin != NULL && *begin != '\0' && *begin!= '\n') {
            index_type t;
            begin = SplitL(' ', begin, t);
            idxs.push_back(t);
            //printf("%lld ",t);
        }
        ++idx;
        //printf("begin training!\n");
        if(label == -1)
            label = 0;
        ftrl.TrainSingleInstance(idxs, label);
        //printf("end training!\n");
        if(idx %100000 == 0) {
            printf("%s %dth instance is training\t", Utils::GetTime().c_str(), idx);
            printf("average loss is: %f\n", ftrl.AvgLoss());
        }
        idxs.clear();
    }
    printf("%s train over!\n", Utils::GetTime().c_str());
    
    ftrl.DumpW(w_path);
    printf("%s w is saved!\n", Utils::GetTime().c_str());
    vector<float> predict;
    vector<int> Y;
    
    printf("%s begin predicting...\n", Utils::GetTime().c_str());
    FILE* fd = fopen(test_path.c_str(), "r"); 
    idx = 0;
    while(ReadLine(fd) != NULL) {
        int label;
        char* begin = line;
        begin = Split(' ', begin, label);
        while(begin != NULL && *begin != '\0' && *begin!= '\n') {
            index_type t;
            begin = SplitL(' ', begin, t);
            idxs.push_back(t);
        }
        ++idx;
        Y.push_back(label);
        predict.push_back(ftrl.PredictSingleInstance(idxs));
        if(idx %100000 == 0) {
            printf("%s %dth instance is predicting\t", Utils::GetTime().c_str(), idx);
        }
        idxs.clear();
    }
    printf("%s predict over!\n", Utils::GetTime().c_str());
    float auc = Utils::GetAUC(predict, Y);
    printf("auc is: %f\n", auc);
    std::fstream ofile(predict_path, std::fstream::out);
    for(unsigned int i = 0; i < Y.size(); i++) {
        ofile<<predict[i]<<" "<<Y[i]<<std::endl;
    }    
}


int main(int argc, char** argv) {
    //int feature_length = atoi(argv[1]);
    string train_path = argv[1];
    string w_path = argv[2];
    string test_path = argv[3];
    string predict_path = argv[4];
    

    int thread_num = 1;
    FtrlModel ftrl;
    
    if(thread_num == 1)
        TrainPredict(ftrl,  train_path, test_path, w_path, predict_path);
    //else
    //    MultithredTrainPredict(ftrl, thread_num, train_path, test_path, w_path, predict_path);
    
    return 0;
}
