#version 450


layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform PEColorShader{
    float x;
    float y;
    float z;
} sh;

void main() {
    outColor = vec4(sh.x,sh.y,sh.z,1);
}
