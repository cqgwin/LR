/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   utils.hpp
 * Author: qingguochen
 *
 * Created on 2016年6月15日, 下午9:49
 */

#ifndef UTILS_H
#define UTILS_H

#include<string>
#include<stdio.h>
#include<vector>
#include<map>


typedef std::map<int, int> feature_items;

class utils{
public:
    static std::vector<std::string> split(const std::string &s, const std::string & seperator);
    static void libsvmFomatParse(char * line, feature_items & x, int &y);
};

#endif 
