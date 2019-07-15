#ifndef VECTOR_H
#define VECTOR_H



#include "vertex.h"

#include <memory.h>

typedef struct vertex_array{    
    size_t size;
    size_t count;
    struct Vertex* vertices;
}VertexArray;

typedef struct index_array{
    size_t size;
    size_t count;
    unsigned short int * indices;
}IndexArray;

#include "model.h"

typedef struct model_array{
    size_t size;
    size_t count;
    size_t capacity;
    struct Model* models;
}ModelArray;


typedef struct Array{
    size_t actual_bytes_size;
    size_t count;
    size_t element_bytes_size;
    size_t bytes_capacity;
    void * data;
}Array;

void init_array(Array* array, size_t element_bytes_size);
void init_array_with_count(Array * array, size_t element_bytes_size, int count);
void add_element_to_array(Array* array, void* element);
void remove_element_from_array(Array* array);

void remove_last_element_from_model_array(ModelArray* array);

int init_vertex_array(VertexArray* array, size_t size);
int init_index_array(IndexArray* array, size_t size);

void add_vextex_to_array(VertexArray*, struct Vertex);
void add_index_to_array(IndexArray*, unsigned short int);

void *get_element_from_array(Array*,int index);

void clean_array(Array* array);
void clean_model_array(ModelArray* model_array);

#endif // !VECTOR_H