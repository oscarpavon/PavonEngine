#ifndef ENGINE_COMPONENTS_H
#define ENGINE_COMPONENTS_H

#include "../../engine.h"

typedef struct CameraComponent{
    mat4 projection;
    mat4 view;
    vec3 front;
    vec3 up;
}CameraComponent;

typedef enum ComponentType{
    SPHERE_COMPONENT,
    CAMERA_COMPONENT,
    CUBE_COMPONENT
}ComponentType;

typedef struct SphereComponent{
    ComponentType type;
    int radius;
    vec3 position;
    Model* model;
}SphereComponent;

void add_component_to_selected_element(int component_byte_size, void* new_component);
void init_sphere_component(SphereComponent* component);

#endif