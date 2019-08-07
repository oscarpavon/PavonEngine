#include "../../level_editor/input.h"
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
        LOG("W pressed\n");
    }
}