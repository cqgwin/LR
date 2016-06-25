/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ftrl.hpp
 * Author: qingguochen
 *
 * Created on 2016年6月15日, 下午7:26
 */

#ifndef FTRL_HPP
#define FTRL_HPP

#include<cmath>
#include<map>
#include<fstream>

using namespace std;

namespace ftrl {
    typedef std::map<int, int> feature_items;
    
    inline int sgn(float x) {
        if(x > 0)
            return 1;
        if(x < 0)
            return -1;
    }

    float sigmod(float x) {
        if(x < -35)
            x = -35;
        if(x > 35)
            x = 35;
        return 1/(1+exp(-1.0*x));
    } 
    
    class FtrlModel {
    private:
        float* z;
        float* n;
        int dim;
        float* w;
        float p;
        float lambda1;
        float lambda2;
        float alpha;
        float beta;
    public:
        FtrlModel(int _dim, float _lambda1=1, float _lambda2=1, float _alpha=0.1,float _beta=1): dim(_dim),lambda1(_lambda1),lambda2(_lambda2), alpha(_alpha), beta(_beta) {
            w = new float[_dim]();
            z = new float[_dim]();
            n = new float[_dim]();
        }
        
        ~FtrlModel() {
            delete[] w;
            delete[] z;
            delete[] n;
        }
        
        float logistic(feature_items& x){
            float sum = 0;
            for(feature_items::iterator pos = x.begin(); pos != x.end(); pos++) {
                int idx = pos->first;
                int val = pos->second;
                if(idx > dim){
                    return 0;
                }
                sum += val * w[idx];
            }
            return sigmod(sum);
        }
        
        bool trainSingleInstance(feature_items& x, int y) {
            int sum = 0.0;
            for(feature_items::iterator pos = x.begin(); pos != x.end(); pos++) {
                int i = pos->first;
                int val = pos->second;
                if(abs(z[i])< lambda1)
                    w[i] = 0;
                else
                    w[i] = -(z[i]-sgn(z[i])*lambda1)/((beta+sqrt(n[i])) / alpha+lambda2);
                sum += val*w[i];
            }
            float p = sigmod(sum);
            float g = 0.0;
            float sigma = 0.0;
            
            for(feature_items::iterator pos = x.begin(); pos != x.end(); pos++) {
                int i = pos->first;
                int val = pos->second;
                g = (p-y)*val;
                sigma = (sqrt(n[i]+g*g)-sqrt(n[i]))/alpha;
                z[i] += g-sigma*w[i];
                n[i] += g*g;
            }
            return true;
        }
        
        bool dumpw(string& filename) {
            ofstream ofile;
            ofile.open(filename.c_str());
            for(int i = 0; i < dim; i++) {
                if(w[i]>10e-10||w[i]<-10e-10)
                    ofile<<i<<":"<<w[i]<<endl;
            }
            ofile.close();
            return true;
        }
    };
}


#endif /* FTRL_HPP */

