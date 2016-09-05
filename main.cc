#include <thread>

#include "common/utils.h"
#include "ftrl/ftrl.h"

using std::thread;

void Train(FtrlModel& ftrl, const string& train_dir) { 
    printf("%s begin training...\n", Utils::GetTime().c_str());
    LocalFileSystem lfs; 
    vector<string> file_list = lfs.GetDirFiles(train_dir);
    for (auto pos = file_list.begin(); pos != file_list.end(); ++pos) {
        string train_path = *pos;
        ftrl.Train(train_dir + train_path);
    }
    printf("%s train over!\n", Utils::GetTime().c_str());
}


void MultithreadTrain(FtrlModel& ftrl, const int thread_num, const string& train_dir) { 
    printf("%s begin training...\n", Utils::GetTime().c_str());
    LocalFileSystem lfs; 
    vector<string> file_list = lfs.GetDirFiles(train_dir);
    vector<thread> thread_list;

    for(int i = 0; i < thread_num; i++) {
        thread_list.push_back(thread(&FtrlModel::MultithreadTrain, &ftrl, train_dir, file_list, i, thread_num));
    }
    for(int i = 0; i < thread_num; i++) {
        thread_list[i].join();
    }
    printf("%s train over!\n", Utils::GetTime().c_str());
}

void DumpW(FtrlModel& ftrl, const string& w_path) {
    ftrl.DumpW(w_path);
    printf("%s w is saved!\n", Utils::GetTime().c_str());
}

void Predict(FtrlModel& ftrl, const string& test_dir, const string& predict_dir) {
    printf("%s begin predicting...\n", Utils::GetTime().c_str());
    LocalFileSystem lfs;
    vector<string> file_list = lfs.GetDirFiles(test_dir);
    
    for (auto pos = file_list.begin(); pos != file_list.end(); ++pos) {
        string test_file = *pos;
        ftrl.Predict(test_dir + test_file, predict_dir + test_file);
    } 
    printf("%s predict over!\n", Utils::GetTime().c_str());
}


void MultithreadPredict(FtrlModel& ftrl, const int thread_num, const string& test_dir, const string& predict_dir) { 
    printf("%s begin predicting...\n", Utils::GetTime().c_str());
    LocalFileSystem lfs; 
    vector<string> file_list = lfs.GetDirFiles(test_dir);
    vector<thread> thread_list;

    for(int i = 0; i < thread_num; i++) {
        thread_list.push_back(thread(&FtrlModel::MultithreadPredict, &ftrl, test_dir, file_list, predict_dir, i, thread_num));
    }
    for(int i = 0; i < thread_num; i++) {
        thread_list[i].join();
    }
    printf("%s predict over!\n", Utils::GetTime().c_str());
}

int main(int argc, char** argv) {
    if(argc != 4) {
        printf("USAGE LIKE: mmibzlr train_dir w_path test_dir predict_dir\n");
        return -1;
    }
    string train_dir = argv[1];
    string w_path = argv[2];
    string test_dir = argv[3];
    string predict_dir = argv[4];
    

    int thread_num = 1;
    FtrlModel ftrl;
    
    if(thread_num == 1) {
        Train(ftrl, train_dir);
        Predict(ftrl, test_dir, predict_dir);
    } else {
        MultithreadTrain(ftrl, thread_num, train_dir);
        MultithreadPredict(ftrl, thread_num, test_dir, predict_dir);
    }
    DumpW(ftrl, w_path);
    
    return 0;
}
