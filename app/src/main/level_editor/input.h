#ifndef EDITOR_INPUT_H
#define EDITOR_INPUT_H

#include <GLFW/glfw3.h>
#include <stdbool.h>



typedef struct Key{
	bool pressed;
	bool Released;
}Key;

typedef struct Input {
	Key D;
	Key A;
	Key E;
	Key Q;
	Key X;
	Key Z;
	Key W;
	Key S;
	Key C;
	Key V;
	Key G;
	Key TAB;
	Key KEY_1;
	Key KEY_2;
	Key KEY_3;
	Key SPACE;
	Key ESC;
	Key SHIFT;
	Key R;
	Key ENTER;
	Key J;
	Key K;
	Key T;
	Key L;
}Input;


Input input;

void update_input();
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void init_input();

#endif // !EDITOR_INPUT_H
