

#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <algorithm>

typedef unsigned long long index_type; 
typedef std::vector<index_type> fea_items;

using std::string;
using std::vector;
using std::map;

class Utils{
public:
    static float GetAUC(const vector<float>& predict, const vector<int>& Y);
    static string GetTime();
};

#endif 

