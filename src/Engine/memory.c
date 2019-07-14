#include "memory.h"
#include <stdlib.h>

void init_engine_memory(){
    engine_memory = malloc(INIT_MEMORY);
    actual_free_memory = INIT_MEMORY;
    memory_marker = 0;
}

void* allocate_memory(int size){
    if(actual_free_memory > size){
        memory_used += size;
        actual_free_memory -= size;
        void* allocated_memory = &engine_memory[memory_marker];
        memory_marker += size;
        return allocated_memory;
    }   
    
    return NULL;
}

void clear_engine_memory(){
    free(engine_memory);
}

void free_to_marker(int marker){
    memory_used += marker;
    actual_free_memory += marker;
    memory_marker -= marker;
}   