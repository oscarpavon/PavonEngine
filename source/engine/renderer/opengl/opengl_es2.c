#include <GLES2/gl2.h>
#include "opengl_es2.h"
#include <signal.h>
#include <engine/log.h>
#include <engine/components/components.h>
#include <engine/camera.h>
#include <engine/engine.h>

void GPU_buffers_create_for_model(Model* model){
    Array* vertex_array = &model->vertex_array;
    Array* index_array = &model->index_array;

    glGenBuffers(1,&model->vertex_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER,model->vertex_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, vertex_array->count * sizeof(struct Vertex) , vertex_array->data, GL_STATIC_DRAW);

    glGenBuffers(1,&model->index_buffer_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,model->index_buffer_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_array->count * sizeof(u8), index_array->data , GL_STATIC_DRAW);

}

void load_texture_to_GPU(Texture* texture){
    glGenTextures(1, &texture->id);
    glBindTexture(GL_TEXTURE_2D, texture->id);

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, texture->image.width,
                    texture->image.heigth, 0,
                    GL_RGB, GL_UNSIGNED_BYTE, texture->image.pixels_data);

    free_image(&texture->image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);

    check_error("Texture to GPU");
		texture->gpu_loaded = true;
}

void pe_mat_skinned(GLuint shader, GLuint buffer, mat4 matrix){

  glUseProgram(shader);

  glBindBuffer(GL_ARRAY_BUFFER, buffer);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex),
                        (void *)0);

  GLint model_uniform = get_uniform_location(shader, "model");

  GLint projection_uniform = get_uniform_location(shader, "projection");
  GLint view_uniform = get_uniform_location(shader, "view");
  GLint joints_matrices_uniform = get_uniform_location(shader, "joint_matrix");

  glUniformMatrix4fv(model_uniform, 1, GL_FALSE, &matrix[0][0]);
  glUniformMatrix4fv(projection_uniform, 1, GL_FALSE,
                     &main_camera.projection[0][0]);
  glUniformMatrix4fv(view_uniform, 1, GL_FALSE, &main_camera.view[0][0]);
  check_send_matrix_error("view");

  SkinnedMeshComponent *skin_component =
      get_component_from_selected_element(COMPONENT_SKINNED_MESH);
  glUniformMatrix4fv(joints_matrices_uniform,
                     skin_component->node_uniform.joint_count, GL_FALSE,
                     skin_component->node_uniform.joints_matrix);
  check_send_matrix_error("Skin");

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 4, GL_FLOAT, false, sizeof(Vertex),
                        (void *)offsetof(Vertex, joint));

  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 4, GL_FLOAT, false, sizeof(Vertex),
                        (void *)offsetof(Vertex, weight));
}
void update_draw_vertices(GLuint shader, GLuint buffer, mat4 matrix){

    glUseProgram(shader);   

    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(struct Vertex),(void*)0);

    GLint mvp_uniform =  get_uniform_location(shader,"MVP");
    if(mvp_uniform == -1){
        
        GLint model_uniform = get_uniform_location(shader,"model");

        GLint projection_uniform = get_uniform_location(shader,"projection");
        GLint view_uniform = get_uniform_location(shader,"view");
        GLint joints_matrices_uniform = get_uniform_location(shader,"joint_matrix");

        glUniformMatrix4fv(model_uniform, 1, GL_FALSE, &matrix[0][0]);
        glUniformMatrix4fv(projection_uniform, 1, GL_FALSE, &main_camera.projection[0][0]);
        glUniformMatrix4fv(view_uniform, 1, GL_FALSE, &main_camera.view[0][0]);
        check_send_matrix_error("view");
        
				SkinnedMeshComponent* skin_component = get_component_from_selected_element(COMPONENT_SKINNED_MESH);
        glUniformMatrix4fv(joints_matrices_uniform,skin_component->node_uniform.joint_count , 
													GL_FALSE, skin_component->node_uniform.joints_matrix);
        check_send_matrix_error("Skin");
        
        glEnableVertexAttribArray(2);
				glVertexAttribPointer(2, 4, GL_FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, joint));

				glEnableVertexAttribArray(3);
				glVertexAttribPointer(3, 4, GL_FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, weight));
        
    }else{
        glUniformMatrix4fv(mvp_uniform, 1, GL_FALSE, &matrix[0][0]);
        check_send_matrix_error("MVP");
    }    

}

void draw_vertices_like(GLenum mode, Model* model, vec4 color){
    update_draw_vertices(model->shader,model->vertex_buffer_id,model->model_mat);
    //send_color_to_shader(model->shader,color);
    glDrawArrays(GL_POINTS,0,model->vertex_array.count);
    check_error("simple draw");
}

void draw_two_dimention_element(DrawData* data, vec2 position , vec2 size, vec4 color){
    glDisable(GL_CULL_FACE);

    glUseProgram(data->shader);
    glBindTexture(GL_TEXTURE_2D,data->texture);

    two_dimension_screen_space_send_matrix(data->shader, size, position);

    glBindBuffer(GL_ARRAY_BUFFER,data->vertex);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(struct Vertex),(void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,2, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (void*)offsetof(struct Vertex, uv));

    send_color_to_shader(data->shader,color);

    glDrawArrays(GL_TRIANGLE_STRIP,0,4);        

    check_error("Two dimension");
}

void draw_model_with_color(Model* model, GLenum mode, vec4 color){
    update_draw_vertices(model->shader, model->vertex_buffer_id, model->model_mat);
    
    send_color_to_shader(model->shader,color);

    check_error("color matrix error");
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,model->index_buffer_id);
    
    glDrawElements(mode,model->index_array.count, GL_UNSIGNED_SHORT, (void*)0);
}

void draw_model_like(Model* model, GLenum mode){
    update_draw_vertices(model->shader, model->vertex_buffer_id, model->model_mat);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,model->index_buffer_id);
    
    glDrawElements(mode,model->index_array.count, GL_UNSIGNED_SHORT, (void*)0);
}

void pe_render_skinned_model(Model * new_model){
    
		glBindTexture(GL_TEXTURE_2D,new_model->texture.id);
   
	 	pe_mat_skinned(new_model->shader,new_model->vertex_buffer_id,new_model->model_mat);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,2, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (void*)offsetof(struct Vertex, uv));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,new_model->index_buffer_id);

    if(new_model->index_array.count == 0)
        LOG("Index is equal to 0, model not render\n");
    glDrawElements(GL_TRIANGLES, new_model->index_array.count , GL_UNSIGNED_SHORT, (void*)0);

    check_error("sigle model error");
}
void draw_simgle_model(Model * new_model){
    mat4 mvp;      
    update_mvp(new_model->model_mat, mvp);  
    
		glBindTexture(GL_TEXTURE_2D,new_model->texture.id);
   
	 	update_draw_vertices(new_model->shader,new_model->vertex_buffer_id,mvp);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,2, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (void*)offsetof(struct Vertex, uv));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,new_model->index_buffer_id);

    if(new_model->index_array.count == 0)
        LOG("Index is equal to 0, model not render\n");
    glDrawElements(GL_TRIANGLES, new_model->index_array.count , GL_UNSIGNED_SHORT, (void*)0);

    check_error("sigle model error");
}
