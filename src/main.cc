#include <thread>

#include "common/utils.h"
#include "ftrl/ftrl.h"
#include "base_learner.h"
#include "common/config.h"

using std::thread;

void Train(BaseModel& learner, const string& train_dir) { 
    printf("%s begin training...\n", GetTime().c_str());
    LocalFileSystem lfs; 
    vector<string> file_list = lfs.GetDirFiles(train_dir);
    for (auto pos = file_list.begin(); pos != file_list.end(); ++pos) {
        string train_path = *pos;
        learner.Train(train_dir + train_path);
    }
    printf("%s train over!\n", GetTime().c_str());
}


void MultithreadTrain(BaseModel& learner, const int thread_num, const string& train_dir) { 
    printf("%s begin training...\n", GetTime().c_str());
    LocalFileSystem lfs; 
    vector<string> file_list = lfs.GetDirFiles(train_dir);
    vector<thread> thread_list;
    printf("there is %d files in the training dir\n", file_list.size());
    for (int i = 0; i < thread_num; i++) {
        thread_list.push_back(thread(&BaseModel::MultithreadTrain, &learner, train_dir, file_list, i, thread_num));
    }
    for (int i = 0; i < thread_num; i++) {
        thread_list[i].join();
    }
    printf("%s train over!\n", GetTime().c_str());
}

void Predict(BaseModel& learner, const string& test_dir, const string& predict_dir) {
    printf("%s begin predicting...\n", GetTime().c_str());
    LocalFileSystem lfs;
    vector<string> file_list = lfs.GetDirFiles(test_dir);
    
    for (auto pos = file_list.begin(); pos != file_list.end(); ++pos) {
        string test_file = *pos;
        learner.Predict(test_dir + test_file, predict_dir + test_file);
    } 
    printf("%s predict over!\n", GetTime().c_str());
}


void MultithreadPredict(BaseModel& learner, const int thread_num, const string& test_dir, const string& predict_dir) { 
    printf("%s begin predicting...\n", GetTime().c_str());
    LocalFileSystem lfs; 
    vector<string> file_list = lfs.GetDirFiles(test_dir);
    vector<thread> thread_list;

    for (int i = 0; i < thread_num; i++) {
        thread_list.push_back(thread(&BaseModel::MultithreadPredict, &learner, test_dir, file_list, predict_dir, i, thread_num));
    }
    for (int i = 0; i < thread_num; i++) {
        thread_list[i].join();
    }
    printf("%s predict over!\n", GetTime().c_str());
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("USAGE: <config>\n");
        return 0;
    }
    Config config;
    BaseModel learner = config.Init(argc, argv);
    if (learner == NULL) {
        return -1;
    }
    if (learner.Task == "train") {
        FILE* fh = fopen(config.ModelPath().c_str(), "r");
        if (fh != NULL) {
            learner.LoadModel(config.ModelPath());
        }

        printf("%d %s %s\n", config.ThreadNum(), config.TrainDir().c_str(), config.ModelPath().c_str()); 
        if (thread_num == 1) {
            Train(learner, config.TrainDir());
        } else {
            MultithreadTrain(learner, config.ThreadNum(), config.TrainDir());
        }
        learner.SaveModel(config.ModelPath());    
    } else { 
        FILE* fh = fopen(config.ModelPath().c_str(), "r");
        if (fh == NULL) {
            printf("model doesn't exist!\n");
            return -1;
        }
        learner.LoadModel(config.ModelPath());
        printf("%d %s %s %s\n", config.ThreadNum(), config.ModelPath().c_str(), config.TestDir().c_str(), config.PredictDir().c_str()); 
        if (thread_num == 1) {
            Predict(learner, config.TestDir(), config.PredictDir());
        } else {
            MultithreadPredict(ftrl, thread_num, test_dir, predict_dir);
        }
    }
}


int main(int argc, char** argv) {
    if (argc < 2) {
        printf("USAGE: <config>\n");
        return 0;
    }
    if (strcmp(argv[1], "train") == 0) {             
if (argc != 7 && argc != 8) {
            printf("USAGE LIKE: ./mmibzlr train ftrl lambda1 lambda2 train_dir/ model_path||./mmibzlr train ftrl thread_num lambda1 lambda2 train_dir/ model_file\n");
            return 0;
        }
        int thread_num = 1;
        string train_dir, model_path;
        float lambda1, lambda2;
        if (argc == 6) {
            lambda1 = atof(argv[2]);
            lambda2 = atof(argv[3]);
            train_dir = DirEndCheck(argv[4]);
            model_path = argv[5];
        } else {
            lambda1 = atof(argv[3]);
            lambda2 = atof(argv[4]);
            thread_num = atoi(argv[2]);
            train_dir = DirEndCheck(argv[5]);
            model_path = argv[6];
        }
        FtrlModel ftrl(lambda1, lambda2);

        FILE* fh = fopen(model_path.c_str(), "r");
        if (fh != NULL) {
            ftrl.LoadModel(model_path);
            printf("%d modle is loading\n", ftrl.GetPSize());
        }

        printf("%d %s %s\n", thread_num, train_dir.c_str(), model_path.c_str()); 
        if (thread_num == 1) {
            Train(ftrl, train_dir);
        } else {
            MultithreadTrain(ftrl, thread_num, train_dir);
        }
        ftrl.SaveModel(model_path);    
        return 0;
    } else if (strcmp(argv[1], "test") == 0) {
        if (argc != 7 && argc != 8) {
            printf("USAGE LIKE: ./mmibzlr test lambda1 lambda2 model_path test_dir/ predict_dir/||mmibzlr test thread_num lambda1 lambda2 model_file test_dir/ predict_dir/\n");
            return -1;
        }
        int thread_num = 1;
        string test_dir, model_path, predict_dir;
        float lambda1, lambda2;
        if (argc == 7) {
            lambda1 = atof(argv[2]);
            lambda2 = atof(argv[3]);
            test_dir = DirEndCheck(argv[5]);
            model_path = argv[4];
            predict_dir = DirEndCheck(argv[6]);
        } else {
            lambda1 = atof(argv[3]);
            lambda2 = atof(argv[4]);
            thread_num = atoi(argv[2]);
            test_dir = DirEndCheck(argv[6]);
            model_path = argv[5];
            predict_dir = DirEndCheck(argv[7]);
        }
        FtrlModel ftrl(lambda1, lambda2);

        FILE* fh = fopen(model_path.c_str(), "r");
        if (fh == NULL) {
            printf("model doesn't exist!\n");
            return -1;
        }
        ftrl.LoadModel(model_path);
        printf("%d %s %s %s\n", thread_num, model_path.c_str(), test_dir.c_str(), predict_dir.c_str()); 
        if (thread_num == 1) {
            Predict(ftrl, test_dir, predict_dir);
        } else {
            MultithreadPredict(ftrl, thread_num, test_dir, predict_dir);
        }
    } else {
        printf("Parameter input ERROR\n");
        return -1;
    }
    return 0;
}

