//
// Created by pavon on 6/23/19.
//

#ifndef PAVONTHEGAME_GUI_H
#define PAVONTHEGAME_GUI_H

#include "utils.h"
#include "vector.h"
#include <GLES2/gl2.h>



typedef struct Button{
    bool pressed;
    vec2 size;
    vec2 position;


    VertexArray vertex_array;
    GLuint vertex_buffer;
    GLuint shader;
}Button;

Button button1;
Button shoot_button;

Array buttons;

void init_button();
void draw_button();
void draw_gui();

#endif //PAVONTHEGAME_GUI_H
