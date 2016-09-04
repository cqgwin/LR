// =====================================================================================
// 
//       Filename:  parser.h
// 
//    Description:  parser head file
// 
//        Version:  1.0
//        Created:  09/01/2016 10:18:21 PM
//       Revision:  none
//       Compiler:  g++
// 
// 
// =====================================================================================
#ifndef PARSER_H_
#define PARSER_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <vector>

#include "common/utils.h"

using std::string;
using std::vector;

class Parser {
 public:
    void TabParser(char* line, fea_items& X, int& Y);
};

#endif
