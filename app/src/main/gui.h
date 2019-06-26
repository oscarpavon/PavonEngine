//
// Created by pavon on 6/23/19.
//

#ifndef PAVONTHEGAME_GUI_H
#define PAVONTHEGAME_GUI_H

#include "utils.h"
#include "vector.h"
#include <GLES2/gl2.h>


#define POSITION_RELATIVE_LEFT_BOTTON 0
#define POSITION_RELATIVE_LEFT_TOP 1


typedef struct Button{
    bool pressed;
    bool released;

    vec2 size;
    vec2 position;
    unsigned short int relative_to;
    vec2 original_position;

    GLuint shader;
}Button;


typedef struct Menu{
    Array buttons;
}Menu;

Button button1;
Button shoot_button;
Button back_button;
Button virtual_joystick;

Array buttons;

void init_gui();
void draw_button();
void draw_gui();


#endif //PAVONTHEGAME_GUI_H
