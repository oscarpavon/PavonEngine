#include "vector.h"
#include "vertex.h"

#include "utils.h"

#include "Engine/memory.h"


void init_array(Array * array, size_t element_bytes_size){
    array->count = 0;
    array->data = malloc(element_bytes_size);
    array->actual_bytes_size = 0;
    array->element_bytes_size = element_bytes_size;
    array->bytes_capacity = element_bytes_size;
}

void init_array_with_count(Array * array, size_t element_bytes_size, int count){
    if(array->initialized != true){
       
    }else{
        printf("Array ready initialized\n");
        return;
    }  
    array->count = 0;
    array->data = allocate_stack_memory_alignmed(element_bytes_size * count,16);
    array->actual_bytes_size = 0;
    array->element_bytes_size = element_bytes_size;
    array->bytes_capacity = element_bytes_size * count;
    array->initialized = true;
    array->element_capacity = count;
}

void add_element_to_array(Array* array, void* element){
    if(array->count == 0){
        array->actual_bytes_size += array->element_bytes_size;
        memcpy(array->data,element,array->element_bytes_size);
        array->count++;
        return;
    }
    if(array->bytes_capacity < array->actual_bytes_size + array->element_bytes_size){
        printf("Array need reallocation\n");//TODO: reallocation engine memory
    }
    size_t offset = array->actual_bytes_size;
    array->count++;
    array->actual_bytes_size += array->element_bytes_size;
    
    memcpy(array->data+(offset),element,array->element_bytes_size);
}

void *get_element_from_array(Array* array,int index){
    size_t offset = array->element_bytes_size;
    if(index == 0)
        return &array->data[0];
    if(index > array->count){
        printf("Element out of ranger, array count: %i\n",array->count);
        return NULL;
    }         
    return &array->data[0] + (index*offset);
}

void clean_array(Array* array){
    array->count = 0;    
    array->actual_bytes_size = 0;
}

int init_vertex_array(VertexArray* array, size_t size){
    array->count = 0;
    //array->vertices = calloc(size,sizeof(struct Vertex));
    array->vertices = allocate_memory(sizeof(struct Vertex));
    if(array->vertices == NULL){
        printf("Memory not allocated ERROR\n");
        return -1;
    }
    array->size = sizeof(struct Vertex) * size;
    return 0;
}

int init_index_array(IndexArray* array, size_t size){
    array->count = 0;
    array->indices = allocate_stack_memory(&indices, size * sizeof(unsigned short int) ) ;
    array->size = sizeof(unsigned short int) * size;
    return 0;
}



void remove_last_element_from_model_array(ModelArray* array){
    if(array->count==1){
        array->count--;
        array->size = sizeof(struct Model);
        Model* temp = malloc(sizeof(struct Model));
        free(array->models);
        array->models = temp;
        return;
    }

    array->count--;
    array->size -= sizeof(struct Model);
    Model* temp = malloc(sizeof(struct Model) * array->size);
    memcpy(temp,array->models,sizeof(struct Model) * array->size);
    free(array->models);
    array->models = temp;
    
}

void remove_element_from_array(Array* array){
    if(array->count==1){
        array->count--;
        array->actual_bytes_size = array->element_bytes_size;
        Model* temp = malloc(array->element_bytes_size);
        free(array->data);
        array->data = temp;
        return;
    }

    array->count--;
    array->actual_bytes_size -= array->element_bytes_size;
    Model* temp = malloc(array->actual_bytes_size);
    memcpy(temp,array->data,array->actual_bytes_size);
    free(array->data);
    array->data = temp;
    
}


void add_index_to_array(IndexArray* array, unsigned short int value){
    if(array->count == 0){
        memcpy(array->indices,&value,sizeof(unsigned short int));
        array->count++;
        return;
    }
    
    array->count++;
    array->size += sizeof(unsigned short int);    

    memcpy(&array->indices[array->count-1],&value,sizeof(unsigned short int));
    
}

void add_vextex_to_array(VertexArray *array, struct Vertex vertex){
    if(array->count == 0){
        memcpy(array->vertices,&vertex,sizeof(struct Vertex));
        array->count++;
        return;
    }

    array->count++;
    array->size += sizeof(struct Vertex);   
    
    /* struct Vertex* allocated_vertex = allocate_memory(sizeof(struct Vertex));
    if(allocated_vertex== NULL){
        printf("Memory not allocated ERROR\n");
    } */
    if(!array->vertices){
        printf("array no allocated\n");
    }
    memcpy(&array->vertices[array->count-1],&vertex,sizeof(struct Vertex));   
}


