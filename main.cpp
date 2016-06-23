

#include <cstdlib>
#include "ftrl.hpp"
#include "utils.hpp"
#include<fstream>

using namespace ftrl;
using namespace std;

int main(int argc, char** argv) {
    ifstream ifile;
    ifile.open("D:/datasets/test/ftrl.csv");
    FtrlModel ftrl(10);
    while(ifile.good()) {
        char line[4096];
        ifile.getline(line, 4096);
        feature_items x;
        int y;
        utils::libsvmFomatParse(line, x, y);
        ftrl.trainSingleInstance(x,y);
    }
    string outpath = "D:/datasets/test/w.txt";
    ftrl.dumpw(outpath);
    return 0;
}

