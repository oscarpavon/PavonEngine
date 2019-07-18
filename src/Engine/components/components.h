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
    TRASNFORM_COMPONENT,
    SPHERE_COMPONENT,
    CAMERA_COMPONENT,
    CUBE_COMPONENT,
    STATIC_MESH_COMPONENT
}ComponentType;


typedef struct ComponentDefinition{
    ComponentType type;
    Element* parent;
    unsigned int id;
    unsigned short int bytes_size;
    void* data;
}ComponentDefinition;

typedef struct SphereComponent{    
    int radius;
    vec3 center;
    Model* model;
}SphereComponent;

typedef struct CubeComponent{    
    vec3 center;
    Model* model;
}CubeComponent;

typedef struct TransformComponent{    
    vec3 position;
    vec4 rotation;
    vec3 scale;
    mat4 model_matrix;
}TransformComponent;

typedef struct StaticMeshComponent{    
    Model* model;
}StaticMeshComponent;

typedef struct SkinnedMeshComponent{    
    Model* model;
}SkinnedMeshComponent;

void add_component_to_selected_element(int component_byte_size, void* new_component, ComponentType type);
void init_sphere_component(SphereComponent* component);
void init_cube_component(CubeComponent* component);
void update_component(ComponentDefinition*);
void init_transfrom_component(TransformComponent* component);

#endif