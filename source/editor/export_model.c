#include "export_model.h"
#include <stdbool.h>

#define CGLTF_WRITE_IMPLEMENTATION
#include "../ThirdParty/cgltf_write.h"
#include "../engine/log.h"
#include "../engine/engine.h"
#include "editor.h"
#include "../ThirdParty/cgltf.h"

#include <ctype.h>


#include "HLOD/HLOD_factory.h"

#include "../engine/vertex.h"

bool data_export_in_binary = false;
u32 data_export_binary_bytes_size = 0;
FILE* glb_file;
u32 data_export_bytes_writed = 0;
u32 data_export_JSON_bytes_size = 0;

int data_count = 0;
cgltf_data* data_array[20];


unsigned char* vertices_uchar;
unsigned char* uv_uchar;
unsigned char* indices_uchar;

int offset_vertices = 0;
int offset_indices = 0;
int offset_uv = 0;

int indices_char_bytes = 0;
int vertices_char_bytes = 0;
int uv_char_bytes = 0;

int vertex_count_merged = 0;
int indices_count_merged = 0;
int UV_count_merged = 0;

vec3 box[2];

typedef unsigned char UCharInBytes[2];
static const char encoded_header[] = {"data:application/octet-stream;base64,"};

int previous_indices_count = 0;

static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                '4', '5', '6', '7', '8', '9', '+', '/'};

static char decoding_table[256];;
static int mod_table[] = {0, 2, 1};
 
 
char *base64_encode(const unsigned char *data,
                    size_t input_length,
                    size_t *output_length) {
 
    *output_length = 4 * ((input_length+2) / 3);
 
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
    
    return encoded_data;
}
 
 void build_decoding_table() {
  
    for (int i = 0; i < 64; i++)
        decoding_table[(unsigned char) encoding_table[i]] = i;
}


void load_mesh_for_proccess(const char* path){
    File new_file;

    load_file(path,&new_file);

    cgltf_options options = {0};
   
    cgltf_result result = cgltf_parse(&options,new_file.data,new_file.size_in_bytes, &data_array[data_count]);

    if (result != cgltf_result_success){
        LOG("Model no loaded: %s \n", new_file.path);        
    }
    //data_count++;
    
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

void float_to_unsigned_char(float* numbers, unsigned char* data , int byte_count){
    uint8_t      bytes[sizeof(float)];
    *(float*)(bytes) = numbers[0]; 
    int bytes_counter = 0;
    int float_offset = 0;
    for(int i = 0; i<byte_count ; i++){
        if(bytes_counter==4){
            float_offset++;
            *(float*)(bytes) = numbers[float_offset]; 
            bytes_counter = 0;
        }
        data[i] = bytes[bytes_counter];
        bytes_counter++;
    }
}

void float_array_to_base64_encoded_bytes(float* position, int count, Encoded* encoded){
    //float size equal 4
    uint8_t      bytes[sizeof(float)];
    *(float*)(bytes) = position[0];  

    int byte_count = sizeof(float) * count;

    unsigned char data[byte_count];
    int bytes_counter = 0;
    int float_offset = 0;
    for(int i = 0; i<byte_count ; i++){
        if(bytes_counter==4){
            float_offset++;
            *(float*)(bytes) = position[float_offset]; 
            bytes_counter = 0;
        }
        data[i] = bytes[bytes_counter];
        bytes_counter++;
    }

    Encoded encoded_data;
    encoded_data.char_len = 0;    
    encoded_data.data = base64_encode(data,byte_count,&encoded_data.char_len);
    encoded_data.byte_size = byte_count;

    memcpy(encoded,&encoded_data,sizeof(Encoded));
  
}

int prepare_vertices_data_from_model(PModel* model){
       
    vec2 UV_values[model->vertex_array.count];
    vec3 vertex_position_array[model->vertex_array.count];
    
    for(int i = 0; i<model->vertex_array.count; i++){
        Vertex* vertex = array_get(&model->vertex_array,i);
        mat4 position;
        glm_mat4_identity(position);
        glm_translate(position,vertex->position);
        glm_translate(position,selected_element->transform->position);
        
        vertex_position_array[i][0] = position[3][0];
        vertex_position_array[i][1] = position[3][1];
        vertex_position_array[i][2] = position[3][2];
        UV_values[i][0] = vertex->uv[0];
        UV_values[i][1] = vertex->uv[1];
    }

    if(data_export_in_binary){ 
        u8 indices[model->index_array.count];
        for(u8 i = 0; i<model->index_array.count; i++){
            u8* index = array_get(&model->index_array,i);
            indices[i] = *index;
        }

        u32 binary_byte_size = sizeof(vertex_position_array) + sizeof(UV_values) + sizeof(indices);
        fwrite(&binary_byte_size,sizeof(u32),1,glb_file);
        LOG("Binary byte size: %i\n",binary_byte_size);

        unsigned int t = 0x004E4942;
        fwrite(&t,sizeof(t),1,glb_file);
        data_export_bytes_writed += 3;


        fwrite(vertex_position_array,sizeof(vertex_position_array),1,glb_file); 
        fwrite(UV_values,sizeof(UV_values),1,glb_file); 

        data_export_bytes_writed += sizeof(UV_values);
        data_export_bytes_writed += sizeof(vertex_position_array);
        data_export_bytes_writed += sizeof(indices);        


        fwrite(indices,sizeof(indices),1,glb_file); 


        return 0;
    } 

    /* Base 64 */
    int vertex_position_char_byte_count = sizeof(float) * model->vertex_array.count*3;
    unsigned char vertex_position_char[vertex_position_char_byte_count];
    memset(vertex_position_char,0,vertex_position_char_byte_count);
    float_to_unsigned_char(vertex_position_array,vertex_position_char,vertex_position_char_byte_count);

    memcpy(&vertices_uchar[offset_vertices],vertex_position_char,vertex_position_char_byte_count);
    offset_vertices += vertex_position_char_byte_count;

    int uv_position_char_byte_count = sizeof(float) * model->vertex_array.count*2;
    unsigned char uv_position_char[uv_position_char_byte_count];
    memset(uv_position_char,0,uv_position_char_byte_count);
    float_to_unsigned_char(UV_values,uv_position_char,uv_position_char_byte_count);

    memcpy(&uv_uchar[offset_uv],uv_position_char,uv_position_char_byte_count);
    offset_uv += uv_position_char_byte_count;

    int indices_char_bytes_count = model->index_array.count*2;
    unsigned char indices_charcters[indices_char_bytes_count];
    unsigned short int count = 0;

    for(int i = 0; i<model->index_array.count; i++){
        unsigned short int* index = array_get(&model->index_array,i);
        unsigned short int index_offset = (*index + previous_indices_count);  
        UCharInBytes uchar_int_bytes;
        uint8_to_char(index_offset,uchar_int_bytes);
        indices_charcters[count] = uchar_int_bytes[0];
        indices_charcters[count+1] = uchar_int_bytes[1];
        count +=2;      
    }
    
    previous_indices_count += model->vertex_array.count;

    memcpy(&indices_uchar[offset_indices],indices_charcters,indices_char_bytes_count);
    offset_indices += indices_char_bytes_count;

    return 0;
}

typedef struct CodedData{
    int buffer_bytes_count;
    char* coded_buffer;
}CodedData;

CodedData merge_all_encoded_data(){
    int uchar_buffer_size = vertices_char_bytes + uv_char_bytes + indices_char_bytes;
    unsigned char vertex_char[uchar_buffer_size+1];
    memset(vertex_char,0,uchar_buffer_size+1);
    memcpy(vertex_char,vertices_uchar,vertices_char_bytes);
    memcpy(&vertex_char[vertices_char_bytes],uv_uchar,uv_char_bytes);
    memcpy(&vertex_char[vertices_char_bytes+uv_char_bytes],indices_uchar,indices_char_bytes);


    int char_len = 0;
    int input_length = sizeof(vertex_char);

    char_len = 4 * ((input_length+2) / 3);
 
    char data_encoded_64[char_len+1];
    memset(data_encoded_64,0,sizeof(data_encoded_64));
    
    for (int i = 0, j = 0; i < input_length;) {
 
        uint32_t octet_a = i < input_length ? (unsigned char)vertex_char[i++] : 0;
        uint32_t octet_b = i < input_length ? (unsigned char)vertex_char[i++] : 0;
        uint32_t octet_c = i < input_length ? (unsigned char)vertex_char[i++] : 0;
 
        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;
 
        data_encoded_64[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
        data_encoded_64[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
        data_encoded_64[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
        data_encoded_64[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
    }
 
    for (int i = 0; i < mod_table[input_length % 3]; i++)
        data_encoded_64[char_len - 1 - i] = '=';



    int bytes_count = sizeof(vertex_char);
    int char_count = char_len;
    
    char new_buffer[strlen(encoded_header) + char_count];
    memset(new_buffer,0,sizeof(new_buffer));
    strcat(new_buffer,encoded_header);
    strcat(new_buffer,data_encoded_64);

    CodedData data;
    data.coded_buffer = malloc(sizeof(new_buffer)+1);
    memset(data.coded_buffer,0,sizeof(new_buffer)+1);
    memcpy(data.coded_buffer,new_buffer,sizeof(new_buffer));
    data.buffer_bytes_count = bytes_count;

    return data;
}

void data_count_merged(ComponentDefinition* component){
    if(component->type == STATIC_MESH_COMPONENT){
        StaticMeshComponent* mesh_component = component->data;
        unsigned int *mode_id = array_get(&mesh_component->meshes,mesh_component->meshes.count-1);
        PModel* model = array_get(actual_model_array,*mode_id);
        vertex_count_merged += model->vertex_array.count;
        indices_count_merged += model->index_array.count;
        UV_count_merged += model->vertex_array.count;
        vertices_char_bytes += sizeof(float) * (model->vertex_array.count*3);
        uv_char_bytes += sizeof(float) * (model->vertex_array.count*2);
        indices_char_bytes += 2 * (model->index_array.count);        
    }
}

void data_export_buffer_count_and_AABB(ComponentDefinition* component, cgltf_data* data){
    selected_element = component->parent;
    if(component->type == STATIC_MESH_COMPONENT){
        StaticMeshComponent* mesh_component = component->data;
        unsigned int *mode_id = array_get(&mesh_component->meshes,mesh_component->meshes.count-1);
        PModel* model = array_get(actual_model_array,*mode_id);

        if(mesh_component->bounding_box[1][0] > box[1][0]){
            box[1][0] = mesh_component->bounding_box[1][0];
        }
        if(mesh_component->bounding_box[1][1] > box[1][1]){
            box[1][1] = mesh_component->bounding_box[1][1];
        }
        if(mesh_component->bounding_box[1][2] > box[1][2]){
            box[1][2] = mesh_component->bounding_box[1][2];
        }

        if(mesh_component->bounding_box[0][0] < box[0][0]){
            box[0][0] = mesh_component->bounding_box[0][0];
        }

        if(mesh_component->bounding_box[0][1] < box[0][1]){
            box[0][1] = mesh_component->bounding_box[0][1];
        }
        
        if(mesh_component->bounding_box[0][2] < box[0][2]){
            box[0][2] = mesh_component->bounding_box[0][2];
        }

        int vertices_bytes = model->vertex_array.count * (3*sizeof(float));
        int uv_bytes = model->vertex_array.count * (2 *sizeof(float));
        int index_bytes = model->index_array.count * (sizeof(u8));

        data->buffers[0].size = vertices_bytes + uv_bytes + index_bytes;
    }
}

void encode_vertices(ComponentDefinition* component){
    selected_element = component->parent;
    if(component->type == STATIC_MESH_COMPONENT){
        StaticMeshComponent* mesh_component = component->data;
        unsigned int *mode_id = array_get(&mesh_component->meshes,mesh_component->meshes.count-1);
        PModel* model = array_get(actual_model_array,*mode_id);
        prepare_vertices_data_from_model(model);

        if(mesh_component->bounding_box[1][0] > box[1][0]){
            box[1][0] = mesh_component->bounding_box[1][0];
        }
        if(mesh_component->bounding_box[1][1] > box[1][1]){
            box[1][1] = mesh_component->bounding_box[1][1];
        }
        if(mesh_component->bounding_box[1][2] > box[1][2]){
            box[1][2] = mesh_component->bounding_box[1][2];
        }

        if(mesh_component->bounding_box[0][0] < box[0][0]){
            box[0][0] = mesh_component->bounding_box[0][0];
        }

        if(mesh_component->bounding_box[0][1] < box[0][1]){
            box[0][1] = mesh_component->bounding_box[0][1];
        }
        
        if(mesh_component->bounding_box[0][2] < box[0][2]){
            box[0][2] = mesh_component->bounding_box[0][2];
        }

        
        
    }
}

void data_export_finish(){

    cgltf_data* data_to_export  = data_array[0];
    cgltf_free(data_to_export);

    data_count = 0;
    vertex_count_merged = 0;
    indices_count_merged = 0;
    UV_count_merged = 0;
    previous_indices_count = 0;

    indices_char_bytes = 0;
    vertices_char_bytes = 0;
    uv_char_bytes = 0;

    offset_indices = 0;
    offset_vertices = 0;
    offset_uv = 0;

    data_export_bytes_writed = 0;
    data_export_JSON_bytes_size = 0;

    if(!data_export_in_binary){
        free(vertices_uchar);
        free(uv_uchar);
        free(indices_uchar);
    }

    data_export_in_binary = false;
}

void data_export_remove_JSON_white_space(char* json){
    int size = strlen(json);
    char buffer[size];
    memset(buffer,0,sizeof(buffer));
    strcpy(buffer,json);
    memset(json,0,size);

    bool can_remove_white_space = true;
    int valid_char_count = 0;
    bool checked_buffer = false;
    for (int i = 0; i < size; i++)
    {
        if(can_remove_white_space == false){
            if(checked_buffer==true){
                if(buffer[i] == ','){
                    can_remove_white_space = true;
                    checked_buffer = false;
                    continue;
                }
                continue;
            }
            if(buffer[i] == '\"'){
                can_remove_white_space = true;
            }
        }else{
            if(buffer[i] == '\"'){
                can_remove_white_space = false;
                if(checked_buffer == false){
                    if(buffer[i+1] == 'u'){
                        if( buffer[i+2] == 'r'){
                            if( buffer[i+3] == 'i'){
                                checked_buffer = true;
                                continue;
                            }

                        }
                    }
                }
            }           

        }

        if(can_remove_white_space){
            if(buffer[i] != ' '){
                if(buffer[i] != '\n'){
                    json[valid_char_count] = buffer[i];
                    valid_char_count++;
                }
            }
        }else{
            json[valid_char_count] = buffer[i];
            valid_char_count++;
        }
    }
    
}

int data_export_encoded_data(cgltf_data new_data, const char* name){
    char* saved_uri = NULL;
    char* new_path = NULL;
    if(!data_export_in_binary){
        CodedData coded_data = merge_all_encoded_data();
        new_data.buffers[0].uri = coded_data.coded_buffer;
        new_data.buffers[0].size = coded_data.buffer_bytes_count;
    }else{
        saved_uri = new_data.buffers[0].uri;
        for (u8 i = strlen(name); i > 0; i--)
        {
            if(name[i] == '/'){
                new_path = malloc(strlen(name)-(i+1));
                sprintf(new_path,"%s%s",&name[i+1],".bin");
                new_data.buffers[0].uri = new_path;
                break;
            }
        }
        
        
        //new_data.buffers[0].size = data_export_binary_bytes_size;
    }

    cgltf_buffer_view* buffer =  &new_data.buffer_views[0];
    buffer->size = vertex_count_merged * (sizeof(float)*3);

    buffer = &new_data.buffer_views[1];
    buffer->size = UV_count_merged * (sizeof(float)*2);
    buffer->offset = vertex_count_merged * (sizeof(float)*3);
    
    buffer = &new_data.buffer_views[2];
    buffer->size = indices_count_merged * sizeof(unsigned short int);
    buffer->offset = (vertex_count_merged * (sizeof(float)*3)) + (UV_count_merged * (sizeof(float)*2));

    new_data.accessors[0].count = vertex_count_merged;
    new_data.accessors[1].count = UV_count_merged;
    new_data.accessors[2].count = indices_count_merged;

    glm_vec3_copy(box[0],new_data.accessors[0].min);
    glm_vec3_copy(box[1],new_data.accessors[0].max);

    cgltf_options options = {0};
    cgltf_data* data_to_export  = data_array[0];
    
    cgltf_result result;
    if(!data_export_in_binary){
        result = cgltf_write_file(&options, name, data_to_export);
    }else{
       
        /* Write JSON in memory */
        size_t expected = cgltf_write(&options, NULL, 0, &new_data);
        char* buffer = (char*) malloc(expected);
        size_t actual = cgltf_write(&options, buffer, expected, &new_data);
        if (expected != actual) {
            fprintf(stderr, "Error: expected %zu bytes but wrote %zu bytes.\n", expected, actual);
        }
        //expected -= 2;// /n and random number
        data_export_remove_JSON_white_space(buffer);

        fwrite(buffer,strlen(buffer),1,glb_file);
        
        data_export_bytes_writed += strlen(buffer);
        data_export_JSON_bytes_size = strlen(buffer);
        LOG("JSON bytes size: %i\n",data_export_JSON_bytes_size);

        if ( (strlen(buffer) % 4) != 0 ) {
            LOG("Data analigned\n");
            int un = strlen(buffer) % 4;
            LOG("Unaligned %i\n",un);
            for (u8 i = 0; i < un; i++)
            {
                fprintf(glb_file," ");
                data_export_bytes_writed += 1;
                data_export_JSON_bytes_size += 1;
            }
            LOG("JSON afet align bytes size: %i\n",data_export_JSON_bytes_size);

        }
        
        free(buffer);
    }

    if (result != cgltf_result_success)
    {
        LOG("Error can't save\n");
        return -1;
    }
    
    new_data.buffers[0].uri = saved_uri;
    
    LOG("Exported: %s\n",name);

    if(data_export_in_binary){
        free(new_path);

    }

}


int data_export_select_element(const char* name,bool binary){
    data_export_in_binary = binary;

    char glb_path[strlen(name) + 4];
    sprintf(glb_path,"%s%s",name,".glb");
    glb_file = fopen(glb_path,"wb");

    fprintf(glb_file,"glTF");
    u32 version = 2;
    u32 file_size = 0;
    u32 json_bytes_size = 0;
    fwrite(&version,sizeof(u32),1,glb_file); 
    fwrite(&file_size,sizeof(u32),1,glb_file); 
    fwrite(&json_bytes_size,sizeof(u32),1,glb_file);

    fprintf(glb_file,"JSON");    
    
    memset(&box,0,sizeof(box));

    load_mesh_for_proccess("/home/pavon/PavonTheGame/Content/test/export_template_with_uv.gltf");
    cgltf_data* data1 = data_array[0];

    cgltf_data new_data;
    memcpy(&new_data, data1, sizeof(cgltf_data));
    
    ComponentDefinition* component = array_get(&selected_element->components,1);
    data_count_merged(component);
    if(!binary){
        vertices_uchar = malloc(vertices_char_bytes);
        indices_uchar = malloc(indices_char_bytes);
        uv_uchar = malloc(uv_char_bytes);
        encode_vertices(component);
        data_export_encoded_data(new_data,name);
    }else{
        data_export_buffer_count_and_AABB(component,&new_data);
        data_export_encoded_data(new_data,name);
        encode_vertices(component);
    } 

    version = 2;
    data_export_bytes_writed += 12;//header
    data_export_bytes_writed += 12;//two chunk length and chunk type of JSON

    LOG("GLB bytes size = %i\n",data_export_bytes_writed);

    rewind(glb_file);
    fprintf(glb_file,"glTF");

    file_size = data_export_bytes_writed;
    json_bytes_size = data_export_JSON_bytes_size;

    fwrite(&version,sizeof(u32),1,glb_file); 
    fwrite(&file_size,sizeof(u32),1,glb_file);

    fwrite(&json_bytes_size,sizeof(u32),1,glb_file);

    int glb_saved = fclose(glb_file); 
    data_export_finish();
}


int data_export_models_in_array(Array* array, const char *name){
    if(array->count == 0){
        LOG("Not exported, Array is empty\n");
        return -1;
    }
    memset(&box,0,sizeof(box));

    load_mesh_for_proccess("/home/pavon/PavonTheGame/Content/test/export_template_with_uv.gltf");
    cgltf_data* data1 = data_array[0];

    cgltf_data new_data;
    memcpy(&new_data, data1, sizeof(cgltf_data));

    
    for_each_element_components_in_array_of_pp(array,&data_count_merged);

    vertices_uchar = malloc(vertices_char_bytes);
    indices_uchar = malloc(indices_char_bytes);
    uv_uchar = malloc(uv_char_bytes);

    for_each_element_components_in_array_of_pp(array,&encode_vertices);


    return data_export_encoded_data(new_data,name);
}
