#include "../../Editor/input.h"
#include "../../engine.h"

struct champion{
    int champion01;
    int champion02;
};

void test02(){
    LOG("work\n");
}
void test(void* in){
    Input* input = in;
    if(input->D.pressed){
        LOG("input work\n");
    }
    if(input->W.pressed){
        //update_translation(VEC3(0,0.2,0));        
    }
    if(key_released(&input->Q)){
        LOG("q pressed\n");
    }
}