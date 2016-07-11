

#ifndef UTILS_H
#define UTILS_H

#include<string>
#include<stdio.h>
#include<vector>
#include<map>


typedef std::map<int, int> feature_items;
typedef std::vector<int> fea_items;

class utils{
public:
    static std::vector<std::string> split(const std::string &s, const std::string & seperator);
    static void libsvm_format_parse(const char* line, feature_items & x, int &y);
    static float calculate_recall(std::vector<int> &predict_y, std::vector<int> &y);
    static float calculate_precision(fea_items &predict_y, std::vector<int> &y);
    static void feature_parse(const char* line, std::vector<int>& features, int& y);
};

#endif 

