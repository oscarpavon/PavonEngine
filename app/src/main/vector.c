#include "vector.h"
#include "vertex.h"

#include "utils.h"

void init_array(Array * array, size_t element_bytes_size){
    array->count = 0;
    array->data = malloc(element_bytes_size);
    array->actual_bytes_size = element_bytes_size;
    array->element_bytes_size = element_bytes_size;
}



int init_vertex_array(VertexArray* array, size_t size){
    array->count = 0;
    array->vertices = calloc(size,sizeof(struct Vertex));
    array->size = sizeof(struct Vertex) * size;
    return 0;
}

int init_index_array(IndexArray* array, size_t size){
    array->count = 0;
    array->indices = calloc(size,sizeof(unsigned short int));
    array->size = sizeof(unsigned short int) * size;
    return 0;
}

int init_model_array(ModelArray* array, size_t size){
    if(array->capacity != 0){
        void* temp = realloc(array->models, sizeof(struct Model) * (size+array->capacity));
        array->models = temp;
        array->size = sizeof(struct Model) * (size + array->capacity);
        array->capacity = size + array->capacity;
        return 0;
    }
    array->count = 0;
    array->models = malloc(sizeof(struct Model) * size);
    array->size = sizeof(struct Model) * size;
    array->capacity = size;
    return 0;
}

void clean_model_array(ModelArray* array){
    array->count = 0;
    free(array->models);
    array->models = calloc(1,sizeof(struct Model));
    array->size = sizeof(struct Model);
}

void clean_array(Array* array){
    array->count = 0;
    free(array->data);
    array->data = malloc(array->element_bytes_size);
    array->actual_bytes_size = array->element_bytes_size;
    
}

void add_model_to_array(ModelArray* array, struct Model model){
    if(array->count == 0){
        memcpy(array->models,&model,sizeof(struct Model));
        array->count++;
        return;
    }
    if(array->count < array->capacity ){
        array->count++;
        memcpy(&array->models[array->count-1],&model,sizeof(struct Model));    
        return;
    }
    array->count++;
    array->size += sizeof(struct Model);
    array->models = realloc(array->models,sizeof(struct Model) * array->size);
    if(!array->models){
        printf("array no allocated\n");
    }
    memcpy(&array->models[array->count-1],&model,sizeof(struct Model));
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

void *get_element_from_array(Array* array,int index){
    size_t offset = array->element_bytes_size;
    if(index == 0)
        return array->data;
    return &array->data[0] + (index*offset);
}

void add_index_to_array(IndexArray* array, unsigned short int value){
    if(array->count == 0){
        memcpy(array->indices,&value,sizeof(unsigned short int));
        array->count++;
        return;
    }
    
    array->count++;
    array->size += sizeof(unsigned short int);
    array->indices = realloc(array->indices,sizeof(unsigned short int) * array->size);
    if(!array->indices){
        printf("array no allocated\n");
    }
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
    array->vertices = realloc(array->vertices,sizeof(struct Vertex) * array->size);
    if(!array->vertices){
        printf("array no allocated\n");
    }
    memcpy(&array->vertices[array->count-1],&vertex,sizeof(struct Vertex));
    //array->vertices2[array->count-1] = vertex;
   
}


void add_element_to_array(Array* array, void* element){
    if(array->count == 0){
        memcpy(array->data,element,array->element_bytes_size);
        array->count++;
        return;
    }
    size_t offset = array->actual_bytes_size;
    array->count++;
    array->actual_bytes_size += array->element_bytes_size;
    void* buffer;
    buffer = realloc(array->data, array->actual_bytes_size);
    if(!buffer){
        LOGW("array no allocated\n");
    }
    memcpy(buffer+(offset),element,array->element_bytes_size);

    array->data = buffer;


}