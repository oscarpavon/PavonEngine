#include "model.h"
#define CGLTF_IMPLEMENTATION
#include "third_party/cgltf.h"
#include "stdio.h"
#include <vec3.h>

#include "file_loader.h"

void load_uv(cgltf_data* data, VertexArray* out_vertex_array){


  for(size_t i = 0 ; i < data->accessors[1].count ; i++){

    cgltf_accessor_read_float(&data->accessors[1], i, &out_vertex_array->vertices[i].uv[0], 2);

  }


}
void load_indices(cgltf_data* data, IndexArray* index_array){
    init_index_array(index_array,data->meshes[0].primitives[0].indices->count);

    for(size_t i = 0 ; i < data->meshes[0].primitives[0].indices->count ; i++){
        unsigned short int index= cgltf_accessor_read_index(data->meshes[0].primitives[0].indices,i);
        add_index_to_array(index_array,index);
    }



}

void load_primitives(cgltf_data* data, VertexArray* out_vertex_array){

    init_vertex_array(out_vertex_array,data->accessors[0].count);

    for(size_t v = 0 ; v < data->accessors[0].count ; v++){
            struct Vertex vertex;
            vertex.uv[0] = 0;
            vertex.uv[1] = 0;
            cgltf_accessor_read_float(&data->accessors[0],v,&vertex.postion[0],3);
            add_vextex_to_array(out_vertex_array,vertex);
    }

}

void load_model(const char* path , struct Model* model){
    File new_file;

    load_file(path,&new_file);

    cgltf_options options = {0};
    cgltf_data* data = NULL;

    cgltf_result result = cgltf_parse(&options,new_file.data,new_file.size_in_bytes, &data);

    if (result != cgltf_result_success){
         LOGW("Model no loaded: %s \n", new_file.path);
         return;
    }
    cgltf_load_buffers(&options,data,new_file.path);

    load_primitives(data,&model->vertex_array);
    load_indices(data, &model->index_array);
    load_uv(data,&model->vertex_array);
    LOGW("gltf loaded. \n");

    cgltf_free(data);
    close_file(&new_file);
}


static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
  if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
      strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
    return 0;
  }
  return -1;
}

void parse_json(const char* json_file, size_t json_file_size){
  jsmn_parser p;
  jsmntok_t t[10];
  jsmn_init(&p);
 
  int result = jsmn_parse(&p,json_file,json_file_size,t,10);

  for(size_t i = 1 ; i < 10 ; i++){
     if (jsoneq(json_file, &t[i], "id") == 0) {
       long number = 600;
       printf("id found\n");
       printf("- UID: %.*s\n", t[i + 1].end - t[i + 1].start,
      json_file + t[i + 1].start);
     
      //number = strtol(json_file + t[i + 1].start , t[i + 1].end - t[i + 1].start , 10);

      printf("the number: %i\n",number);
       i++;
     }
  }
  printf("json parsed\n");
}