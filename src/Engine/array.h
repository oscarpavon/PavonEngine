#ifndef VECTOR_H
#define VECTOR_H

#include "../vertex.h"

#include <memory.h>


typedef struct Array{
    bool initialized;
    int element_capacity;
    size_t actual_bytes_size;
    size_t count;
    size_t element_bytes_size;
    size_t bytes_capacity;
    void * data;
}Array;

void init_array(Array * array, size_t element_bytes_size, int count);

void add_to_array(Array* array, void* element);

void *get_from_array(Array*,int index);

void clean_array(Array* array);


#endif // !VECTOR_H