#include <engine/renderer/renderer.h>
#include <GL/gl.h>

GLubyte* frame;

void pe_render_rasterize_init(){
    frame = malloc(400*400*3);
    memset(frame, 0,(400*400*3));
    
}

void pe_render_rasterize(){
   
    glDrawPixels(400,400,GL_RGB, GL_UNSIGNED_BYTE, frame) ;


}
