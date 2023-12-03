#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;

layout(location = 0) out vec3 frag_color;

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

const vec3 LIGHT_DIRECTION = normalize(vec3(3.0,3.0,3.0));

const float AMBIENT = 0.03f;

void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(position, 1.0);
   
    vec3 normal_world_space = normalize(mat3(ubo.model) * normal);

    float light_intensity = AMBIENT + max(dot(normal_world_space, LIGHT_DIRECTION), 0);

    frag_color = light_intensity * vec3(1,0,0);

}
