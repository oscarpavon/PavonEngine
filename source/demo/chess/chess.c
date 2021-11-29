#include "chess.h"

#include <engine/engine.h>

#include <engine/renderer/vulkan/vulkan.h>
#include <engine/model.h>

void chess_create_pieces(){
    pawn = pe_vk_model_load("/home/pavon/chess/pawn.glb");
    rook = pe_vk_model_load("/home/pavon/chess/rook.glb");


    glm_vec3_copy(VEC3(3,0,0),pawn->position);
}
