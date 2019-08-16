#include "content_browser.h"
#include "../../engine.h"

Model content_model;

void editor_window_content_browser_draw(){
    glfwMakeContextCurrent(window_content_browser.window);

    glClearColor(1,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Model* model = get_from_array(actual_model_array,0);
    if(model){
        draw_simgle_model(model);
    }
   
    draw_engine_memory();
    
    glfwSwapBuffers(window_content_browser.window);
}

void editor_window_content_init(){
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

}