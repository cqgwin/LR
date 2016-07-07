
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

void utils::libsvm_format_parse(const char * line,feature_items & x, int &y) {
    std::vector<std::string> items1 = split(line, " ");
    if(items1.size()<=1)
	return;
    y = atoi(items1[0].c_str());
    for(unsigned int i = 1; i < items1.size(); i++) {
        std::vector<std::string> items2 = split(items1[i], ":");
        x[atoi(items2[0].c_str())] = atoi(items2[1].c_str());
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
 

