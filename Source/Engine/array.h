#ifndef ARRAY_H
#define ARRAY_H

#include "../Engine/numbers.h"
#include <stdbool.h>

typedef struct Array{
    bool initialized;
    bool isPointerToPointer;
    bool block;
    int element_capacity;
    u32 actual_bytes_size;
    u32 count;
    u32 element_bytes_size;
    u32 bytes_capacity;
    void * data;
}Array;

int array_init(Array * array, u32 element_bytes_size, int count);

void array_add(Array* array, void* element);

void *array_get(Array*,int index);

void array_clean(Array* array);

inline static void* array_pop(Array* array){
	return array_get(array,array->count-1);
}
#endif // !ARRAY_H
