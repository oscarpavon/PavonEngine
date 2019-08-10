
#include "render.h"
#include "../../engine.h"

Model* uv_model;

void init_UV_draw(Model* model){
    new_empty_model();
    init_array(&selected_model->vertex_array,sizeof(Vertex),model->vertex_array.count);
    for( int i = 0; i<model->vertex_array.count; i++){
        Vertex* vertex = get_from_array(&model->vertex_array,i);
        vec3 position;
        glm_vec3_copy(VEC3(vertex->uv[0],vertex->uv[1],0),position);
        add_to_array(&selected_model->vertex_array,position);
    }
    
    init_model_gl_buffers(selected_model);
    selected_model->index_buffer_id = model->index_buffer_id;
    selected_model->index_array.count = model->index_array.count;
    
    selected_model->shader = create_engine_shader(standart_vertex_shader,standart_fragment_shader);
    uv_model = selected_model;
}

void draw_UV(){
    if(!uv_model){
        if(selected_element){
            init_UV_draw(selected_model);
        }else
        {
            return;
        }
        
    }
        
    if(!uv_model->vertex_array.initialized){
        
        return;
    }
        
        update_draw_vertices(uv_model->shader, uv_model->vertex_buffer_id,uv_model->model_mat);
                
        //glDrawArrays(GL_POINTS, 0, uv_model->vertex_array.count);
        draw_simgle_model(uv_model);
}