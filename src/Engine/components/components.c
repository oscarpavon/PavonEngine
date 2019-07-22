#include "components.h"
#include "../../engine.h"

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
    previous_models_array = actual_model_array;
    actual_model_array = &engine_native_models;
    new_empty_model();
    actual_model_array = previous_models_array;
    Model* sphere_model = get_from_array(&engine_native_models,0);
    memcpy(selected_model,sphere_model,sizeof(Model));
    selected_model->shader = create_engine_shader(standart_vertex_shader,standart_fragment_shader);
    component->model = selected_model;
}

void init_cube_component(CubeComponent* component){
    previous_models_array = actual_model_array;
    actual_model_array = &engine_native_models;
    new_empty_model();
    actual_model_array = previous_models_array;
    Model* cube_model = get_from_array(&engine_native_models,1);
    memcpy(selected_model,cube_model,sizeof(Model));
    selected_model->shader = create_engine_shader(standart_vertex_shader,standart_fragment_shader);
    component->model = selected_model;
}

void init_camera_component(CameraComponent* component){
    previous_models_array = actual_model_array;
    actual_model_array = &engine_native_models;
    new_empty_model();
    actual_model_array = previous_models_array;
    Model* camera_model = get_from_array(&engine_native_models,2);
    memcpy(selected_model,camera_model,sizeof(Model));
    selected_model->shader = create_engine_shader(standart_vertex_shader,standart_fragment_shader);
    component->camera_gizmo = selected_model;

    glm_vec3_copy(VEC3(0,0,1),component->up),
    glm_vec3_copy((vec3){0,-1,0},component->front);
    glm_mat4_copy(main_camera.projection,component->projection);
    vec3 look_pos;
    glm_vec3_add(selected_element->transform->position, component->front, look_pos);
    glm_vec3_copy(VEC3(0,0,0),component->position);
    glm_lookat(selected_element->transform->position, look_pos, main_camera.up , component->view);

}


void change_view_to_camera_component(CameraComponent* camera_component){
    memcpy(&saved_camera,&main_camera, sizeof(CameraComponent));
    
    memcpy(&main_camera,camera_component,sizeof(CameraComponent));
}

void update_main_camera_with_camera_component_values(CameraComponent* camera_component){
    mat4 local;
    glm_mat4_identity(local);
    glm_translate(local,camera_component->position);

    mat4 global;
    glm_mat4_mul(selected_element->transform->model_matrix,local,global);
    vec3 global_position;
    glm_vec3_copy(VEC3(global[3][0],global[3][1],global[3][2]),main_camera.position);
    vec3 direction;
    vec3 direction_pivot;
    glm_vec3_add(selected_element->transform->position,VEC3(0,0,1.5),direction_pivot);
    glm_vec3_sub(direction_pivot,main_camera.position, direction);
    glm_vec3_normalize(direction);
    glm_vec3_copy(direction,main_camera.front);

    update_look_at(); 
}  

void init_transfrom_component(TransformComponent* component){
    memset(component->position,0,sizeof(vec3));
    glm_vec3_copy(VEC3(1,1,1),component->scale);
    glm_mat4_identity(component->model_matrix);
    glm_quat_identity(component->rotation);
}

void update_component(ComponentDefinition* element_component){
    switch (element_component->type)
    {
    case SPHERE_COMPONENT:{
        SphereComponent* component = &element_component->data[0];
        glm_mat4_copy(element_component->parent->transform->model_matrix,component->model->model_mat);
        add_to_array(&models_for_test_occlusion,&component->model);
        break;
    }
     case CUBE_COMPONENT:{
        CubeComponent* component = &element_component->data[0];
        glm_mat4_copy(element_component->parent->transform->model_matrix,component->model->model_mat);
        add_to_array(&models_for_test_occlusion,&component->model);
        break;
    }
    case TRASNFORM_COMPONENT:{
        if(element_component->parent->transform == NULL){
            TransformComponent* transform = get_component_from_element(element_component->parent,TRASNFORM_COMPONENT);
            element_component->parent->transform = transform;
        }

        break;
    }      
    case CAMERA_COMPONENT:{
        CameraComponent* component = &element_component->data[0];
        mat4 local;
        glm_mat4_identity(local);
        glm_translate(local,component->position);
   
        glm_mat4_mul(element_component->parent->transform->model_matrix, local , component->camera_gizmo->model_mat);      
        add_to_array(&models_for_test_occlusion,&component->camera_gizmo);
        
        break;
    }      
    case STATIC_MESH_COMPONENT:{
        StaticMeshComponent* component = &element_component->data[0];
        if(component->model == NULL){
            int model_id;          
            model_id = component->model_id;
            new_empty_model();
            Model* orinal_model = get_from_array(actual_model_array,model_id);
            duplicate_model_data(selected_model,orinal_model);
            selected_model->shader = create_engine_shader(standart_vertex_shader,standart_fragment_shader);
            component->model = selected_model;
            Texture* texture = get_from_array(current_textures_array,component->texture_id);
            component->model->texture.id = texture->id;
            
        }
        if(component->model == NULL){
            return;
        }
        
        glm_mat4_copy(element_component->parent->transform->model_matrix,component->model->model_mat);
        add_to_array(&models_for_test_occlusion,&component->model);
        break;
    }      
    default:
        break;
    }
}

void* get_component_from_selected_element(ComponentType type){
    if(!selected_element)
        return NULL;
    if(selected_element->components_count > 0){
        for(int i = 0; i< selected_element->components_count ; i++){       
            ComponentDefinition* component = get_from_array(&selected_element->components,i);
            if(component->type == type){
                return &component->data[0];
            }           
        }        
    }
    return NULL;
}
void* get_component_from_element(Element* element, ComponentType type){
    Element* previous_element = selected_element;
    selected_element = element;
    Element* result = get_component_from_selected_element(type);
    if(result){
        selected_element = previous_element;
        return result;
    }
    selected_element = previous_element;
    return NULL;
}

void add_transform_component_to_selected_element(){
    TransformComponent transform;
    init_transfrom_component(&transform);
    add_component_to_selected_element(sizeof(TransformComponent),&transform,TRASNFORM_COMPONENT);
    selected_element->transform = get_component_from_selected_element(TRASNFORM_COMPONENT);

}

void add_camera_component_to_selected_element(){
    CameraComponent camera_component;
    init_camera_component(&camera_component);
    add_component_to_selected_element(sizeof(CameraComponent), &camera_component, CAMERA_COMPONENT);

}