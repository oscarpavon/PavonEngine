#include "components.h"

void add_component_to_selected_element(int component_byte_size, void* new_component, ComponentType type){
    ComponentDefinition new_element_component;
    memset(&new_element_component,0,sizeof(ComponentDefinition));
    new_element_component.type = type;
    new_element_component.id = components_id_count;
    new_element_component.parent = selected_element;
    new_element_component.bytes_size = component_byte_size;
    new_element_component.data = allocate_stack_memory_alignmed(component_byte_size,16);//;allocate_stack_memory(&created_components,component_byte_size);
    memcpy(new_element_component.data,new_component,component_byte_size);

    add_to_array(&selected_element->components,&new_element_component);    
    
    selected_element->components_count++;   
}

void init_sphere_component(SphereComponent* component){
    new_empty_model();
    Model* sphere_model = get_from_array(&engine_native_models,0);
    memcpy(selected_model,sphere_model,sizeof(Model));
    selected_model->id = model_id_count-1;
    selected_model->shader = create_engine_shader(standart_vertex_shader,standart_fragment_shader);
    component->model = selected_model;
}

void init_cube_component(CubeComponent* component){
    new_empty_model();
    Model* cube_model = get_from_array(&engine_native_models,1);
    memcpy(selected_model,cube_model,sizeof(Model));
    selected_model->id = model_id_count-1;
    selected_model->shader = create_engine_shader(standart_vertex_shader,standart_fragment_shader);
    component->model = selected_model;
}

void init_transfrom_component(TransformComponent* component){
    memset(component->position,0,sizeof(vec3));
    glm_vec3_copy(VEC3(1,1,1),component->scale);
    glm_mat4_identity(component->model_matrix);
}

void update_component(ComponentDefinition* element_component){
    switch (element_component->type)
    {
    case SPHERE_COMPONENT:{
        SphereComponent* component = &element_component->data[0];
        glm_mat4_copy(element_component->parent->transform->model_matrix,component->model->model_mat);
        add_to_array(&frame_draw_elements,&component->model);
        break;
    }
     case CUBE_COMPONENT:{
        CubeComponent* component = &element_component->data[0];
        glm_mat4_copy(element_component->parent->transform->model_matrix,component->model->model_mat);
        add_to_array(&frame_draw_elements,&component->model);
        break;
    }
    case TRASNFORM_COMPONENT:{
        if(element_component->parent->transform == NULL)
            element_component->parent->transform = &element_component->data[0];
        break;
    }      
    case STATIC_MESH_COMPONENT:{
        StaticMeshComponent* component = &element_component->data[0];
        glm_mat4_copy(element_component->parent->transform->model_matrix,component->model->model_mat);
        add_to_array(&frame_draw_elements,&component->model);
        break;
    }      
    default:
        break;
    }
}