#ifndef ENGINE_COMPONENTS_H
#define ENGINE_COMPONENTS_H

typedef struct Element Element;
typedef struct Model Model;
#include <cglm/cglm.h>

typedef enum ComponentType{
    TRASNFORM_COMPONENT = 0,
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

typedef struct CameraComponent{
    mat4 projection;
    mat4 view;
    vec3 front;
    vec3 up;
    vec3 position;
    Model* camera_gizmo;
}CameraComponent;

typedef struct StaticMeshComponent{
    int model_id;    
    Model* model;
}StaticMeshComponent;

typedef struct SkinnedMeshComponent{    
    Model* model;
}SkinnedMeshComponent;

void add_component_to_selected_element(int component_byte_size, void* new_component, ComponentType type);
void init_sphere_component(SphereComponent* component);
void init_camera_component(CameraComponent* component);
void init_cube_component(CubeComponent* component);
void update_component(ComponentDefinition*);
void init_transfrom_component(TransformComponent* component);
void* get_component_from_selected_element(ComponentType type);
void* get_component_from_element(Element* element, ComponentType type);
void change_view_to_camera_component(CameraComponent* camera_component);

#endif