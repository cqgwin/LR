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
    ifile.open("/home/qspace/user/tools/vw/data/06-train");
    FtrlModel ftrl(1730);
    
    int thread_num = 1;//thread::hardware_concurrency() - 1;
    printf("thread num = %d\n",thread_num);

    //vector<thread> thread_list;

    //vector<ftrl_data> data_list(thread_num);
    long inc = 0;
    while(ifile.good()) {
        string line;
        getline(ifile, line);
        feature_items x;
        int y;
        utils::libsvm_format_parse(line.c_str(), x, y);
	/*int idx = inc % thread_num;
	data_list[idx].x_data.push_back(x);
	data_list[idx].y_data.push_back(y);*/
	ftrl.train_single_instance(x,y);
    }
    /*for(int i = 0; i < thread_num; i++) {
	ftrl_data t_data = data_list[i];
    	thread_list.push_back(thread(&FtrlModel::multithread_train, &ftrl, t_data, i));
    }
    for(int i = 0; i < thread_num; i++) {
		thread_list[i].join();
    }*/
    string outpath = "/home/qspace/data/user/qingguochen/dataset/ftrl/w.txt";
    ftrl.dumpw(outpath);
    
    vector<float> predict;
    vector<int> Y;
    ifstream test_file;
    test_file.open("/home/qspace/user/tools/vw/data/06-test");
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
	ofile<<predict[i]<<" "<<Y[i]<<endl;
    }
    return 0;
}
