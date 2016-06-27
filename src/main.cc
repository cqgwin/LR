#include <cstdlib>
#include "ftrl.h"
#include "utils.h"
#include<fstream>
using namespace std;

int main(int argc, char** argv) {
    ifstream ifile;
    ifile.open("/home/eleven/Datasets/ftrl.csv");
    FtrlModel ftrl(10);
    while(ifile.good()) {
        char line[4096];
        ifile.getline(line, 4096);
        feature_items x;
        int y;
        utils::libsvmFomatParse(line, x, y);
        ftrl.trainSingleInstance(x,y);
    }
    string outpath = "/home/eleven/Datasets/out.csv";
    ftrl.dumpw(outpath);
    return 0;
}

