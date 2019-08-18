#ifndef ARRAY_H
#define ARRAY_H

#include "../Engine/numbers.h"
#include <stdbool.h>

typedef struct Array{
    bool initialized;
    bool isPointerToPointer;
    int element_capacity;
    u32 actual_bytes_size;
    u32 count;
    u32 element_bytes_size;
    u32 bytes_capacity;
    void * data;
}Array;

int init_array(Array * array, u32 element_bytes_size, int count);

void add_to_array(Array* array, void* element);

void *get_from_array(Array*,int index);

void clean_array(Array* array);

#endif // !ARRAY_H