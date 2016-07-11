#include <cstdlib>
#include<vector>
#include<fstream>
#include<iostream>
#include<thread>
#include"utils.h"
#include"fast_ftrl.h"

using namespace std;


int main(int argc, char** argv) {
    FtrlModel ftrl(5374046);
    
    int thread_num = 10;//thread::hardware_concurrency() - 1;
    printf("thread num = %d\n",thread_num);

    vector<thread> thread_list;

    string path = "/home/qspace/user/qingguochen/dataset/rg/cross/merge/train/train.";
    
    cout<<"begin training..."<<endl;
    for(int i = 0; i < thread_num; i++) {
    	thread_list.push_back(thread(&FtrlModel::multithread_train, &ftrl, path, i));
    }
    for(int i = 0; i < thread_num; i++) {
	thread_list[i].join();
    }
    
    //ftrl.clean_w();
    string outpath = "/home/qspace/data/user/qingguochen/dataset/ftrl/w.txt";
    ftrl.dumpw(outpath);
    
    vector<float> predict;
    vector<int> Y;
    
    thread_list.clear();
    string test_path = "/home/qspace/user/qingguochen/dataset/rg/cross/merge/test/test.";
    vector<vector<float> > predict_list(thread_num);
    vector<vector<int> > Y_list(thread_num);
    
    cout<<"begin predict..."<<endl;
    for(int i = 0; i < thread_num; i++) {
        thread_list.push_back(thread(&FtrlModel::multithread_predict, &ftrl, test_path, i, &predict_list[i], &Y_list[i]));
    }
    for(int i = 0; i < thread_num; i++) {
	thread_list[i].join();
    }
    
    for(int i = 0; i < thread_num; i++) {
	predict.insert(predict.end(), predict_list[i].begin(), predict_list[i].end());
	Y.insert(Y.end(), Y_list[i].begin(), Y_list[i].end());
    }
    
    fstream ofile("/home/qspace/data/user/qingguochen/dataset/ftrl/predict.txt", std::fstream::out);
    for(unsigned int i = 0; i < Y.size(); i++) {
	ofile<<predict[i]<<"\t"<<Y[i]<<endl;
    }
    return 0;
}
