#ifndef ENGINE_COLLISION_H
#define ENGINE_COLLISION_H

#include "../types.h"

typedef struct Model Model;


bool collision_of(Model* model, Model* model2);

#endif // !ENGINE_COLLISION_H