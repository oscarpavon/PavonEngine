//
// Created by pavon on 6/23/19.
//

#ifndef PAVONTHEGAME_GUI_H
#define PAVONTHEGAME_GUI_H

#include "utils.h"
#include "vector.h"
#include <GLES2/gl2.h>



struct Button{
    bool pressed;
    VertexArray vertex_array;
    GLuint vertex_buffer;
    GLuint shader;
};

struct Button button1;

void init_button();
void draw_button();


#endif //PAVONTHEGAME_GUI_H
