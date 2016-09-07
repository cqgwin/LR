
#include"utils.h"

bool Cmp(std::pair<float, int> a, std::pair<float, int> b) {
    return a.first > b.first;
}

float Utils::GetAUC(const std::vector<float>& predict, const std::vector<int>& Y) {
    int M = 0, N = 0;
    std::vector< std::pair<float, int> > predict_list;
    for (unsigned int i = 0; i < Y.size(); i++) {
        if(Y[i] == 1)
            M++;
        else
            N++;
        predict_list.push_back(std::make_pair(predict[i], Y[i]));
    }
    std::sort(predict_list.begin(), predict_list.end(), Cmp);
    
    std::vector<std::pair<bool, int> > rank_list;
    if (predict_list[0].first == predict_list[1].first) {
        rank_list.push_back(std::make_pair(1, M+N));
    }
    else {
        rank_list.push_back(std::make_pair(0, M+N));
    }
    for (unsigned int i = 1; i < predict_list.size()-1; i++) {
        if (predict_list[i].first == predict_list[i-1].first) 
            rank_list.push_back(std::make_pair(1,M+N-i));
        else if (predict_list[i].first == predict_list[i+1].first)
            rank_list.push_back(std::make_pair(1,M+N-i));
        else
            rank_list.push_back(std::make_pair(0,M+N-i));
    }
    if (predict_list[Y.size()-1].first == predict_list[Y.size()-2].first) 
        rank_list.push_back(std::make_pair(1,1));
    else
        rank_list.push_back(std::make_pair(0,1));
    
    int begin = -1, end = -1;
    if (rank_list[0].first == 1) {
        begin = 0;
    }
    
    std::vector<double> score_list(rank_list.size());
    for (unsigned int i = 1; i < rank_list.size()-1 ; i++) {
        score_list[i] = rank_list[i].second;
        if (rank_list[i].first==1&&rank_list[i+1].first==0){
            end = i;
            double sum = 0;
            for (int j = begin; j <= end; j++) {
                sum += rank_list[j].second;
            }
            sum /= (end-begin+1);
            for (int j = begin; j <= end; j++) {
                score_list[j] = sum;
            }
            begin = -1; end = -1;
        }
        if (rank_list[i].first==1&&rank_list[i-1].first==0)
            begin = i;
    }
    score_list[rank_list.size()-1] = rank_list[rank_list.size()-1].second;
    if (rank_list[rank_list.size()-1].first==1&&rank_list[rank_list.size()-2].first==1){
        end = rank_list.size()-1;
        double sum = 0.0;
        for (int j = begin; j <= end; j++) {
            sum += rank_list[j].second;
        }
        sum = 1.0 * sum / (end-begin+1);
        for (int j = begin; j <= end; j++) {
            score_list[j] = sum;
        }
    }
    double sum = 0;
    for (unsigned int i = 0; i < rank_list.size(); i++) {
        if (predict_list[i].second == 1)
            sum += (score_list[i] - (M+1)/2) / M;
    }
    return sum / N;
}

string Utils::GetTime() {
    time_t t = time(NULL);
    struct tm* current_time = localtime(&t);
    char buff[15];
    sprintf(buff, "%4d-%d-%d %2d:%2d:%2d", current_time->tm_year + 1900, current_time->tm_mon + 1, current_time->tm_mday, current_time->tm_hour,current_time->tm_min, current_time->tm_sec);
    string result(buff);
    return result;
}
