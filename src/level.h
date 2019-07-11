//
// Created by pavon on 6/22/19.
//

#ifndef PAVONTHEGAME_LEVEL_H
#define PAVONTHEGAME_LEVEL_H

#include "vector.h"

typedef struct level{
    const char* name;
    ModelArray models_array;
}Level;


void load_level(const char* path, Level* output_level);

#endif //PAVONTHEGAME_LEVEL_H
