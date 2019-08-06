/*Created by pavon on 2019/07/17 */
#ifndef ENGINE_ANIMATION_H
#define ENGINE_ANIMATION_H

#define PATH_TYPE_NULL 0
#define PATH_TYPE_TRANSLATION 1
#define PATH_TYPE_ROTATION 2

#include "../../skeletal.h"

typedef struct AnimationSampler{
    Array inputs;//int
    Array outputs_vec4;//vec4
}AnimationSampler;

typedef struct AnimationChannel {
    unsigned short int path_type; 
    Node* node;
    AnimationSampler sampler;
}AnimationChannel;

typedef struct Animation{
    float time;
    bool loop;
    char name[20];
    Array channels;//AnimationChannel
    float start;
    float end; 
}Animation;

#endif // !ENGINE_ANIMATION_H