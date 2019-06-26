#include "input.h"

#include "../utils.h"

#include <GLFW/glfw3.h>
#include <stdlib.h>
#include "../engine.h"

#include <cglm/cglm.h>
#include "../camera.h"

#include <math.h>

#include "editor.h"

#include "text.h"

bool move_camera;

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
            engine->input.D.Released = true;
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
    if(key == GLFW_KEY_G){
        if(action == GLFW_PRESS){
            engine->input.G.bIsPressed = true;
        }
        if(action == GLFW_RELEASE){
            engine->input.G.bIsPressed = false;
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



void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){

		if (button == GLFW_MOUSE_BUTTON_RIGHT ){
			if(action == GLFW_PRESS){
				
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);  
                move_camera = false;
				
			}
			if(action == GLFW_RELEASE){
			
			}

		}
		if (button == GLFW_MOUSE_BUTTON_LEFT ){
			if(action == GLFW_PRESS){
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
                move_camera = true;
			
			}
			if(action == GLFW_RELEASE){
				
			}

		}

}

void update_input(Engine* engine){
    if(!move_camera){
        if(engine->input.S.bIsPressed){
            get_element_status(selected_element);
        }
        if(engine->input.G.bIsPressed){
            grab_mode = true;
        }
        if(engine->input.X.bIsPressed){
            save_data();
        }
        if(engine->input.Z.Released){
            load_level_in_editor();
        }
        if(engine->input.D.Released){
            draw_text_menu = true;
            engine->input.D.Released = false;
        }

        if(grab_mode){
            if(engine->input.A.bIsPressed){
                glm_translate(selected_element->model->model_mat, (vec3){0.02,0,0});
                glm_vec3_add(selected_element->position,(vec3){0.02,0,0},selected_element->position);
            }
            if(engine->input.D.bIsPressed){
                
            }
        }

    }

    
    if(move_camera){
        if(engine->input.W.bIsPressed){
            vec3 move;
            glm_vec3_mul((vec3){0.04,0.04,0.04},camera_front,move);
            glm_vec3_add(camera_position,move,camera_position);
            update_look_at();
        }
        if(engine->input.S.bIsPressed){
            vec3 move;
            glm_vec3_mul((vec3){0.04,0.04,0.04},camera_front,move);
            glm_vec3_sub(camera_position,move,camera_position);
            update_look_at();
        }
        if(engine->input.D.bIsPressed){
            vec3 cross;
            glm_vec3_cross(camera_front, camera_up, cross);
            glm_normalize(cross);
            vec3 move;
            glm_vec3_mul((vec3){0.04,0.04,0.04}, cross, move );
            glm_vec3_add(camera_position, move,camera_position);
            update_look_at();
        }
        if(engine->input.A.bIsPressed){
            vec3 cross;
            glm_vec3_cross(camera_front, camera_up, cross);
            glm_normalize(cross);
            vec3 move;
            glm_vec3_mul((vec3){0.04,0.04,0.04}, cross, move );
            glm_vec3_sub(camera_position, move,camera_position);
            update_look_at();
        }
    }
    
}

float last_mouse_x = 400;
float last_mouse_y = 300;
bool first_mouse_movement = true;



void camera_mouse_control(float yaw, float pitch){
    vec3 front;
    
    front[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
    front[1] = sin(glm_rad(pitch));
    front[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));

    

    glm_normalize(front);

    glm_vec3_copy(front, camera_front);

    update_look_at();
}
float horizontalAngle = 0;
float verticalAngle = 0;
void mouse_movement_control(float xpos, float ypos){
    
  
    
    horizontalAngle += 800/2 - xpos ;
    
    verticalAngle  += 600/2 - ypos ;

    horizontalAngle *= 0.05;
    verticalAngle *= 0.05;

    camera_mouse_control(0, horizontalAngle);
    

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
	if(move_camera)
        mouse_movement_control(xpos, ypos);
}