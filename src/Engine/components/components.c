#include "components.h"

void add_component_to_selected_element(int component_byte_size, void* new_component){
    ElementComponent new_element_component;
    memset(&new_element_component,0,sizeof(ElementComponent));
    
    new_element_component.id = components_id_count;
    new_element_component.bytes_size = component_byte_size;
    new_element_component.data = allocate_stack_memory(&created_components,component_byte_size);

    add_element_to_array(&components,&new_element_component);    


    ElementComponent * created_component = get_element_from_array(&components,components_id_count);
    selected_element->components = created_component;
    
    components_id_count++;
}

void init_sphere_component(SphereComponent* component){
    component->type = SPHERE_COMPONENT;
    new_empty_model();
    Model* sphere_model = get_element_from_array(&engine_native_models,0);
    memcpy(selected_model,sphere_model,sizeof(Model));
    selected_model->shader = create_engine_shader(standart_vertex_shader,standart_fragment_shader);
    
}