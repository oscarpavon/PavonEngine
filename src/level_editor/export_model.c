#include "export_model.h"
#include <stdbool.h>

#define CGLTF_WRITE_IMPLEMENTATION
#include "../third_party/cgltf_write.h"
#include "../Engine/log.h"
#include "../engine.h"
#include "editor.h"
#include "../third_party/cgltf.h"

#include <ctype.h>

int data_count = 0;
cgltf_data* data_array[20];

static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                '4', '5', '6', '7', '8', '9', '+', '/'};

static char *decoding_table = NULL;
static int mod_table[] = {0, 2, 1};
 
 
char *base64_encode(const unsigned char *data,
                    size_t input_length,
                    size_t *output_length) {
 
    *output_length = 4 * ((input_length + 2) / 3);
 
    char *encoded_data = malloc(*output_length+1);
    if (encoded_data == NULL) return NULL;
 
    for (int i = 0, j = 0; i < input_length;) {
 
        uint32_t octet_a = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_b = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_c = i < input_length ? (unsigned char)data[i++] : 0;
 
        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;
 
        encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
    }
 
    for (int i = 0; i < mod_table[input_length % 3]; i++)
        encoded_data[*output_length - 1 - i] = '=';
    
    encoded_data[*output_length] = '\0';
    return encoded_data;
}
 
 void build_decoding_table() {
 
    decoding_table = malloc(256);
 
    for (int i = 0; i < 64; i++)
        decoding_table[(unsigned char) encoding_table[i]] = i;
}

unsigned char *base64_decode(const char *data,
                             size_t input_length,
                             size_t *output_length) {
 
    if (decoding_table == NULL) build_decoding_table();
 
    if (input_length % 4 != 0) return NULL;
 
    *output_length = input_length / 4 * 3;
    if (data[input_length - 1] == '=') (*output_length)--;
    if (data[input_length - 2] == '=') (*output_length)--;
 
    unsigned char *decoded_data = malloc(*output_length);
    if (decoded_data == NULL) return NULL;
 
    for (int i = 0, j = 0; i < input_length;) {
 
        uint32_t sextet_a = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_b = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_c = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_d = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
 
        uint32_t triple = (sextet_a << 3 * 6)
        + (sextet_b << 2 * 6)
        + (sextet_c << 1 * 6)
        + (sextet_d << 0 * 6);
 
        if (j < *output_length) decoded_data[j++] = (triple >> 2 * 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 1 * 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 0 * 8) & 0xFF;
    }
 
    return decoded_data;
} 

void base64_cleanup() {
    free(decoding_table);
}


void load_mesh_for_proccess(const char* path){
    File new_file;

    load_file(path,&new_file);

    cgltf_options options = {0};
   
    cgltf_result result = cgltf_parse(&options,new_file.data,new_file.size_in_bytes, &data_array[data_count]);

    if (result != cgltf_result_success){
        LOG("Model no loaded: %s \n", new_file.path);        
    }
    data_count++;
    
}

void prepare_mesh_data(ComponentDefinition* component){
    if(component->type == STATIC_MESH_COMPONENT){
        StaticMeshComponent* mesh_component = component->data;
        unsigned int *model_id_in_loaded_models = get_from_array(&mesh_component->meshes,0);
        const char* path = get_from_array(&texts,*model_id_in_loaded_models);
        //load_mesh_for_proccess(path);
    }
}

void float_to_char(float number,unsigned char* char_number){
    uint8_t      bytes[sizeof(float)];
    *(float*)(bytes) = number;  // convert float to bytes
    char_number[0] = bytes[0];
    char_number[1] = bytes[1];
    char_number[2] = bytes[2];
    char_number[3] = bytes[3];
}

void uint8_to_char(unsigned short int number, unsigned char* char_number){
    uint8_t      bytes[sizeof(unsigned short int)];
    *(unsigned short int*)(bytes) = number;
    char_number[0] = bytes[0];
    char_number[1] = bytes[1];
}

typedef struct Encoded{
    size_t char_len;
    int byte_size;
    char* data;
}Encoded;

Encoded vertices_encoded[3];
int vertices_encoded_count = 0;

void vec3_to_base64_encoded_bytes(vec3 position, Encoded* encoded){
    uint8_t      bytes[sizeof(float)];
    *(float*)(bytes) = position[0];  

    unsigned char data[12] = {bytes[0],bytes[1],bytes[2],bytes[3]};

    *(float*)(bytes) = position[1]; 
    data[4] = bytes[0];
    data[5] = bytes[1];
    data[6] = bytes[2];
    data[7] = bytes[3];
   
    *(float*)(bytes) = position[2]; 
    data[8] = bytes[0];
    data[9] = bytes[1];
    data[10] = bytes[2];
    data[11] = bytes[3];

    Encoded encoded_data;
    encoded_data.char_len = 0;    
    encoded_data.data = base64_encode(data,12,&encoded_data.char_len);
    encoded_data.byte_size = 12;

    memcpy(encoded,&encoded_data,sizeof(Encoded));
  
}

void float_to_bytes(float x, float y, float z){
    uint8_t      bytes[sizeof(float)];
    *(float*)(bytes) = x;  // convert float to bytes
    //printf("bytes = [ 0x%.2x, 0x%.2x, 0x%.2x, 0x%.2x ]\r\n", bytes[0], bytes[1], bytes[2], bytes[3]);

    unsigned char data[12] = {bytes[0],bytes[1],bytes[2],bytes[3]};
    //printf("Character [ %c , %c , %c , %c ]\n",data[0],data[1],data[2],data[3]);

    *(float*)(bytes) = y;  // convert float to bytes
    data[4] = bytes[0];
    data[5] = bytes[1];
    data[6] = bytes[2];
    data[7] = bytes[3];
   
    *(float*)(bytes) = z;  // convert float to bytes
    data[8] = bytes[0];
    data[9] = bytes[1];
    data[10] = bytes[2];
    data[11] = bytes[3];

    Encoded encoded_data;
    encoded_data.char_len = 0;    
    encoded_data.data = base64_encode(data,12,&encoded_data.char_len);
    encoded_data.byte_size = 12;
    printf("Encoded: %s\n",encoded_data.data);

    memcpy(&vertices_encoded[vertices_encoded_count],&encoded_data,sizeof(Encoded));
    vertices_encoded_count++;

  
}

Array array_vertices_encoded;
Array array_indices_encoded;
typedef unsigned char UCharInBytes[2];
static const char encoded_header[] = {"data:application/octet-stream;base64,"};

int previous_indices_count = 0;

void prepare_indices_data_from_model(Model* model){
    
    unsigned char indices_charcters[model->index_array.count*2];
    unsigned short int count = 0;

    for(int i = 0; i<model->index_array.count; i++){
        unsigned short int* index = get_from_array(&model->index_array,i);
        unsigned short int index_offset = (*index + previous_indices_count);  
        UCharInBytes uchar_int_bytes;
        uint8_to_char(index_offset,uchar_int_bytes);
        indices_charcters[count] = uchar_int_bytes[0];
        indices_charcters[count+1] = uchar_int_bytes[1];
        count +=2;      
    }
    Encoded indices_encodes;
    indices_encodes.data = base64_encode(indices_charcters,model->index_array.count*2,&indices_encodes.char_len);
    indices_encodes.byte_size = model->index_array.count*2;
    add_to_array(&array_indices_encoded,&indices_encodes); 
    
    previous_indices_count += model->vertex_array.count;
    
    printf("Indices encoded: %s\n",indices_encodes.data);
}

typedef struct CodedData{
    int buffer_bytes_count;
    const char* coded_buffer;
}CodedData;

CodedData merge_all_coded_data(){
    int bytes_count = 0;
    int char_count = 0;
    for(int i = 0; i<array_vertices_encoded.count ; i++){
        Encoded* vertex_encoded = get_from_array(&array_vertices_encoded,i);
        bytes_count += vertex_encoded->byte_size;
       
        char_count += vertex_encoded->char_len;
    }
    
    for(int i =0; i<array_indices_encoded.count; i++){
        Encoded* index_encode = get_from_array(&array_indices_encoded,i);
        char_count += index_encode->char_len;
        bytes_count += index_encode->byte_size;
    }

    char* new_buffer = malloc(strlen(encoded_header)+char_count+1);
    memset(new_buffer,0,strlen(encoded_header)+char_count+1);
    strcat(new_buffer,encoded_header);

    for(int i = 0; i<array_vertices_encoded.count ; i++){
        Encoded* vertex_encoded = get_from_array(&array_vertices_encoded,i);
        strcat(new_buffer,vertex_encoded->data);
    }

    for(int i =0; i<array_indices_encoded.count; i++){
        Encoded* index_encode = get_from_array(&array_indices_encoded,i);
        strcat(new_buffer,index_encode->data);
    }

    CodedData data;
    data.coded_buffer = new_buffer;
    data.buffer_bytes_count = bytes_count;

    return data;
}

void prepare_vertices_data_from_model(Model* model){   
    
    for(int i = 0; i<model->vertex_array.count; i++){
        Vertex* vertex = get_from_array(&model->vertex_array,i);
        mat4 position;
        glm_mat4_identity(position);
        glm_translate(position,vertex->postion);
        glm_translate(position,selected_element->transform->position);
        
        Encoded new_encoded;
        vec3_to_base64_encoded_bytes(position[3],&new_encoded);
        add_to_array(&array_vertices_encoded,&new_encoded);
    }

 
}

void create_triangle_data(){
        float_to_bytes(0,0,0);
    float_to_bytes(0,1,0);
    float_to_bytes(0,0,1);
    
    unsigned char indices_char[3][2];

    uint8_to_char(0,&indices_char[0][0]);
    uint8_to_char(1,&indices_char[1][0]);
    uint8_to_char(2,&indices_char[2][0]);

    unsigned char indices_charcters[] = 
                    {indices_char[0][0],indices_char[0][1],
                    indices_char[1][0],indices_char[1][1],
                    indices_char[2][0],indices_char[2][1]};
 
    Encoded indices_encodes;
    indices_encodes.data = base64_encode(indices_charcters,6,&indices_encodes.char_len);
    indices_encodes.byte_size = 6;
    printf("uint char : %s\n",indices_encodes.data);

    int bytes_count = 0;
    int char_count = 0;
    for(int i = 0; i<3;i++){
        bytes_count += vertices_encoded[i].byte_size;
       
        char_count += vertices_encoded[i].char_len;
        
    } 
    
    printf("Byte size gltf URI: %i\n",bytes_count);   
   

    char* new_buffer = malloc(strlen(encoded_header)+char_count+1);
    memset(new_buffer,0,strlen(encoded_header)+char_count+1);
    strcat(new_buffer,encoded_header);
    for(int i = 0; i<3;i++){
        strcat(new_buffer,vertices_encoded[i].data);
    }   

    strcat(new_buffer,indices_encodes.data);

    printf("Uri: %s\n",new_buffer);
}

int vertex_count_merged = 0;
int indices_count_merged = 0;

void data_count_merged(ComponentDefinition* component){
    if(component->type == STATIC_MESH_COMPONENT){
        StaticMeshComponent* mesh_component = component->data;
        unsigned int *mode_id = get_from_array(&mesh_component->meshes,mesh_component->meshes.count-1);
        Model* model = get_from_array(actual_model_array,*mode_id);
        vertex_count_merged += model->vertex_array.count;
        indices_count_merged += model->index_array.count;
    }
}

void encode_vertices(ComponentDefinition* component){
    selected_element = component->parent;
    if(component->type == STATIC_MESH_COMPONENT){
        StaticMeshComponent* mesh_component = component->data;
        unsigned int *mode_id = get_from_array(&mesh_component->meshes,mesh_component->meshes.count-1);
        Model* model = get_from_array(actual_model_array,*mode_id);
        prepare_vertices_data_from_model(model);
    }
}

void encode_indices(ComponentDefinition* component){
    if(component->type == STATIC_MESH_COMPONENT){
        StaticMeshComponent* mesh_component = component->data;
        unsigned int *mode_id = get_from_array(&mesh_component->meshes,mesh_component->meshes.count-1);
        Model* model = get_from_array(actual_model_array,*mode_id);
        prepare_indices_data_from_model(model);
    }
}

int export_gltf(const char *name){
    //for_each_element_components(&prepare_mesh_data);

    load_mesh_for_proccess("test/triangle.gltf");
    cgltf_data* data1 = data_array[0];
    cgltf_data* data2 = data_array[1];

    cgltf_data new_data;
    memcpy(&new_data, data1, sizeof(cgltf_data));
   // memset(&new_data,0,sizeof(cgltf_data));

    for_each_element_components(&data_count_merged);
    init_array(&array_vertices_encoded,sizeof(Encoded),vertex_count_merged);
    init_array(&array_indices_encoded,sizeof(Encoded),indices_count_merged);

    for_each_element_components(&encode_vertices);
    for_each_element_components(&encode_indices);

    CodedData coded_data = merge_all_coded_data();
    new_data.buffers[0].uri = coded_data.coded_buffer;
    new_data.buffers[0].size = coded_data.buffer_bytes_count;
    new_data.buffer_views[0].size = vertex_count_merged * 12;
    new_data.buffer_views[1].size = indices_count_merged * 2;
    new_data.buffer_views[1].offset = vertex_count_merged * 12;
    new_data.accessors[0].count = vertex_count_merged;
    new_data.accessors[1].count = indices_count_merged;

    cgltf_options options = {0};
    cgltf_data* data_to_export  = data_array[0];
    
    cgltf_result result = cgltf_write_file(&options, name, data_to_export);
    if (result != cgltf_result_success)
    {
        LOG("Error can't save\n");
        return -1;
    }
    cgltf_free(data_to_export);

    LOG("Exported\n");
    data_count = 0;
    clean_array(&array_indices_encoded);
    clean_array(&array_vertices_encoded);
    return 0;
}