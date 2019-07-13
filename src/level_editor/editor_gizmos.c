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

Array debug_objects;

typedef struct DebugLine{
    bool initialized;
    vec3 start;
    vec3 end;
    float duration;
    vec4 color;
    VertexArray vertex_array;
    GLuint vertex_buffer_id;
    GLuint shader;
}DebugLine;

void add_debug_line(vec3 start, vec3 end){
    DebugLine new_line;
    memset(&new_line,0,sizeof(DebugLine));
    add_element_to_array(&debug_objects, &new_line);
    DebugLine* pnew_line = get_element_from_array(&debug_objects,debug_objects.count-1);
    glm_vec3_copy(start,pnew_line->start);
    glm_vec3_copy(end,pnew_line->end);

}

void init_line_vertices(DebugLine* line){
    init_vertex_array(&line->vertex_array, 2);

    struct Vertex vert = {{line->start[0],line->start[1], line->start[2]},{0,0}};
    struct Vertex vert2 = {{line->end[0],line->end[1], line->end[2]},{0,0}};


    add_vextex_to_array(&line->vertex_array, vert);
    add_vextex_to_array(&line->vertex_array, vert2);

    init_static_gpu_vertex_buffer(&line->vertex_array,&line->vertex_buffer_id);

}

void init_line(DebugLine* line){
    init_line_vertices(line);
    line->shader = glCreateProgram();
    init_shader(line->shader, standart_vertex_shader,standart_fragment_shader);
}

void draw_grid_lines(){
    for(int i = 0 ; i< debug_objects.count ; i++){
        DebugLine* line = get_element_from_array(&debug_objects,i);
        if(line->initialized == true){
            update_draw_vertices(line->shader, line->vertex_buffer_id);
            glLineWidth(1);   
            glDrawArrays(GL_LINES, 0, line->vertex_array.count);
            continue;
        }          
        init_line(line);
        line->initialized = true;
    }

}

void draw_gizmos(){
    if(can_draw_skeletal_bones)   
        draw_skeletal_bones();

    if(can_draw_gizmos){
        draw_grid_lines();

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
    
    init_array(&debug_objects, sizeof(DebugLine));

    load_editor_element("editor/transform.gltf","editor/transform_gizmo.jpg");
    load_editor_element("editor/rotate.gltf", "editor/rotate_gizmo.png");
    load_editor_element("editor/camera.gltf", "editor/camera_gizmo.jpg");
    load_editor_element("editor/player_start.gltf", "editor/player_start_gizmo.jpg");

    can_draw_gizmos = true;
    can_draw_skeletal_bones = false;

    draw_translate_gizmo = false;
    draw_rotate_gizmo = false;

    add_debug_line((vec3){0,0,5},(vec3){0,0,-5});
    add_debug_line((vec3){0,5,0},(vec3){0,-5,0});
    add_debug_line((vec3){5,0,0},(vec3){-5,0,0});
}