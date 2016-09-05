// =====================================================================================
// 
//       Filename:  parser.cc
// 
//    Description:  parser implementation
// 
//        Version:  1.0
//        Created:  09/01/2016 10:19:58 PM
//       Revision:  none
//       Compiler:  g++
// 
// 
// =====================================================================================

#include "parser.h"


index_type atoul(char *s) {
    char* p = s;
    unsigned long long t = 0;
    while (p != NULL && *p != '\n' && *p != '\0') {
        t *= 10;
        t += *p - '0';
        ++p;
    }
    return t;
}

char* SplitL(char c, char* p, index_type& num) {
    char num_p[65];
    int idx = 0;
    while (p != NULL && *p != c && *p != '\0' && *p != '\n') {
        num_p[idx] = *p;
        ++idx;
        ++p;
    }
    num_p[idx] = '\0';
    if (*p == '\0')
        return NULL;
    ++p;
    num = atoul(num_p);
    return p;
}


char* Split(char c, char* p, int& num) {
    char num_p[65];
    int idx = 0;
    while (p != NULL && *p != c && *p != '\0' && *p != '\n') {
        num_p[idx] = *p;
        ++idx;
        ++p;
    }
    num_p[idx] = '\0';
    if (*p == '\0')
        return NULL;
    ++p;
    num = atoi(num_p);
    return p;
}

void Parser::TabParser(char* line, fea_items& X, int& Y) {
    char* begin = line;
    begin = Split(' ', begin, Y);
    while (begin != NULL && *begin != '\0' && *begin != '\n') {
        index_type t;
        begin = SplitL(' ', begin, t);
        X.push_back(t);
    }
}
