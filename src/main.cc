#include <cstdlib>
#include<vector>
#include<fstream>
#include<iostream>
#include<thread>
#include"utils.h"
#include"ftrl.h"

using namespace std;


int main(int argc, char** argv) {
    FtrlModel ftrl(1800);
    
    int thread_num = 10;//thread::hardware_concurrency() - 1;
    printf("thread num = %d\n",thread_num);

    vector<thread> thread_list;

    string path = "/home/qspace/user/tools/vw/bigData/train_files/train.";
    for(int i = 0; i < thread_num; i++) {
    	thread_list.push_back(thread(&FtrlModel::multithread_train, &ftrl, path, i));
    }
    for(int i = 0; i < thread_num; i++) {
	thread_list[i].join();
    }
    string outpath = "/home/qspace/data/user/qingguochen/dataset/ftrl/w.txt";
    ftrl.dumpw(outpath);
    
    vector<float> predict;
    vector<int> Y;
    
    thread_list.clear();
    string test_path = "/home/qspace/user/tools/vw/bigData/test_files/test.";
    vector<vector<float> > predict_list(thread_num);
    vector<vector<int> > Y_list(thread_num);
    for(int i = 0; i < thread_num; i++) {
        thread_list.push_back(thread(&FtrlModel::multithread_predict, &ftrl, test_path, i, predict_list[i], Y_list[i]));
    }
    for(int i = 0; i < thread_num; i++) {
	thread_list[i].join();
    }
    
    for(int i = 0; i < thread_num; i++) {
	predict.insert(predict.end(), predict_list[i].begin(), predict_list[i].end());
	Y.insert(Y.end(), Y_list[i].begin(), Y_list[i].end());
    }
    cout<<Y.size()<<endl;
    fstream ofile("/home/qspace/data/user/qingguochen/dataset/ftrl/predict.txt", std::fstream::out);
    for(unsigned int i = 0; i < Y.size(); i++) {
	ofile<<predict[i]<<"\t"<<Y[i]<<endl;
    }
    return 0;
}
