glslc shader_vk_tri.frag -o frag.spv
glslc blue.frag -o blue_frag.spv
glslc other.vert -o other_vert.spv
glslc in_position.vert -o in_position.spv
glslc shader_vk_tri.vert -o vert.spv
echo "[OK]Shaders compiled"
