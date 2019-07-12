#include "editor_gizmos.h"
#include "../shader.h"



void load_editor_element(const char* path_model, const char* color_texture_path){   

    struct Model new_model;
    load_model(path_model,&new_model);
    glm_mat4_identity(new_model.model_mat);   

    new_model.shader = glCreateProgram();
    
    glAttachShader(new_model.shader, standart_vertex_shader);
    glAttachShader(new_model.shader, standart_fragment_shader);
    glLinkProgram(new_model.shader);

    glUseProgram(new_model.shader);

    new_model.texture.image = load_image(color_texture_path);

    init_model(&new_model);

    load_model_texture_to_gpu(&new_model);

    add_model_to_array(&gizmos,new_model);  

}

void draw_gizmos(){
    if(can_draw_skeletal_bones)   
        draw_skeletal_bones();

    if(can_draw_gizmos){
        for(int i = 0; i< editor_elements.count ; i++){
            Element* element = get_element_from_array(&editor_elements,i);
            if(element->type == ELEMENT_TYPE_CAMERA){
                Model* actual_gizmo = &gizmos.models[2];
                if(selected_element != NULL){
                    glm_mat4_copy(element->model->model_mat, actual_gizmo->model_mat);
                }
                draw_simgle_model(actual_gizmo);
                
            }
            if(element->type == ELEMENT_TYPE_PLAYER_START){
                Model* actual_gizmo = &gizmos.models[3];
                if(selected_element != NULL){
                    glm_mat4_copy(element->model->model_mat, actual_gizmo->model_mat);
                }
                draw_simgle_model(actual_gizmo);
                
            }
        }
        
        glClear(GL_DEPTH_BUFFER_BIT);
        if(draw_translate_gizmo){
            Model* actual_gizmo = &gizmos.models[0];
            if(selected_element != NULL){
                if(selected_element->model == NULL)
                    return;
                glm_mat4_copy(selected_element->model->model_mat, actual_gizmo->model_mat);
            }
            draw_simgle_model(actual_gizmo);
        }
        if(draw_rotate_gizmo){
            Model* actual_gizmo = &gizmos.models[1];
            if(selected_element != NULL){
                glm_mat4_copy(selected_element->model->model_mat, actual_gizmo->model_mat);
            }
            draw_simgle_model(actual_gizmo);
        }
        
        
    }    
}

void init_gizmos(){
    init_model_array(&gizmos,1);
    
    load_editor_element("editor/transform.gltf","editor/transform_gizmo.jpg");
    load_editor_element("editor/rotate.gltf", "editor/rotate_gizmo.png");
    load_editor_element("editor/camera.gltf", "editor/camera_gizmo.jpg");
    load_editor_element("editor/player_start.gltf", "editor/player_start_gizmo.jpg");

    can_draw_gizmos = true;
    can_draw_skeletal_bones = false;

    draw_translate_gizmo = false;
    draw_rotate_gizmo = false;


}