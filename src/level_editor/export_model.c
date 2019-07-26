#include "export_model.h"
#include <stdbool.h>

#define CGLTF_WRITE_IMPLEMENTATION
#include "../third_party/cgltf_write.h"
#include "../Engine/log.h"
#include "../engine.h"
#include "editor.h"
#include "../third_party/cgltf.h"


int data_count = 0;
cgltf_data* data_array[20];

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

void export_mesh(ComponentDefinition* component){
    if(component->type == STATIC_MESH_COMPONENT){
        StaticMeshComponent* mesh_component = component->data;
        unsigned int *model_id_in_loaded_models = get_from_array(&mesh_component->meshes,0);
        const char* path = get_from_array(&texts,*model_id_in_loaded_models);
        load_mesh_for_proccess(path);
    }
}
int export_gltf(const char *name){
    for_each_element_components(&export_mesh);

    

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
    return 0;
}