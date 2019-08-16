#include "content_browser.h"
#include "../../engine.h"

Model content_model;
bool initialized_test_content_view;

GLuint shader1;
GLuint shader2;
void editor_window_content_browser_draw(){
    glfwMakeContextCurrent(window_content_browser.window);

    glClearColor(1,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Model* model = get_from_array(actual_model_array,0);
    if(model){
        if(!initialized_test_content_view){
            duplicate_model_data(&content_model,model);
            initialized_test_content_view = true;
            content_model.shader = create_engine_shader(shader1,shader2);
        }
    }
    if(initialized_test_content_view){
        draw_simgle_model(model);
    }

    glfwSwapBuffers(window_content_browser.window);
}

void editor_window_content_init(){
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    shader1 = compile_shader(triVertShader, GL_VERTEX_SHADER);
    shader2 = compile_shader(triFragShader, GL_FRAGMENT_SHADER);
}