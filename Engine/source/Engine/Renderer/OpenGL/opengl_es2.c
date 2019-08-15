#include "GLES2/gl2.h"
#include "../../../model.h"
#include <signal.h>
#include "../../log.h"
#include "../../components/components.h"
#include "../../../camera.h"
#include "../../../engine.h"

static inline void mvp_error(const char* uniform_name){
    LOG("Uniform not found: %s\n",uniform_name);
    raise(SIGINT);
}


static inline void check_send_matrix_error(const char* message){
    GLenum error = glGetError();
    if(error != GL_NO_ERROR){
        LOG("[X] Send %s matrix error, Error %08x \n", message, error);
    }
}

static inline void check_error(const char* message){
    GLenum error = glGetError();
    if(error != GL_NO_ERROR){
        LOG("%s, Error %08x \n", message, error);
    }
}

static inline GLint get_uniform_location(GLuint shader, const char* name){
    GLint uniform = glGetUniformLocation(shader,name);
    if(uniform == -1){
        mvp_error(name);
    }
    return uniform;
}

void update_draw_vertices(GLuint shader, GLuint buffer, mat4 model_matrix){

    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    glUseProgram(shader);   

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(struct Vertex),(void*)0);

    GLint mvp_uniform =  get_uniform_location(shader,"MVP");
    if(mvp_uniform == -1){
        
        GLint model_uniform = get_uniform_location(shader,"model");

        GLint projection_uniform = get_uniform_location(shader,"projection");
        GLint view_uniform = get_uniform_location(shader,"view");
        GLint joints_matrices_uniform = get_uniform_location(shader,"joint_matrix");

        glUniformMatrix4fv(model_uniform, 1, GL_FALSE, &model_matrix[0][0]);
        glUniformMatrix4fv(projection_uniform, 1, GL_FALSE, &main_camera.projection[0][0]);
        glUniformMatrix4fv(view_uniform, 1, GL_FALSE, &main_camera.view[0][0]);
        check_send_matrix_error("view");

        SkinnedMeshComponent* skin_component = get_component_from_selected_element(COMPONENT_SKINNED_MESH);
        glUniformMatrix4fv(joints_matrices_uniform,skin_component->node_uniform.joint_count , GL_FALSE, skin_component->node_uniform.joints_matrix);
        check_send_matrix_error("Skin");
        
        glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, joint));

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, weight));
        
    }else{
        mat4 mvp;      
        update_mvp(model_matrix, mvp);  
        glUniformMatrix4fv(mvp_uniform, 1, GL_FALSE, &mvp[0][0]);
        check_send_matrix_error("MVP");
    }    

}
void draw_model_with_color(Model* model, GLenum mode, vec4 color){
    update_draw_vertices(model->shader, model->vertex_buffer_id, model->model_mat);
    GLint uniform_color = get_uniform_location(model->shader,"color");
    
    glUniform4fv(uniform_color, 1, color);
    check_error("color matrix error");
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,model->index_buffer_id);
    
    glDrawElements(mode,model->index_array.count, GL_UNSIGNED_SHORT, (void*)0);
}

void draw_model_like(Model* model, GLenum mode){
    update_draw_vertices(model->shader, model->vertex_buffer_id, model->model_mat);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,model->index_buffer_id);
    
    glDrawElements(mode,model->index_array.count, GL_UNSIGNED_SHORT, (void*)0);
}

void draw_simgle_model(struct Model * new_model){
    glBindTexture(GL_TEXTURE_2D,new_model->texture.id);
    update_draw_vertices(new_model->shader,new_model->vertex_buffer_id,new_model->model_mat);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,2, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (void*)offsetof(struct Vertex, uv));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,new_model->index_buffer_id);

    if(new_model->index_array.count == 0)
        LOG("Index is equal to 0, model not render\n");
    glDrawElements(GL_TRIANGLES, new_model->index_array.count , GL_UNSIGNED_SHORT, (void*)0);

    GLenum error;
    error = glGetError();
    if(error != GL_NO_ERROR){
        LOG("draw error\n");
        LOG("Error %08x \n",error);
    }
}