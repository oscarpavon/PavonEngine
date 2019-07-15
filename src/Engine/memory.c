#include "memory.h"
#include <stdlib.h>


void init_engine_memory(){
    engine_memory = malloc(INIT_MEMORY);
    actual_free_memory = INIT_MEMORY;
    memory_marker = 0;

    indices.memory = allocate_memory(10000);
    indices.available = 10000;
    indices.used = 0;
    indices.marker = 0;

    engine_stack_memory.memory = allocate_memory(60000);
    engine_stack_memory.available = 60000;
    engine_stack_memory.used = 0;
    engine_stack_memory.marker = 0;
}


void* allocate_stack_memory(StackMemory* stack, int bytes_size){
    if(stack->available > bytes_size){
        stack->used += bytes_size;
        stack->available -= bytes_size;
        void* allocated_memory = &stack->memory[stack->marker];
        stack->previous_marker = stack->marker;
        stack->marker += bytes_size;
        return allocated_memory;
    }   
    printf("ERROR allocating stack memory\n");
    return NULL;
}



void free_stack_to_market(StackMemory* stack){
    stack->used += stack->marker;
    stack->available += stack->marker;
    stack->used -= stack->marker;
}

void* allocate_memory(int size){
    if(actual_free_memory > size){
        memory_used += size;
        actual_free_memory -= size;
        void* allocated_memory = &engine_memory[memory_marker];
        previous_marker = memory_marker;
        memory_marker += size;
        return allocated_memory;
    }   
    printf("ERROR engine memory\n");
    return NULL;
}

void* allocate_stack_memory_alignmed(int bytes_size, int alignment){
    int expanded_size_bytes = bytes_size + alignment;
    unsigned long int raw_memory = allocate_memory(expanded_size_bytes);

    int mask = (alignment - 1);
    unsigned long int misalignment = (raw_memory & mask);
    unsigned long int adjustment = alignment - misalignment;

    unsigned long int aligned_adress = raw_memory + adjustment;
    return (void*)aligned_adress;
}

void clear_engine_memory(){
    free(engine_memory);
}

void free_to_marker(int marker){
    memory_used += memory_marker - marker;
    actual_free_memory += memory_marker - marker;
    memory_marker -= memory_marker - marker;
}   