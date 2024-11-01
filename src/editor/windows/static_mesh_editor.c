#include "static_mesh_editor.h"
#include "../editor.h"

void editor_window_static_mesh_draw(){
	
    render_clear_buffer(RENDER_COLOR_BUFFER | RENDER_DEPTH_BUFFER); 
	draw_gizmos();	

}

