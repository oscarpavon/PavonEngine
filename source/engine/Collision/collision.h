#ifndef ENGINE_COLLISION_H
#define ENGINE_COLLISION_H

#include "../types.h"

typedef struct Model PModel;


bool collision_of(PModel* model, PModel* model2);

#endif // !ENGINE_COLLISION_H
