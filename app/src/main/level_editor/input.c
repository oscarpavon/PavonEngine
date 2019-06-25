#include "input.h"

#include "../utils.h"

#include <GLFW/glfw3.h>
#include <stdlib.h>
#include "../engine.h"

#include <cglm.h>
#include "../camera.h"


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){

    Engine* engine = glfwGetWindowUserPointer(window);
	
    if(key == GLFW_KEY_TAB){
        if(action == GLFW_PRESS){
            engine->input.TAB.bIsPressed = true;
            engine->input.TAB.Released = false;
        }
        if(action == GLFW_RELEASE){
            engine->input.TAB.bIsPressed = false;
            engine->input.TAB.Released = true;
        }
    }
    if(key == GLFW_KEY_S){
        if(action == GLFW_PRESS){
            engine->input.S.bIsPressed = true;
        }
        if(action == GLFW_RELEASE){
            engine->input.S.bIsPressed = false;
        }
    }
    if(key == GLFW_KEY_W){
        if(action == GLFW_PRESS){
            engine->input.W.bIsPressed = true;
        }
        if(action == GLFW_RELEASE){
            engine->input.W.bIsPressed = false;
        }
    }
    if(key == GLFW_KEY_D){
        if(action == GLFW_PRESS){
            engine->input.D.bIsPressed = true;
        }
        if(action == GLFW_RELEASE){
            engine->input.D.bIsPressed = false;
        }
    }
    if(key == GLFW_KEY_A){
        if(action == GLFW_PRESS){
            engine->input.A.bIsPressed = true;
        }
        if(action == GLFW_RELEASE){
            engine->input.A.bIsPressed = false;
        }
    }
    if(key == GLFW_KEY_Z){
        if(action == GLFW_PRESS){
            engine->input.Z.bIsPressed = true;
        }
        if(action == GLFW_RELEASE){
            engine->input.Z.bIsPressed = false;
        }
    }
    
    if(key == GLFW_KEY_E){
        if(action == GLFW_PRESS){
            engine->input.E.bIsPressed = true;
        }
        if(action == GLFW_RELEASE){
            engine->input.E.bIsPressed = false;
        }
    }
    if(key == GLFW_KEY_Q){
        if(action == GLFW_PRESS){
            engine->input.Q.bIsPressed = true;
        }
        if(action == GLFW_RELEASE){
            engine->input.Q.bIsPressed = false;
        }
    }
    if(key == GLFW_KEY_X){
        if(action == GLFW_PRESS){
            engine->input.X.bIsPressed = true;
        }
        if(action == GLFW_RELEASE){
            engine->input.X.bIsPressed = false;
        }
    }
    if(key == GLFW_KEY_V){
        if(action == GLFW_PRESS){
            engine->input.V.bIsPressed = true;
        }
        if(action == GLFW_RELEASE){
            engine->input.V.bIsPressed = false;
        }
    }
  
           
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
	

}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){

		if (button == GLFW_MOUSE_BUTTON_RIGHT ){
			if(action == GLFW_PRESS){
				
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);  
				
			}
			if(action == GLFW_RELEASE){
			
			}

		}
		if (button == GLFW_MOUSE_BUTTON_LEFT ){
			if(action == GLFW_PRESS){
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
			
			}
			if(action == GLFW_RELEASE){
				
			}

		}

}

void update_input(Engine* engine){
    if(engine->input.W.bIsPressed){
        glm_translate(main_camera.view, (vec3){0,0.01,0});
        
    }
}