#ifndef UTILS_H
#define UTILS_H

#include <cglm.h>


static inline void init_vec3(float x, float y, float z , vec3 vec){
    vec[0] = x;
    vec[1] = y;
    vec[2] = z;
}

#endif