#version 100
attribute vec3 vPosition;
attribute vec2 inUV;
attribute vec3 inNormal;

varying vec2 v_TexCoord; 

uniform mat4 MVP; 
uniform mat4 model;

varying vec3 Normal;
varying vec3 FragPosition;

void main()
{
    gl_Position = MVP * vec4(vPosition,1);
//   gl_PointSize = 10.0;
   // FragPosition = vec3(model * vec4(vPosition,1));
    //v_TexCoord = inUV;
    //Normal = inNormal;
}
//end
