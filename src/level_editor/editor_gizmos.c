#include "editor_gizmos.h"
#include "../shader.h"

GLuint color_fragment_shader;

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
ModelArray bounding_boxes;

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
bool bounding_box_initialized = false;
void init_selected_object_bounding_box_vertices(){
    if(bounding_box_initialized == false && selected_element != NULL) {
        actual_model_array = &bounding_boxes;

        Model new_model;
        memset(&new_model,0,sizeof(Model));
        add_model_to_array(actual_model_array,new_model);
        selected_model = &actual_model_array->models[actual_model_array->count-1];
        glm_mat4_identity(selected_model->model_mat);
        selected_model->id = model_id_count;
        
       
        init_vertex_array(&selected_model->vertex_array,1);

        struct Vertex min;
        
        struct Vertex max;

        memset(&min,0,sizeof(struct Vertex));
        memset(&max,0,sizeof(struct Vertex));

        glm_vec3_copy(selected_element->model->min, min.postion);
        glm_vec3_copy(selected_element->model->max, max.postion);


        struct Vertex vert3;
        glm_vec3_copy((vec3){min.postion[0],max.postion[1],min.postion[2]},vert3.postion);
        
        struct Vertex vert4;
        glm_vec3_copy((vec3){max.postion[0],min.postion[1],max.postion[2]},vert4.postion);

        struct Vertex vert5;
        glm_vec3_copy((vec3){max.postion[0],max.postion[1],min.postion[2]},vert5.postion);

        struct Vertex vert6;
        glm_vec3_copy((vec3){min.postion[0],min.postion[1],max.postion[2]},vert6.postion);
        
        struct Vertex vert7;
        glm_vec3_copy((vec3){max.postion[0],min.postion[1],min.postion[2]},vert7.postion);
        
        struct Vertex vert8;
        glm_vec3_copy((vec3){min.postion[0],max.postion[1],max.postion[2]},vert8.postion);

        add_vextex_to_array(&selected_model->vertex_array,min);
        add_vextex_to_array(&selected_model->vertex_array,vert7);

        add_vextex_to_array(&selected_model->vertex_array,vert6);
        add_vextex_to_array(&selected_model->vertex_array,min);

        add_vextex_to_array(&selected_model->vertex_array,vert4);
        add_vextex_to_array(&selected_model->vertex_array,vert6);

        add_vextex_to_array(&selected_model->vertex_array,vert7);
        add_vextex_to_array(&selected_model->vertex_array,vert4);


        add_vextex_to_array(&selected_model->vertex_array,vert8);
        add_vextex_to_array(&selected_model->vertex_array,vert6);

        add_vextex_to_array(&selected_model->vertex_array,min);
        add_vextex_to_array(&selected_model->vertex_array,vert3);


        add_vextex_to_array(&selected_model->vertex_array,vert8);
        add_vextex_to_array(&selected_model->vertex_array,vert3);

        add_vextex_to_array(&selected_model->vertex_array,vert7);
        add_vextex_to_array(&selected_model->vertex_array,vert5);

         add_vextex_to_array(&selected_model->vertex_array,vert8);
        add_vextex_to_array(&selected_model->vertex_array,max);
        
        add_vextex_to_array(&selected_model->vertex_array,max);
        add_vextex_to_array(&selected_model->vertex_array,vert5);
        
        add_vextex_to_array(&selected_model->vertex_array,vert5);
        add_vextex_to_array(&selected_model->vertex_array,vert3);

        add_vextex_to_array(&selected_model->vertex_array,max);
        add_vextex_to_array(&selected_model->vertex_array,vert4);

        init_static_gpu_vertex_buffer(&selected_model->vertex_array,&selected_model->vertex_buffer_id);

        selected_model->shader = glCreateProgram();
        init_shader(selected_model->shader, standart_vertex_shader,color_fragment_shader);

        bounding_box_initialized = true;
    }
}

void draw_bounding_box(){
    if(bounding_box_initialized == true){
         Model* model = &bounding_boxes.models[bounding_boxes.count-1];
        update_draw_vertices(model->shader, model->vertex_buffer_id);
        GLint uniform_color = glGetUniformLocation(model->shader,"color");
        
        glUniform4fv(uniform_color, 1, (vec4){0.6,1,0,1});
        GLenum error;
        error = glGetError();
        if(error != GL_NO_ERROR){
            LOGW("[X] Send uniform error, Error %08x \n",error);
        }
        glDrawArrays(GL_LINES, 0, model->vertex_array.count);
        return;
    }
    init_selected_object_bounding_box_vertices();
}

void init_line_vertices(DebugLine* line){
    init_vertex_array(&line->vertex_array, 2);

    struct Vertex vert = {{line->start[0],line->start[1], line->start[2]},{0,0}};
    struct Vertex vert2 = {{line->end[0],line->end[1], line->end[2]},{0,0}};


    add_vextex_to_array(&line->vertex_array, vert);
    add_vextex_to_array(&line->vertex_array, vert2);

    init_static_gpu_vertex_buffer(&line->vertex_array,&line->vertex_buffer_id);

}

void update_line_vertices(DebugLine* line){
    struct Vertex vert = {{line->start[0],line->start[1], line->start[2]},{0,0}};
    struct Vertex vert2 = {{line->end[0],line->end[1], line->end[2]},{0,0}};

    memcpy(&line->vertex_array.vertices[0], &vert, sizeof(struct Vertex));
    memcpy(&line->vertex_array.vertices[1], &vert2, sizeof(struct Vertex));
}

void init_line(DebugLine* line){
    init_line_vertices(line);
    line->shader = glCreateProgram();
    init_shader(line->shader, standart_vertex_shader,color_fragment_shader);
    glm_vec4_copy((vec4){1,1,1,1},line->color);
   
}

void init_camera_frustrum_gizmo_geometry(float * proj, float *mv){ 

	// Get near and far from the Projection matrix.
	const double near = proj[11] / (proj[10] - 1.0);
	const double far = proj[11] / (1.0 + proj[10]);

	// Get the sides of the near plane.
	const double nLeft = near * (proj[2] - 1.0) / proj[0];
	const double nRight = near * (1.0 + proj[2]) / proj[0];
	const double nTop = near * (1.0 + proj[6]) / proj[5];
	const double nBottom = near * (proj[6] - 1.0) / proj[5];

	// Get the sides of the far plane.
	const double fLeft = far * (proj[2] - 1.0) / proj[0];
	const double fRight = far * (1.0 + proj[2]) / proj[0];
	const double fTop = far * (1.0 + proj[6]) / proj[5];
	const double fBottom = far * (proj[6] - 1.0) / proj[5];
  

}

void draw_grid_lines(){
    for(int i = 0 ; i< debug_objects.count ; i++){
        DebugLine* line = get_element_from_array(&debug_objects,i);
        if(line->initialized == true){            
            update_draw_vertices(line->shader, line->vertex_buffer_id);
            glLineWidth(1);            
            GLint uniform_color = glGetUniformLocation(line->shader,"color");
            
            glUniform4fv(uniform_color, 1, line->color);
            GLenum error;
            error = glGetError();
            if(error != GL_NO_ERROR){
                LOGW("[X] Send uniform error, Error %08x \n",error);
            }
            glDrawArrays(GL_LINES, 0, line->vertex_array.count);
            continue;
        }          
        init_line(line);
        line->initialized = true;
    }
    DebugLine* line;
    line = get_element_from_array(&debug_objects,0);
    glm_vec4_copy((vec4){0,0,1,1},line->color);

    line = get_element_from_array(&debug_objects,2);
    glm_vec4_copy((vec4){1,0,0,1},line->color);

    line = get_element_from_array(&debug_objects,1);
    glm_vec4_copy((vec4){0,1,0,1},line->color);

}
void draw_camera_direction(){
    if(selected_element != NULL && selected_element->type == ELEMENT_TYPE_CAMERA){
        vec3 direction;
        CameraComponent* camera = get_element_from_array(&components,0);
        vec3 look_pos;
        glm_vec3_add(selected_element->position, camera->front, look_pos);        
        glm_vec3_sub(look_pos,selected_element->position,direction);

        DebugLine* line = get_element_from_array(&debug_objects,2);
        glm_vec3_copy(selected_element->position,line->start);
        
        vec3 front_dir;
        glm_vec3_copy((vec3){0,-1,0},front_dir);

        vec3 direction_len;
        glm_vec3_mul((vec3){2,2,2},front_dir,direction_len);

        vec3 pos2;
        glm_vec3_add(selected_element->position,direction_len,pos2);
        
        glm_vec3_copy(pos2,line->end);
        update_line_vertices(line);
        update_gpu_vertex_data(&line->vertex_array,line->vertex_buffer_id);
    }
    
    
}

void draw_gizmos(){
    if(can_draw_skeletal_bones)   
        draw_skeletal_bones();

    if(can_draw_gizmos){
        draw_bounding_box();
        draw_camera_direction();

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
    
    init_model_array(&bounding_boxes,1);

    init_array(&debug_objects, sizeof(DebugLine));

    load_editor_element("editor/transform.gltf","editor/transform_gizmo.jpg");
    load_editor_element("editor/rotate.gltf", "editor/rotate_gizmo.png");
    load_editor_element("editor/camera.gltf", "editor/camera_gizmo.jpg");
    load_editor_element("editor/player_start.gltf", "editor/player_start_gizmo.jpg");

    color_fragment_shader = compile_shader(color_shader_src,GL_FRAGMENT_SHADER);

    can_draw_gizmos = true;
    can_draw_skeletal_bones = false;

    draw_translate_gizmo = false;
    draw_rotate_gizmo = false;

    add_debug_line((vec3){0,0,5},(vec3){0,0,-5});
    add_debug_line((vec3){0,5,0},(vec3){0,-5,0});
    add_debug_line((vec3){5,0,0},(vec3){-5,0,0});

    add_debug_line((vec3){1,1,1},(vec3){0,1,0});

    for(int i = 1; i < 6; i++){
        for(int o = 1; o < 6; o++){
            add_debug_line((vec3){i,o,0},(vec3){-i,o,0});
            add_debug_line((vec3){o,i,0},(vec3){o,-i,0});
            
            add_debug_line((vec3){i,-o,0},(vec3){-i,-o,0});
            add_debug_line((vec3){-o,i,0},(vec3){-o,-i,0});
        }
        
    }

    
}