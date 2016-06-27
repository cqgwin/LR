#include <cstdlib>
#include<vector>
#include "ftrl.h"
#include "utils.h"
#include<fstream>
#include<iostream>
#include<thread>
using namespace std;


int main(int argc, char** argv) {
    ifstream ifile;
    ifile.open("/home/eleven/Datasets/ftrl.csv");
    FtrlModel ftrl(6);
    
    int thread_num = thread::hardware_concurrency() - 1;
    printf("thread num = %d\n",thread_num);

    vector<thread> thread_list;

    vector<ftrl_data> data_list(thread_num);
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
	ftrl_data t_data = data_list[i];
    	thread_list.push_back(thread(&FtrlModel::multithread_train, &ftrl, t_data, i));//, i));
    }
    for(int i = 0; i < thread_num; i++) {
	thread_list[i].join();
    }
    string outpath = "/home/eleven/Datasets/out.csv";
    ftrl.dumpw(outpath);
    return 0;
}

