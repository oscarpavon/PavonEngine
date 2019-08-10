
#include "render.h"
#include "../../engine.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../third_party/stb_image_write.h"
#include "../windows.h"

Model *uv_model;
GLuint renderedTexture;
void *export_pixels;
GLint old_fbo;
Model *fireman;
Model *UV_true;

void draw()
{
    glBindBuffer(GL_ARRAY_BUFFER, uv_model->vertex_buffer_id);

    glUseProgram(uv_model->shader);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (void *)offsetof(struct Vertex, uv));

    glBindTexture(GL_TEXTURE_2D, uv_model->texture.id);
    GLint mvp_uniform = glGetUniformLocation(uv_model->shader, "MVP");

    mat4 scale;
    glm_mat4_identity(scale);

    mat4 position_mat;
    glm_mat4_identity(position_mat);

    mat4 projection;
    glm_mat4_identity(projection);

    glm_ortho(0, 512, 512, 0, 0, 1, projection);

    glm_scale(scale, (vec3){512, 512, 0});

    glm_translate(position_mat, (vec3){0, 0, 0});

    mat4 model;
    glm_mul(position_mat, scale, model);

    mat4 model_projection;
    glm_mat4_identity(model_projection);

    glm_mul(projection, model, model_projection);

    glUniformMatrix4fv(mvp_uniform, 1, GL_FALSE, &model_projection[0][0]);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uv_model->index_buffer_id);

    if (uv_model->index_array.count == 0)
        LOG("Index is equal to 0, model not render\n");
    glDrawElements(GL_TRIANGLES, uv_model->index_array.count, GL_UNSIGNED_SHORT, (void *)0);
}

void render_to_texture()
{
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo);

    GLuint FramebufferName = 0;
    glGenFramebuffers(1, &FramebufferName);
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

    glGenTextures(1, &renderedTexture);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, renderedTexture);

    // Give an empty image to OpenGL ( the last "0" )
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // Poor filtering. Needed !
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 512, 512);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           renderedTexture, 0);

    // Render to our framebuffer

    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

    glViewport(0, 0, 512, 512); // Render on the whole framebuffer, complete from the lower left corner to the upper right

    glClearColor(0.3, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    uv_model = UV_true;

    //draw_editor_viewport();
    glClear(GL_DEPTH_BUFFER_BIT);

    draw();

    export_pixels = malloc(512 * 512 * 3);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, 512, 512, GL_RGB, GL_UNSIGNED_BYTE, export_pixels);

    stbi_write_png("test.png", 512, 512, 3, export_pixels, 512 * 3);

    glBindFramebuffer(GL_FRAMEBUFFER, old_fbo);
    glViewport(0, 0, actual_window_width, actual_window_height);
}

void init_UV_draw(Model *model)
{
    fireman = model;
    new_empty_model();
    init_array(&selected_model->vertex_array, sizeof(Vertex), model->vertex_array.count);
    for (int i = 0; i < model->vertex_array.count; i++)
    {
        Vertex *vertex = get_from_array(&model->vertex_array, i);
        vec3 position;
        glm_vec3_copy(VEC3(vertex->uv[0], vertex->uv[1], 0), position);
        add_to_array(&selected_model->vertex_array, position);
    }

    init_model_gl_buffers(selected_model);
    selected_model->index_buffer_id = model->index_buffer_id;
    selected_model->index_array.count = model->index_array.count;

    selected_model->shader = create_engine_shader(standart_vertex_shader, standart_fragment_shader);
    selected_model->texture.id = model->texture.id;
    uv_model = fireman;
    UV_true = selected_model;
    render_to_texture();
}

bool initialized = false;

void draw_UV()
{
    if (!uv_model)
    {
        if (selected_element)
        {
            if (selected_model->texture.id > 0)
                if (!uv_model)
                {
                    initialized = true;
                    init_UV_draw(selected_model);
                }
                else
                {
                    return;
                }
        }
        else
        {
            return;
        }
    }
    if (!uv_model)
        return;
    if (!uv_model->vertex_array.initialized)
    {

        return;
    }

    glBindBuffer(GL_ARRAY_BUFFER, uv_model->vertex_buffer_id);

    glUseProgram(uv_model->shader);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (void *)offsetof(struct Vertex, uv));

    glBindTexture(GL_TEXTURE_2D, uv_model->texture.id);
    GLint mvp_uniform = glGetUniformLocation(uv_model->shader, "MVP");
    //glDrawArrays(GL_POINTS, 0, uv_model->vertex_array.count);
    //draw_simgle_model(uv_model);
    mat4 mvp;
    glm_mat4_identity(mvp);
    glm_rotate(mvp, 180, VEC3(0, 1, 0));
    glUniformMatrix4fv(mvp_uniform, 1, GL_FALSE, &mvp[0][0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uv_model->index_buffer_id);

    if (uv_model->index_array.count == 0)
        LOG("Index is equal to 0, model not render\n");
    glDrawElements(GL_TRIANGLES, uv_model->index_array.count, GL_UNSIGNED_SHORT, (void *)0);
}
