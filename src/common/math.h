// =====================================================================================
// 
//       Filename:  math.h
// 
//    Description:  common math function
// 
//        Version:  1.0
//        Created:  09/18/2016 07:37:53 PM
//       Revision:  none
//       Compiler:  g++
// =====================================================================================

#ifndef COMMON_MATH_H_
#define COMMON_MATH_H_

#include <cmath>
#include <vector>

using std::vector;


inline int Sgn(float x) {
    if (x < 0.)
        return -1.;
    else
        return 1.;
}

inline float Sigmoid(float x) {
    if (x < -35)
        x = -35;
    if (x > 35)
        x = 35;
    return 1 / (1 + exp(-1.0 * x));
}

inline float LogLoss(float p, float y) {
    if(p < 10e-15)
        p = 10e-15;
    if(p > 1 - 10e-15)
        p = 1 - 10e-15;
    return -y * log(p) - (1-y) * log(1-p);
}

#endif
