/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include"utils.h"
#include<string.h>
#include <stdlib.h>

std::vector<std::string> utils::split(const std::string &s, const std::string & seperator) {
    std::vector<std::string> v;
    std::string::size_type pos1, pos2;
    pos2 = s.find(seperator);
    pos1 = 0;
    while(std::string::npos != pos2)
    {
        v.push_back(s.substr(pos1, pos2-pos1));

        pos1 = pos2 + seperator.size();
        pos2 = s.find(seperator, pos1);
    }
    if(pos1 != s.length())
        v.push_back(s.substr(pos1));
    return v;
}

void utils::libsvmFomatParse(char * line,feature_items & x, int &y) {
    char * label, * idx, * val, * endptr;
    label = strtok(line, " \t\n");
    int idx_int;
    int val_int;
    if(label == NULL)
        return;
    y = int(strtol(label, &endptr,10));
    if(endptr == label || *endptr != '\0')
        return;
    
    while(1) {
        idx = strtok(NULL, ":");
        val = strtok(NULL, " \t");
        if(val == NULL)
            break;
        idx_int = (int) strtol(idx, &endptr, 10);
        val_int = (int) strtol(val, &endptr, 10);
        x[idx_int] = val_int;
    }
}

float utils::calculate_precision(std::vector<int> &predict_y, std::vector<int> &y) {
    if(predict_y.size() != y.size())
	return -1.0;
    int true_pos = 0, false_pos = 0;
    for(unsigned int i = 0; i < predict_y.size(); i++) {
	if(predict_y[i] == y[i] && predict_y[i] == 1) {
	    true_pos++;
	}
	if(predict_y[i] != y[i] && y[i] == 1) {
	    false_pos++;
        }
    }
    return 1.0 * true_pos / (true_pos + false_pos);
}

float utils::calculate_recall(std::vector<int> &predict_y, std::vector<int> &y) {
    int pos_num = 0, rec_num = 0;
    for(unsigned int i = 0; i < predict_y.size(); i++) {
	if(predict_y[i] == 1 && y[i] == 1)
	    pos_num++;
        if(y[i] == 1)
	    rec_num++;
    }
    return 1.0 * pos_num / rec_num;
}
 
