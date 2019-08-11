#include "HLOD_factory.h"
#include "editor.h"

HLODBoxComponent* current_HLOD_box_component;
void check_is_inside(ComponentDefinition* component_definition){

    if(component_definition->type == STATIC_MESH_COMPONENT){
        StaticMeshComponent* mesh = component_definition->data;
        
        if(glm_aabb_contains(current_HLOD_box_component->bounding_box,mesh->bounding_box)){
            LOG("%s\n",component_definition->parent->name);
        }
    }
}

void generate_HLODS(){
    for(int i = 0; i< HLOD_cluster_count; i++){
       Element* elmeent = HLOD_clusters[i];
       current_HLOD_box_component = get_component_from_element(elmeent,COMPONENT_HLOD_BOX);
       for_each_element_components(check_is_inside);
    }    
}