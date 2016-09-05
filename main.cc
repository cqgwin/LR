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


void TrainPredict(FtrlModel& ftrl, const string& train_dir, const string& test_path, const string& w_path, const string& predict_path) {
    printf("%s begin training...\n", Utils::GetTime().c_str());
    LocalFileSystem lfs; 
    vector<string> file_list = lfs.GetDirFiles(train_dir);
    
    for (auto pos = file_list.begin(); pos != file_list.end(); ++pos) {
        string train_path = *pos;
        ftrl.Train(train_path);
    }
    printf("%s train over!\n", Utils::GetTime().c_str());
    
    ftrl.DumpW(w_path);
    printf("%s w is saved!\n", Utils::GetTime().c_str());
    vector<float> predict;
    vector<int> Y;
    
    printf("%s begin predicting...\n", Utils::GetTime().c_str());
    
    file_list.clear();
    file_list = lfs.GetDirFiles(train_dir);
    
    for (auto pos = file_list.begin(); pos != file_list.end(); ++pos) {
        string test_path = *pos;
        ftrl.Test(test_path, Y, predict);
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
