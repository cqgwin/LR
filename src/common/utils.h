

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

inline std::vector<std::string> Split(const std::string& s, char delim) {
    std::string item;
    std::istringstream is(s);
    std::vector<std::string> ret;
    while (std::getline(is, item, delim)) {
        ret.push_back(item);
    }
    return ret;
}

float GetAUC(const vector<float>& predict, const vector<int>& Y);

inline string GetTime() {
    time_t t = time(NULL);
    struct tm* current_time = localtime(&t);
    char buff[15];
    sprintf(buff, "%4d-%d-%d %2d:%2d:%2d", current_time->tm_year + 1900, current_time->tm_mon + 1, current_time->tm_mday, current_time->tm_hour,current_time->tm_min, current_time->tm_sec);
    string result(buff);
    return result;
}

#endif 

