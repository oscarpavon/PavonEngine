#include "editor_gizmos.h"
#include "../shader.h"

GLuint color_fragment_shader;

Array debug_objects;
Array bounding_boxes;


typedef struct Grid{
    bool initialized;
    vec2 size;    
    Array vertex_array;
    GLuint vertex_buffer_id;
    GLuint shader;
}Grid;


Grid new_grid;

typedef struct DebugLine{
    bool initialized;
    vec3 start;
    vec3 end;
    float duration;
    vec4 color;    
    Array vertex_array;
    GLuint vertex_buffer_id;
    GLuint shader;
}DebugLine;

void add_debug_line(vec3 start, vec3 end){
    DebugLine new_line;
    memset(&new_line,0,sizeof(DebugLine));
    add_to_array(&debug_objects, &new_line);
    DebugLine* pnew_line = get_from_array(&debug_objects,debug_objects.count-1);
    glm_vec3_copy(start,pnew_line->start);
    glm_vec3_copy(end,pnew_line->end);

}

bool bounding_box_initialized = false;
void init_selected_object_bounding_box_vertices(){
    if(bounding_box_initialized == false && selected_element != NULL) {
        actual_model_array = &bounding_boxes;

        Model new_model;
        memset(&new_model,0,sizeof(Model));
        add_to_array(actual_model_array,&new_model);
        selected_model = get_from_array(actual_model_array,actual_model_array->count-1);
        glm_mat4_identity(selected_model->model_mat);
        selected_model->id = model_id_count;
        
       
        init_array(&selected_model->vertex_array,sizeof(Vertex),50);
        

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

       add_to_array(&selected_model->vertex_array,&min);
       add_to_array(&selected_model->vertex_array,&vert7);

       add_to_array(&selected_model->vertex_array,&vert6);
       add_to_array(&selected_model->vertex_array,&min);

       add_to_array(&selected_model->vertex_array,&vert4);
       add_to_array(&selected_model->vertex_array,&vert6);

       add_to_array(&selected_model->vertex_array,&vert7);
       add_to_array(&selected_model->vertex_array,&vert4);


       add_to_array(&selected_model->vertex_array,&vert8);
       add_to_array(&selected_model->vertex_array,&vert6);

       add_to_array(&selected_model->vertex_array,&min);
       add_to_array(&selected_model->vertex_array,&vert3);


       add_to_array(&selected_model->vertex_array,&vert8);
       add_to_array(&selected_model->vertex_array,&vert3);

       add_to_array(&selected_model->vertex_array,&vert7);
       add_to_array(&selected_model->vertex_array,&vert5);

        add_to_array(&selected_model->vertex_array,&vert8);
       add_to_array(&selected_model->vertex_array,&max);
    
       add_to_array(&selected_model->vertex_array,&max);
       add_to_array(&selected_model->vertex_array,&vert5);
       
       add_to_array(&selected_model->vertex_array,&vert5);
       add_to_array(&selected_model->vertex_array,&vert3);

       add_to_array(&selected_model->vertex_array,&max);
       add_to_array(&selected_model->vertex_array,&vert4);

       init_static_gpu_vertex_buffer(&selected_model->vertex_array,&selected_model->vertex_buffer_id);

       selected_model->shader = create_engine_shader(standart_vertex_shader,color_fragment_shader);
       bounding_box_initialized = true;
   }

}

void update_bounding_vertices_array(Model* model){
    Model* box = get_from_array(&bounding_boxes,0);
    for(int i = 0; i < box->vertex_array.count ; i++){
        vec3 new_vertex_pos;
        struct Vertex* vertex = get_from_array(&box->vertex_array,i);
        glm_vec3_rotate_m4(model->model_mat,vertex->postion,vertex->postion);
    }
}


void draw_bounding_box(){
    if(bounding_box_initialized == true){
        Model* bounding_model = get_from_array(&bounding_boxes,bounding_boxes.count-1);
        update_bounding_vertices_array(selected_element->model);
        //update_gpu_vertex_data(&bounding_model->vertex_array,bounding_model->vertex_buffer_id);
        mat4 model;
        glm_mat4_identity(model);
        update_draw_vertices(bounding_model->shader, bounding_model->vertex_buffer_id, model);
        GLint uniform_color = glGetUniformLocation(bounding_model->shader,"color");
        
        glUniform4fv(uniform_color, 1, (vec4){0.6,1,0,1});
        GLenum error;
        error = glGetError();
        if(error != GL_NO_ERROR){
            LOGW("[X] Send uniform error, Error %08x \n",error);
        }
        glDrawArrays(GL_LINES, 0, bounding_model->vertex_array.count);
        return;
    }
    init_selected_object_bounding_box_vertices();
}

void init_line_vertices(DebugLine* line){
    init_array(&line->vertex_array, sizeof(Vertex),2);

    struct Vertex vert = {{line->start[0],line->start[1], line->start[2]},{0,0}};
    struct Vertex vert2 = {{line->end[0],line->end[1], line->end[2]},{0,0}};


    add_to_array(&line->vertex_array, &vert);
    add_to_array(&line->vertex_array, &vert2);

    init_static_gpu_vertex_buffer(&line->vertex_array,&line->vertex_buffer_id);

}


void update_line_vertices(DebugLine* line){
    struct Vertex vert = {{line->start[0],line->start[1], line->start[2]},{0,0}};
    struct Vertex vert2 = {{line->end[0],line->end[1], line->end[2]},{0,0}};

    Vertex* vertices = get_from_array(&line->vertex_array,0);
    memcpy(&vertices[0], &vert, sizeof(struct Vertex));
    memcpy(&vertices[1], &vert2, sizeof(struct Vertex));
}

void init_line(DebugLine* line){
    init_line_vertices(line);
    line->shader = create_engine_shader(standart_vertex_shader,color_fragment_shader); 
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

void draw_axis_lines(){
    for(int i = 0 ; i< debug_objects.count ; i++){
        DebugLine* line = get_from_array(&debug_objects,i);
        if(line->initialized == true){
            mat4 model;
            glm_mat4_identity(model);            
            update_draw_vertices(line->shader, line->vertex_buffer_id,model);
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
    line = get_from_array(&debug_objects,0);
    glm_vec4_copy((vec4){0,0,1,1},line->color);

    line = get_from_array(&debug_objects,2);
    glm_vec4_copy((vec4){1,0,0,1},line->color);

    line = get_from_array(&debug_objects,1);
    glm_vec4_copy((vec4){0,1,0,1},line->color);

}
void draw_camera_direction(){
    if(selected_element != NULL && selected_element->type == ELEMENT_TYPE_CAMERA){
        vec3 direction;
        CameraComponent* camera = get_from_array(&components,0);
        vec3 look_pos;
        glm_vec3_add(selected_element->position, camera->front, look_pos);        
        glm_vec3_sub(look_pos,selected_element->position,direction);

        DebugLine* line = get_from_array(&debug_objects,2);
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

void init_grid_greometry(){
    init_array(&new_grid.vertex_array, sizeof(Vertex), 500);

    for(int i = 1; i < 6; i++){
        for(int o = 1; o < 6; o++){
            Vertex vertex1;
            Vertex vertex2;
            Vertex vertex3;
            Vertex vertex4;
            Vertex vertex5;
            Vertex vertex6;
            Vertex vertex7;
            Vertex vertex8;

            glm_vec3_copy((vec3){i,o,0}, vertex1.postion);
            glm_vec3_copy((vec3){-i,o,0}, vertex2.postion);
            
            glm_vec3_copy((vec3){o,i,0}, vertex3.postion);
            glm_vec3_copy((vec3){o,-i,0}, vertex4.postion);

            glm_vec3_copy((vec3){i,-o,0}, vertex5.postion);
            glm_vec3_copy((vec3){-i,-o,0}, vertex6.postion);

            glm_vec3_copy((vec3){-o,i,0}, vertex7.postion);
            glm_vec3_copy((vec3){-o,-i,0}, vertex8.postion);

            add_to_array(&new_grid.vertex_array,&vertex1);
            add_to_array(&new_grid.vertex_array,&vertex2);
            add_to_array(&new_grid.vertex_array,&vertex3);
            add_to_array(&new_grid.vertex_array,&vertex4);
            add_to_array(&new_grid.vertex_array,&vertex5);
            add_to_array(&new_grid.vertex_array,&vertex6);
            add_to_array(&new_grid.vertex_array,&vertex7);
            add_to_array(&new_grid.vertex_array,&vertex8);
        }
        
    }

    init_static_gpu_vertex_buffer(&new_grid.vertex_array,&new_grid.vertex_buffer_id);
    new_grid.shader = create_engine_shader(standart_vertex_shader,color_fragment_shader); 

}

void draw_grid(){
    mat4 model;
    glm_mat4_identity(model);            
    update_draw_vertices(new_grid.shader, new_grid.vertex_buffer_id,model);
    glLineWidth(1);            
    GLint uniform_color = glGetUniformLocation(new_grid.shader,"color");
    vec4 color = {0,0,0,1};
    glUniform4fv(uniform_color, 1, color);

    glDrawArrays(GL_LINES, 0, new_grid.vertex_array.count);
}

void draw_gizmos(){
    if(can_draw_skeletal_bones)   
        draw_skeletal_bones();
    
    draw_axis_lines();

    draw_grid();

    if(can_draw_gizmos){
        //draw_bounding_box();
        //draw_camera_direction();

    
        for(int i = 0; i< editor_elements.count ; i++){
            Element* element = get_from_array(&editor_elements,i);
            if(element->type == ELEMENT_TYPE_CAMERA){
                Model* actual_gizmo = get_from_array(&gizmos,2);
                if(selected_element != NULL){
                    TransformComponent* transform = get_component_from_selected_element(TRASNFORM_COMPONENT);
                    if(transform)
                        glm_mat4_copy(transform->model_matrix, actual_gizmo->model_mat);
                }
                draw_simgle_model(actual_gizmo);
                
            }
            if(element->type == ELEMENT_TYPE_PLAYER_START){
                Model* actual_gizmo = get_from_array(&gizmos,3);
                if(selected_element != NULL){
                    TransformComponent* transform = get_component_from_selected_element(TRASNFORM_COMPONENT);
                    if(transform)
                        glm_mat4_copy(transform->model_matrix, actual_gizmo->model_mat);
                }
                draw_simgle_model(actual_gizmo);
                
            }
        }
        
        glClear(GL_DEPTH_BUFFER_BIT);
        if(draw_translate_gizmo){
            Model* actual_gizmo = get_from_array(&gizmos,0);
            if(editor_mode == EDITOR_DEFAULT_MODE){                
                if(selected_element != NULL){
                    TransformComponent* transform = get_component_from_selected_element(TRASNFORM_COMPONENT);
                    if(transform)
                        glm_mat4_copy(transform->model_matrix, actual_gizmo->model_mat);
                }
                draw_simgle_model(actual_gizmo);
            }
            if(editor_mode == EDITOR_MODE_GUI_EDITOR){
                if(selected_button == NULL)
                    return;
                glUseProgram(actual_gizmo->shader);
                update_button_matrix(actual_gizmo->shader, selected_button->size, selected_button->position);
                glBindTexture(GL_TEXTURE_2D, actual_gizmo->texture.id);
                glBindBuffer(GL_ARRAY_BUFFER,actual_gizmo->vertex_buffer_id);

                glEnableVertexAttribArray(0);

                glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(struct Vertex),(void*)0);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,actual_gizmo->index_buffer_id);

                glDrawElements(GL_TRIANGLES, actual_gizmo->index_array.count , GL_UNSIGNED_SHORT, (void*)0);

            }
        }
        if(draw_rotate_gizmo){
            Model* actual_gizmo = get_from_array(&gizmos,1);
            if(selected_element != NULL){
                TransformComponent* transform = get_component_from_selected_element(TRASNFORM_COMPONENT);
                if(transform)
                    glm_mat4_copy(transform->model_matrix, actual_gizmo->model_mat);
            }
            draw_simgle_model(actual_gizmo);
        }
        
        
    }    
}

void init_gizmos(){
    init_array(&gizmos,sizeof(Model),10);   
    
    init_array(&bounding_boxes,sizeof(Model),10);

    init_array(&debug_objects, sizeof(DebugLine),300);

    load_model_to_array(&gizmos,"editor/transform.gltf","editor/transform_gizmo.jpg");
    load_model_to_array(&gizmos,"editor/rotate.gltf", "editor/rotate_gizmo.png");
    load_model_to_array(&gizmos,"editor/camera.gltf", "editor/camera_gizmo.jpg");
    load_model_to_array(&gizmos,"editor/player_start.gltf", "editor/player_start_gizmo.jpg");
    

    color_fragment_shader = compile_shader(color_shader_src,GL_FRAGMENT_SHADER);

    can_draw_gizmos = true;
    can_draw_skeletal_bones = false;

    draw_translate_gizmo = false;
    draw_rotate_gizmo = false;

    add_debug_line((vec3){0,0,5},(vec3){0,0,-5});
    add_debug_line((vec3){0,5,0},(vec3){0,-5,0});
    add_debug_line((vec3){5,0,0},(vec3){-5,0,0});

    //add_debug_line((vec3){1,1,1},(vec3){0,1,0});

    init_grid_greometry();

    
}