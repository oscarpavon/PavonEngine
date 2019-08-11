#include "HLOD_factory.h"
#include "editor.h"

#include "Textures/texture_factory.h"
vec2 UV_tranlation_offset;
void* saved_vertex_data[6];
int saved_vertex_data_count = 0;
Model* saved_vertex_model[6];
int saved_vertex_model_count = 0;

HLODBoxComponent* current_HLOD_box_component;
void check_is_inside(ComponentDefinition* component_definition){
    
    if(component_definition->type == STATIC_MESH_COMPONENT){
        StaticMeshComponent* mesh = component_definition->data;
        
        if(glm_aabb_contains(current_HLOD_box_component->bounding_box,mesh->bounding_box)){
            
            add_to_array(&array_elements_for_HLOD_generation,&component_definition->parent);
            LOG("%s\n",component_definition->parent->name);
            unsigned int* id = get_from_array(&mesh->meshes,mesh->meshes.count-1);
            Model* model = get_from_array(actual_model_array,*id);
             
            Model new_model;
            memset(&new_model,0,sizeof(Model));
            init_array(&new_model.vertex_array,sizeof(Vertex),model->vertex_array.count);
            void* data = new_model.vertex_array.data;
            duplicate_model_data(&new_model,model);

            new_model.vertex_array.data = data;
            memcpy(new_model.vertex_array.data,model->vertex_array.data,model->vertex_array.actual_bytes_size);
           
            translate_UV(VEC3(UV_tranlation_offset[0],UV_tranlation_offset[1],0),&new_model,(vec2){0,0});
            UV_tranlation_offset[0] += 1;
            //UV_tranlation_offset[1] = 1;

            scale_UV(0.5, &new_model,(vec2){0,0});

            saved_vertex_model[saved_vertex_model_count] = model;
            saved_vertex_model_count++;
            saved_vertex_data[saved_vertex_data_count] = model->vertex_array.data;
            saved_vertex_data_count++;
            

            model->vertex_array.data = new_model.vertex_array.data;
        }
    }
}


void generate_HLODS(){
    UV_tranlation_offset[0] = 0;
    UV_tranlation_offset[1] = -2;
    if(!array_elements_for_HLOD_generation.initialized){
        init_array(&array_elements_for_HLOD_generation,sizeof(Element*),30);
        array_elements_for_HLOD_generation.isPointerToPointer = true;
    }
    clean_array(&array_elements_for_HLOD_generation);

    for(int i = 0; i< HLOD_cluster_count; i++){
       Element* elmeent = HLOD_clusters[i];
       current_HLOD_box_component = get_component_from_element(elmeent,COMPONENT_HLOD_BOX);
       for_each_element_components(check_is_inside);
    }   

     if(export_gltf("../assets/HLOD/out.gltf") == -1){
        LOG("Not exported\n");
        return;
    }

    saved_vertex_model[0]->vertex_array.data = saved_vertex_data[0];
    saved_vertex_model[1]->vertex_array.data = saved_vertex_data[1];
    
    merge_textures();

   
    //system("blender --python ../scripts/Blender/import.py");

    editor_add_HLOD_element();

}