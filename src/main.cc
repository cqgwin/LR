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
    ifile.open("/home/qspace/user/tools/vw/bigData/06.train");
    FtrlModel ftrl(1800);
    
    int thread_num = 2;//thread::hardware_concurrency() - 1;
    printf("thread num = %d\n",thread_num);

    vector<thread> thread_list;

    //vector<ftrl_data> data_list(thread_num);
    long inc = 0;
    /*while(ifile.good()) {
        string line;
        getline(ifile, line);
        feature_items x;
        int y;
        utils::libsvm_format_parse(line.c_str(), x, y);
	int idx = inc % thread_num;
	data_list[idx].x_data.push_back(x);
	data_list[idx].y_data.push_back(y);
	ftrl.train_single_instance(x,y);
    }*/
    string path = "/home/qspace/user/tools/vw/bigData/train_files/train.";
    for(int i = 0; i < thread_num; i++) {
    	thread_list.push_back(thread(&FtrlModel::multithread_train, &ftrl, path, i));
	printf("%d\n", i);
    }
    for(int i = 0; i < thread_num; i++) {
	thread_list[i].join();
    }
    string outpath = "/home/qspace/data/user/qingguochen/dataset/ftrl/w.txt";
    ftrl.dumpw(outpath);
    
    vector<float> predict;
    vector<int> Y;
    ifstream test_file;
    test_file.open("/home/qspace/user/tools/vw/bigData/06.test");
    while(test_file.good()) {
        string line;
        getline(test_file, line);
        feature_items x;
        int y;
	utils::libsvm_format_parse(line.c_str(), x, y);
	predict.push_back(ftrl.predict_single_instance(x));
	Y.push_back(y);
    }
    
    fstream ofile("/home/qspace/data/user/qingguochen/dataset/ftrl/predict.txt", std::fstream::out);
    for(unsigned int i = 0; i < Y.size(); i++) {
	ofile<<predict[i]<<"\t"<<Y[i]<<endl;
    }
    return 0;
}
