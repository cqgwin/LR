#include <cstdlib>
#include "ftrl.h"
#include "utils.h"
#include<fstream>
#include<iostream>
#include<thread>
using namespace std;

typedef struct ftrl_data_struct{
    vector<feature_items> x_data;
    vector<int> y_data;
} ftrl_data;

int main(int argc, char** argv) {
    ifstream ifile;
    ifile.open("/home/eleven/Datasets/ftrl.csv");
    FtrlModel ftrl(6);
    
    int thread_num = thread::hardware_concurrency() - 1;
    printf("thread num = %d\n",thread_num);

    thread t[thread_num];

    vector<ftrl_data> data_list[thread_num];
    long inc = 0;
    while(ifile.good()) {
        char line[4096];
        ifile.getline(line, 4096);
        feature_items x;
        int y;
        utils::libsvmFomatParse(line, x, y);
	int idx = inc % thread_num;
	data_list[idx].x_data.push_back(x);
	data_list[idx].y_data.push_back(y);
        //ftrl.trainSingleInstance(x,y);
    }
    for(int i = 0; i < thread_num; i++) {
    	t[i] = thread(&FtrlModel::multithread_train, &ftrl, data_list[i], i);
    }
    for(int i = 0; i < thread_num; i++) {
	t[i].join();
    }
    string outpath = "/home/eleven/Datasets/out.csv";
    ftrl.dumpw(outpath);
    return 0;
}

