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
         LOGW("Model no loaded: %s \n", path);
         return;
    }
    cgltf_load_buffers(&options,data,path);

    load_primitives(data,&model->vertex_array);
    load_indices(data, &model->index_array);
    load_uv(data,&model->vertex_array);
    LOGW("gltf loaded. \n");

    cgltf_free(data);
    free(new_file.data);
}
